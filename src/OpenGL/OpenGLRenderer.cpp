/**
 * Class name:    OpenGLRenderer

 * Description:   Uses OpenGLs Vertex Buffer Objects to render the mesh, 
                  results and regions.

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "freeglut_std.h"
#include "freeglut_ext.h"
#include "OpenGLRenderer.h"
#include "Vector.h"
#include "OpenGLProgram.h"
#include "OpenGLExtensions.h"
#include "StreamUtils.h"

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

vec4 OpenGLRenderer::s_colorResult				(0.0f, 0.0f, 0.0f, 1.0f);
vec4 OpenGLRenderer::s_colorResultBackground	(0.8f, 0.8f, 1.0f, 1.0f);
vec4 OpenGLRenderer::s_colorRegion				(0.0f, 0.0f, 0.0f, 1.0f);
vec4 OpenGLRenderer::s_colorRegionLeft			(1.0f, 0.8f, 0.4f, 1.0f);
vec4 OpenGLRenderer::s_colorBB					(0.5f, 0.5f, 0.7f, 1.0f);
vec4 OpenGLRenderer::s_colorTriangleHit		(0.4f, 0.4f, 0.8f, 1.0f);
vec4 OpenGLRenderer::s_colorAxes					(0.9f, 0.5f, 0.0f, 1.0f);
vec4 OpenGLRenderer::s_colorBasis				(0.4f, 0.4f, 0.4f, 1.0f);
vec4 OpenGLRenderer::s_colorSearchPoint		(0.0f, 1.0f, 0.0f, 1.0f);
vec4 OpenGLRenderer::s_clearColor				(1.0f, 1.0f, 1.0f, 1.0f);

const TCHAR*	OpenGLRenderer::SEARCH_CHAPTER	= _T("Search Settings");
const TCHAR*	OpenGLRenderer::CAMERA_CHAPTER	= _T("Camera Settings");


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLRenderer::OpenGLRenderer() 
:
   m_isValidOpenGLVersion	(false),
	m_useLines					(true),
	m_hasTriangleHit			(false),
	m_hasSearchPos				(false),
   m_indexBufferID			(0),
   m_numVerticesAxes			(0),
   m_numIndexesMesh			(0),
   m_numVerticesResult		(0),
	m_numVerticesRegionsLeft(0),
	m_startIdxTriangleHit	(0),
	m_defaultLineWidth		(1.0),
	m_defaultPointSize		(1.0),
	m_lenMaxEdge				(0.0),
	m_lenDiagonal				(0.0),
	m_center						(0.0, 0.0),
	m_errorMessage				(""),
	m_verString					(NULL),
	m_program					(NULL)
{
	memset(m_vboIds, 0, MAX_IDS*sizeof(GLuint));
	memset(m_vaoIds, 0, MAX_IDS*sizeof(GLuint));
	memset(m_OpenGLVersion, 0, 2*sizeof(GLint));

	m_defaultPolygonMode[0] = POLYGONMODE;
	m_defaultPolygonMode[1] = GL_LINE;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

OpenGLRenderer::~OpenGLRenderer()
{
	delete m_program;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::writeSettingsToFile(const TCHAR* filename)
{
	tstringstream output;

	StreamUtils::resetStream(output);
	output << getCameraPos().x;
	WritePrivateProfileString(CAMERA_CHAPTER, _T("CameraPosX"), 
		output.str().c_str(), filename);

	StreamUtils::resetStream(output);
	output << getCameraPos().y;
	WritePrivateProfileString(CAMERA_CHAPTER, _T("CameraPosY"), 
		output.str().c_str(), filename);		

	StreamUtils::resetStream(output);
	output << getCameraPos().z;
	WritePrivateProfileString(CAMERA_CHAPTER, _T("CameraPosZ"), 
		output.str().c_str(), filename);

	if (!hasSearchResults())
	{
		// there are no results
		return true;
	}	
	StreamUtils::resetStream(output);
	output << getSearchPos().x;
	WritePrivateProfileString(SEARCH_CHAPTER, _T("SearchPosX"), 
		output.str().c_str(), filename);

	StreamUtils::resetStream(output);
	output << getSearchPos().y;
	WritePrivateProfileString(SEARCH_CHAPTER, _T("SearchPosY"), 
		output.str().c_str(), filename);		

	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::readSettingsFromFile(const TCHAR* filename)
{
	m_hasSearchPos	= false;

	TCHAR	buf2[3][MAX_BUFSIZE_2];

	GetPrivateProfileString(CAMERA_CHAPTER, _T("CameraPosX"), _T(""),
		buf2[0], MAX_BUFSIZE_2, filename);
	GetPrivateProfileString(CAMERA_CHAPTER, _T("CameraPosY"), _T(""),
		buf2[1], MAX_BUFSIZE_2, filename);
	GetPrivateProfileString(CAMERA_CHAPTER, _T("CameraPosZ"), _T(""),
		buf2[2], MAX_BUFSIZE_2, filename);

	vec3 cameraPos;
	_stscanf_s(buf2[0], _T(PREC_SCAN_2), &cameraPos.x);
	_stscanf_s(buf2[1], _T(PREC_SCAN_2), &cameraPos.y);
	_stscanf_s(buf2[2], _T(PREC_SCAN_2), &cameraPos.z);

	updateCamera(cameraPos);

	GetPrivateProfileString(SEARCH_CHAPTER, _T("SearchPosX"), _T(""),
		buf2[0], MAX_BUFSIZE_2, filename);
	GetPrivateProfileString(SEARCH_CHAPTER, _T("SearchPosY"), _T(""),
		buf2[1], MAX_BUFSIZE_2, filename);

	Vec2D vec2;
	int res1 = _stscanf_s(buf2[0], _T(PREC_SCAN_2), &vec2.x);
	int res2 = _stscanf_s(buf2[1], _T(PREC_SCAN_2), &vec2.y);

	if (res1 == 1 && res2 == 1)
	{
		setSearchPos(vec2);
	}
	return true;
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::setSearchPos(Vec2D& vec2)
{
	m_hasSearchPos = m_renderData.setSearchPos(vec2);	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const Vec2D& OpenGLRenderer::getSearchPos()
{
	return m_renderData.getSearchPos();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

vec3 OpenGLRenderer::getCameraPos()
{
	return m_camera.getCameraPos();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const vec2* OpenGLRenderer::getBoundingBox()
{
	return m_renderData.getBoundingBox();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const MeshData& OpenGLRenderer::getMesh()
{
	return m_renderData.getMesh();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::hasSearchResults()
{
	return m_renderData.hasSearchResults();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::resetResults()
{
	m_renderData.resetResults();
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const char*	OpenGLRenderer::getInfoLog()
{
	return m_program->getInfoLog();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::initRenderStates()
{
	glClearColor(s_clearColor[0], s_clearColor[1], s_clearColor[2], s_clearColor[3]);
	clearBuffer();	
	glPolygonMode(m_defaultPolygonMode[0], m_defaultPolygonMode[1]);   
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);	
	glDepthRange(0, 1);

	glGenVertexArrays(MAX_IDS, m_vaoIds);		
	glGenBuffers(MAX_IDS, m_vboIds);
	glGenBuffers(1, &m_indexBufferID);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::buildShaderProgram()
{	
	m_program			= new OpenGLProgram();
	return m_program->buildShaderProgram();
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::cleanup()
{
	if (m_isValidOpenGLVersion)
	{
		glDisableVertexAttribArray(VERTEX_POS_ATTRIB);
		glDisableVertexAttribArray(VERTEX_COLOR_ATTRIB);
		cleanupVBOs();
		m_program->cleanup();
	}
	m_isValidOpenGLVersion = false;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::cleanupVBOs()
{
	glDeleteVertexArrays(MAX_IDS, m_vaoIds);		
	glDeleteBuffers(MAX_IDS, m_vboIds);
	glDeleteBuffers(1, &m_indexBufferID);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


bool OpenGLRenderer::checkOpenGLVersion()
{
	HGLRC context = OpenGLExtensions::wglGetCurrentContext();

	if (context == NULL)
	{
		return false;
	}


#ifdef FREEGLUT_RUNTIME_LINKING
	if (!OpenGLExtensions::initCoreFunctions())
	{
		return false;
	}
#endif

	m_verString = (const char*)glGetString(GL_VERSION);

	glGetIntegerv(GL_MAJOR_VERSION, &m_OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &m_OpenGLVersion[1]);
	if (	m_OpenGLVersion[0] < MAJOR_VERSION_GL 
		|| m_OpenGLVersion[1] < MINOR_VERSION_GL)
	{
		return false;
	}

	if (!OpenGLExtensions::initExtensions())
	{
		return false;
	}
	bool hasExtensionFP64	= true;
	bool hasExtension64bit	= true;
   bool hasExtensionVBO		= OpenGLExtensions::isExtensionSupported("GL_ARB_vertex_buffer_object");
	bool hasExtensionVAO		= OpenGLExtensions::isExtensionSupported("GL_ARB_vertex_array_object");
#ifdef DOUBLE_PRECISION
	hasExtensionFP64			= OpenGLExtensions::isExtensionSupported("GL_ARB_gpu_shader_fp64");
	hasExtension64bit			= OpenGLExtensions::isExtensionSupported("GL_ARB_vertex_attrib_64bit");
#endif	
   
	if (!hasExtensionFP64 
		|| !hasExtensionVAO 
		|| !hasExtensionFP64
		|| !hasExtension64bit)
   {
      return false;
   }
   m_isValidOpenGLVersion = true;
   return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

const char* OpenGLRenderer::buildErrorMessage()
{
	char buf[20];
	
	int res = sprintf_s(buf, 20, "%d.%d", MAJOR_VERSION_GL, MINOR_VERSION_GL);
	if (res != -1)
	{	
		m_errorMessage += "Your OpenGL graphics card driver is not supported.\n";
		m_errorMessage += "Please update your driver to version: ";
		m_errorMessage += buf;
	}
	return m_errorMessage.c_str();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::setPerspective(LONG width, LONG height)
{
	const mat4& matMVP = m_camera.setPerspective(width, height); 
	m_program->updateMVPMatrix(matMVP);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::updateCamera(const vec3& cameraPos)
{
	const mat4& matMVP = m_camera.updateCamera(cameraPos);
	m_program->updateMVPMatrix(matMVP);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::validateCameraPosition(vec3& cameraPos)
{
	return m_camera.validateCameraPosition(cameraPos);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::computeCursorPos(int x, int y, vec2& cursorPos)
{
	return m_camera.computeCursorPos(x, y, cursorPos);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();		
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::drawScene()
{            
   TRACE(_T("+++ DRAW SCENE +++\n"));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawBaseMesh();
	drawBoundingBox();

   if (m_renderData.hasSearchResults())
   {      
		drawRegionsLeft();
		drawResult();		
		drawAxes();
		drawRegionAndPoint();
   }      
	else
	{
		drawAxes();
		drawRegionsLeft();
	}
		
	glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
   glutSwapBuffers();
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::createRegionVBO()
{
	Vertex_2V*	verticesRegionVBO			= new Vertex_2V[5];
	const vec2* boundingBoxResult			= m_renderData.getBBResult();

	// add four vertices for the bounding box of the current region
	verticesRegionVBO[0].m_point.x		= boundingBoxResult[0].x;
	verticesRegionVBO[0].m_point.y		= boundingBoxResult[0].y;

	verticesRegionVBO[1].m_point.x		= boundingBoxResult[1].x;
	verticesRegionVBO[1].m_point.y		= boundingBoxResult[0].y;

	verticesRegionVBO[2].m_point.x		= boundingBoxResult[1].x;
	verticesRegionVBO[2].m_point.y		= boundingBoxResult[1].y;

	verticesRegionVBO[3].m_point.x		= boundingBoxResult[0].x;
	verticesRegionVBO[3].m_point.y		= boundingBoxResult[1].y;

	// add search position
	verticesRegionVBO[4].m_point			= m_renderData.getSearchPos();

	// bind and fill VBO data for region
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[REGION_SEARCHPOINT]);
	glBufferData(GL_ARRAY_BUFFER, 5 * STRIDE_2V, 
						verticesRegionVBO, GL_STATIC_DRAW);

	// bind VAO data for region
	glBindVertexArray(m_vaoIds[REGION_SEARCHPOINT]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);   

	// unbind VBO and VAO data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] verticesRegionVBO;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::createRegionsLeftVBO()
{
	const PointPtrArray* regionsLeft		= m_renderData.getRegionsLeft();
	m_numVerticesRegionsLeft				= regionsLeft->size()*4;	
	Vertex_2V*	verticesRegionLeftVBO	= new Vertex_2V[m_numVerticesRegionsLeft];

	size_t k = -1;
	for (size_t i = 0; i < regionsLeft->size(); i++)
	{		
		const vec2* region    = regionsLeft->at(i);

		verticesRegionLeftVBO[++k].m_point.x	= region[0].x;
		verticesRegionLeftVBO[k].m_point.y		= region[0].y;

		verticesRegionLeftVBO[++k].m_point.x	= region[1].x;
		verticesRegionLeftVBO[k].m_point.y		= region[0].y;

		verticesRegionLeftVBO[++k].m_point.x	= region[1].x;
		verticesRegionLeftVBO[k].m_point.y		= region[1].y;

		verticesRegionLeftVBO[++k].m_point.x	= region[0].x;
		verticesRegionLeftVBO[k].m_point.y		= region[1].y;
	}

	// bind and fill VBO data for regions
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[REGIONS_LEFT]);
	glBufferData(GL_ARRAY_BUFFER, m_numVerticesRegionsLeft*STRIDE_2V, 
						verticesRegionLeftVBO, GL_STATIC_DRAW);

	// bind VAO data for regions
	glBindVertexArray(m_vaoIds[REGIONS_LEFT]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);   

	// unbind VBO and VAO data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] verticesRegionLeftVBO;
}
/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::createBoundingBoxVBO()
{
	Vertex_2V*	verticesBoundingBoxVBO	= new Vertex_2V[16];
	const vec2* boundingBox					= m_renderData.getBoundingBox();

	REAL offsetX = boundingBox[1].x - boundingBox[0].x;
	REAL offsetY = boundingBox[1].y - boundingBox[0].y;

	offsetX *= 0.1;
	offsetY *= 0.1;		

	// lower left vertices
	verticesBoundingBoxVBO[0].m_point.x		= boundingBox[0].x;
	verticesBoundingBoxVBO[0].m_point.y		= boundingBox[0].y;

	verticesBoundingBoxVBO[1].m_point		= verticesBoundingBoxVBO[0].m_point;
	verticesBoundingBoxVBO[1].m_point.x		+= offsetX;

	verticesBoundingBoxVBO[2].m_point.x		= boundingBox[0].x;
	verticesBoundingBoxVBO[2].m_point.y		= boundingBox[0].y;

	verticesBoundingBoxVBO[3].m_point		= verticesBoundingBoxVBO[0].m_point;
	verticesBoundingBoxVBO[3].m_point.y		+= offsetY;

	// lower right vertices
	verticesBoundingBoxVBO[4].m_point.x		= boundingBox[1].x;
	verticesBoundingBoxVBO[4].m_point.y		= boundingBox[0].y;
	
	verticesBoundingBoxVBO[5].m_point		= verticesBoundingBoxVBO[4].m_point;
	verticesBoundingBoxVBO[5].m_point.x		-= offsetX;		

	verticesBoundingBoxVBO[6].m_point.x		= boundingBox[1].x;
	verticesBoundingBoxVBO[6].m_point.y		= boundingBox[0].y;

	verticesBoundingBoxVBO[7].m_point		= verticesBoundingBoxVBO[4].m_point;
	verticesBoundingBoxVBO[7].m_point.y		+= offsetY;		

	// upper left vertices
	verticesBoundingBoxVBO[8].m_point.x		= boundingBox[0].x;
	verticesBoundingBoxVBO[8].m_point.y		= boundingBox[1].y;

	verticesBoundingBoxVBO[9].m_point		= verticesBoundingBoxVBO[8].m_point;
	verticesBoundingBoxVBO[9].m_point.x		+= offsetX;		

	verticesBoundingBoxVBO[10].m_point.x	= boundingBox[0].x;
	verticesBoundingBoxVBO[10].m_point.y	= boundingBox[1].y;

	verticesBoundingBoxVBO[11].m_point		= verticesBoundingBoxVBO[8].m_point;
	verticesBoundingBoxVBO[11].m_point.y	-= offsetY;		

	// upper right vertices
	verticesBoundingBoxVBO[12].m_point.x	= boundingBox[1].x;
	verticesBoundingBoxVBO[12].m_point.y	= boundingBox[1].y;

	verticesBoundingBoxVBO[13].m_point		= verticesBoundingBoxVBO[12].m_point;
	verticesBoundingBoxVBO[13].m_point.x	-= offsetX;		

	verticesBoundingBoxVBO[14].m_point.x	= boundingBox[1].x;
	verticesBoundingBoxVBO[14].m_point.y	= boundingBox[1].y;

	verticesBoundingBoxVBO[15].m_point		= verticesBoundingBoxVBO[12].m_point;
	verticesBoundingBoxVBO[15].m_point.y	-= offsetY;		

	// bind and fill VBO data for regions
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[BOUNDINGBOX]);
	glBufferData(GL_ARRAY_BUFFER, 16 * STRIDE_2V, 
						verticesBoundingBoxVBO, GL_STATIC_DRAW);

	// bind VAO data for regions
	glBindVertexArray(m_vaoIds[BOUNDINGBOX]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);   

	// unbind VBO and VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] verticesBoundingBoxVBO;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void OpenGLRenderer::createResultVBO()
{		
	const PointArray& points					= m_renderData.getMesh().getPointArray();
	const IntArray&	indexes					= m_renderData.getMesh().getIndexArray();
	const IntArray*   indexResults			= m_renderData.getIndexResults();
	m_numVerticesResult							= (indexResults->size() * 3);   
	Vertex_2V*			verticesResultVBO		=	new Vertex_2V[m_numVerticesResult];
	size_t				j							=	-1;	
	m_hasTriangleHit								= false;
	
	// Fill vbo array with vertex data.
	// The vertices of a triangle have the following vbo layout: 
	// [x|y] [x|y] [x|y]

	// We iterate over the triangles of the mesh result
	for (size_t i = 0; i < indexResults->size(); i++)
	{
		size_t start		= indexResults->at(i);
		const vec2& v1		= points[indexes[start]];
		const vec2& v2		= points[indexes[start+1]];
		const vec2& v3		= points[indexes[start+2]];		

		if (m_renderData.getSearchPos().isPointInTriangle(v1, v2, v3))
		{
			// highlight selected triangle and remember position for later drawing
			m_startIdxTriangleHit	= j+1;
			m_hasTriangleHit			= true;
		}

		verticesResultVBO[++j].m_point	= v1;
		verticesResultVBO[++j].m_point	= v2;				
		verticesResultVBO[++j].m_point	= v3;
	}

	// bind and fill VBO data for result
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[RESULT]);
	glBufferData(GL_ARRAY_BUFFER, (m_numVerticesResult) * STRIDE_2V, 
						 verticesResultVBO, GL_STATIC_DRAW);

	// bind VAO data for later result rendering
	glBindVertexArray(m_vaoIds[RESULT]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);   

	// unbind VBO and VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] verticesResultVBO;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::createBaseMeshVBO()
{   
	const IntArray&	indexes				=	m_useLines ? m_renderData.getEdgeArray() :
														m_renderData.getMesh().getIndexArray();
	m_numIndexesMesh							=	indexes.size();
	const PointArray& points				=	m_renderData.getMesh().getPointArray();
	size_t				numVerticesMesh	=	points.size();
   size_t*				indexesVBO        =	new size_t[m_numIndexesMesh];
   Vertex_2V*			verticesMeshVBO	=	new Vertex_2V[numVerticesMesh];
	
   size_t i;
   for (i = 0; i < indexes.size(); i++)
   {
      indexesVBO[i] = indexes.at(i);
   }
   for (i = 0; i < points.size(); i++)
   {
      verticesMeshVBO[i].m_point	= points[i];		
   }
			
	// bind and fill VBO
   glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[BASEMESH]);
   glBufferData(GL_ARRAY_BUFFER, numVerticesMesh * STRIDE_2V, 
					verticesMeshVBO, GL_STATIC_DRAW);

	// bind VAO 	
	glBindVertexArray(m_vaoIds[BASEMESH]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);	
	
	// bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndexesMesh * sizeof(int), 
						indexesVBO, GL_STATIC_DRAW);

	
	// unbind VBO, VAO and IBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   delete[] indexesVBO;
   delete[] verticesMeshVBO;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void OpenGLRenderer::createAxesVBO()
{   
   const PointPtrArray* bbArray		= m_renderData.getBBArray();
	size_t size								= bbArray->size();
   m_numVerticesAxes						= size * 2;
   Vertex_2V*	verticesAxesVBO		= new Vertex_2V[m_numVerticesAxes];   
	size_t		j							= -1;

   for (unsigned i = 0; i < size; i+=2)
   {
      vec2* bb    = bbArray->at(i);
      vec2* pivot = bbArray->at(i+1);

      verticesAxesVBO[++j].m_point.x	= pivot->x;      
      verticesAxesVBO[j].m_point.y		= bb[0].y;      

      verticesAxesVBO[++j].m_point.x	= pivot->x;       
      verticesAxesVBO[j].m_point.y		= bb[1].y;      

      verticesAxesVBO[++j].m_point.x	= bb[0].x;      
      verticesAxesVBO[j].m_point.y		= pivot->y;      

      verticesAxesVBO[++j].m_point.x	= bb[1].x;      
      verticesAxesVBO[j].m_point.y		= pivot->y;      
   }
	
	// bind and fill VBO
   glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[AXES]);
   glBufferData(GL_ARRAY_BUFFER, m_numVerticesAxes * STRIDE_2V, 
      verticesAxesVBO, GL_STATIC_DRAW);

	// bind VAO data
	glBindVertexArray(m_vaoIds[AXES]);

	// set stride and offset for position
	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);
	glVertexAttribPointerExt(VERTEX_POS_ATTRIB, 2, GL_REAL,
										GL_FALSE, STRIDE_2V, 0);
	
	// unbind VBO and VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

   delete[] verticesAxesVBO;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::glVertexAttribPointerExt(GLuint index, GLint size, 
															 GLenum type, GLboolean normalized, 
															 GLsizei stride, const void *pointer)
{
#if defined DOUBLE_PRECISION
	glVertexAttribLPointer(index, size, type, stride, pointer);
#else
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#endif
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void OpenGLRenderer::drawAxes()
{	
	glLineWidth(2.0);
	glBindVertexArray(m_vaoIds[AXES]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorAxes.r);
	glDrawArrays(GL_LINES, 0, (GLsizei)m_numVerticesAxes);
   glLineWidth(m_defaultLineWidth);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::drawBaseMesh()
{   
	glBindVertexArray(m_vaoIds[BASEMESH]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorBasis.r);
	glDrawElements(m_useLines ? GL_LINES : GL_TRIANGLES, 
						(GLsizei)m_numIndexesMesh, GL_UNSIGNED_INT, 0);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void OpenGLRenderer::drawResult()
{      	
	glPolygonMode(m_defaultPolygonMode[0], GL_FILL);
	glBindVertexArray(m_vaoIds[RESULT]);

	// draw background
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorResultBackground.r);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_numVerticesResult);  
	 
	if (m_hasTriangleHit)
	{
		// draw hit triangle
		glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorTriangleHit.r);
		glDrawArrays(GL_TRIANGLES, (GLsizei)m_startIdxTriangleHit, 3);  
	}
	// draw wireframe
	glPolygonMode(m_defaultPolygonMode[0], m_defaultPolygonMode[1]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorResult.r);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_numVerticesResult);		
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::drawRegionAndPoint()
{	
	// draw regional rectangle
	glLineWidth(4.0);
	glBindVertexArray(m_vaoIds[REGION_SEARCHPOINT]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorRegion.r);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glLineWidth(m_defaultLineWidth);

	// draw search point
	glPointSize(6.0);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorSearchPoint.r);
	glDrawArrays(GL_POINTS, 4, 1);
	glPointSize(m_defaultPointSize);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::drawRegionsLeft()
{	
	if (m_numVerticesRegionsLeft == 0)
	{
		return;
	}
	// draw regions left
	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	glPolygonMode(m_defaultPolygonMode[0], GL_FILL);
	glBindVertexArray(m_vaoIds[REGIONS_LEFT]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorRegionLeft.r);
	glDrawArrays(GL_QUADS, 0, (GLsizei)m_numVerticesRegionsLeft);
	glPolygonMode(m_defaultPolygonMode[0], m_defaultPolygonMode[1]);
	glDisable(GL_BLEND);	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::drawBoundingBox()
{	
	// draw bounding box
	glLineWidth(2.0);
	glBindVertexArray(m_vaoIds[BOUNDINGBOX]);
	glVertexAttrib4v(VERTEX_COLOR_ATTRIB, &s_colorBB.r);
	glDrawArrays(GL_LINES, 0, 16);
	glLineWidth(m_defaultLineWidth);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool OpenGLRenderer::loadData(const TCHAR* filename)
{
	if (!m_renderData.loadData(filename))
	{
		return false;
	}
	m_renderData.computeBoundingBox();
	m_renderData.getMaxSceneEdge(m_lenMaxEdge);
	m_renderData.getSceneDiagonal(m_lenDiagonal);
	m_renderData.getSceneCenter(m_center);
	m_camera.updateClippingPlanes(m_lenDiagonal);
	if (useLinesForBaseMesh())
	{
		m_renderData.extractEdges();
		createBaseMeshVBO();
		m_renderData.cleanupEdgeArray();
	}
	else
	{
		createBaseMeshVBO();
	}
	createBoundingBoxVBO();
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::zoomToRegion()
{
	if (m_renderData.hasSearchResults())
	{
		vec2	center;
		REAL	maxDistance = 0;
		m_renderData.getCenterResultBB(center);
		m_renderData.getDiagonalResultBB(maxDistance);
		maxDistance *= 2;

		if (maxDistance > m_lenMaxEdge)
		{
			maxDistance = m_lenMaxEdge;
		}
		vec3 cameraPos;
		cameraPos.x = center.x;
		cameraPos.y = center.y;
		cameraPos.z = maxDistance;

		validateCameraPosition(cameraPos);
		// update modelview matrix with correct position
		updateCamera(cameraPos);
		drawScene();
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::resetView()
{
	vec3 camPos;
	camPos.x = m_center.x;
	camPos.y = m_center.y;
	camPos.z = m_lenMaxEdge;

	updateCamera(camPos);
	drawScene();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::updateSearchResults(const ResultData& results)
{
	m_renderData.setIndexResults(results.m_triangles);
	m_renderData.setBBResult(results.m_boundingBox);
	createResultVBO();
	createRegionVBO();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void OpenGLRenderer::prepareTreeDisplay(const PointPtrArray* boundingBoxes,
													 const PointPtrArray* regionsLeft)
{
	m_renderData.setBBArray(boundingBoxes);
	m_renderData.setRegionsLeft(regionsLeft);
	createAxesVBO();
	createRegionsLeftVBO();
}
