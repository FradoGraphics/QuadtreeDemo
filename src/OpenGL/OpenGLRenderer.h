/**
 * Class name:    OpenGLRenderer

 * Description:   Uses OpenGLs Vertex Buffer Objects to render the mesh, 
                  results and regions.

 * Author:        Frank Dockhorn
 */


#ifndef _OPENGL_RENDERER
#define _OPENGL_RENDERER

#include "RenderData.h"
#include "OpenGLCamera.h"
#include "ResultData.h"

class OpenGLProgram;
class OpenGLShader;

class OpenGLRenderer
{
public:

	enum VBO_VAO_IDS
	{
		BASEMESH,
		AXES,
		RESULT,
		REGION_SEARCHPOINT,
		BOUNDINGBOX,
		REGIONS_LEFT,
		MAX_IDS
	};

   OpenGLRenderer			();
   ~OpenGLRenderer		();

	const vec2*				getBoundingBox					();
	const MeshData&		getMesh							();
	const Vec2D&			getSearchPos					();
	vec3						getCameraPos					();
	void						setSearchPos					(Vec2D& vec2);
	void						updateSearchResults			(const ResultData& results);
	void						drawScene						();
	void						zoomToRegion					();
	void						resetView						();
	void						initRenderStates				();
   void						createAxesVBO					();
	void						createBaseMeshVBO				();
   void						createResultVBO				();
	void						createRegionVBO				();
	void						createRegionsLeftVBO			();
	void						createBoundingBoxVBO			();
	void						clearBuffer						();   
	void						cleanup							();
	void						cleanupVBOs						();
	void						resetResults					();
	void						setLinesForBaseMesh			(bool val = true) {m_useLines = val;}
	void						setPerspective					(LONG x, LONG y);
	void						updateCamera					(const vec3& cameraPos);
	void						prepareTreeDisplay			(	const PointPtrArray* boundingBoxes,
																		const PointPtrArray* regionsLeft);

	bool						validateCameraPosition		(vec3& cameraPos);
	bool						computeCursorPos				(int x, int y, vec2& cursorPos);
   bool						buildShaderProgram			();
	bool						checkOpenGLVersion				();
	bool						useLinesForBaseMesh			() {return m_useLines;}
	bool						loadData							(const TCHAR* filename);
	bool						writeSettingsToFile			(const TCHAR* filename);
	bool						readSettingsFromFile			(const TCHAR* filename);
	bool						hasSearchResults				();
	const char*				getInfoLog						();
	const char*				buildErrorMessage				();
	
	// inline member functions

	const char* getVersion()
	{
		return m_verString;
	}
	int getLineNumber()
	{
		return m_renderData.getLineNumber();
	}
	bool isInvalidFileFormat()
	{
		return m_renderData.isInvalidFileFormat();
	}
	bool hasSearchPos()
	{
		return m_hasSearchPos;
	}
	void resetSearchPos()
	{
		m_hasSearchPos = false;
	}
	const RenderData* getRenderData()
	{
		return &m_renderData;
	}
	bool isValidOpenGLVersion()
	{
		return m_isValidOpenGLVersion;
	}
protected:
	void						glVertexAttribPointerExt	(GLuint index, GLint size, 
																	GLenum type, GLboolean normalized, 
																	GLsizei stride, const void *pointer);
   void						drawResult						();
	void						drawRegionAndPoint			();
	void						drawBoundingBox				();
	void						drawRegionsLeft				();
   void						drawBaseMesh					();
   void						drawAxes							();
private:
	REAL						m_lenMaxEdge;
	REAL						m_lenDiagonal;
	GLfloat					m_defaultLineWidth;
	GLfloat					m_defaultPointSize;
	GLint						m_defaultPolygonMode[2];
	GLint						m_OpenGLVersion[2];
	const char*				m_verString;
	vec2						m_center;

	GLuint					m_vboIds[MAX_IDS];
	GLuint					m_vaoIds[MAX_IDS];
   GLuint               m_indexBufferID;
	size_t					m_startIdxTriangleHit;
	size_t               m_numVerticesAxes;
   size_t               m_numVerticesResult;
	size_t               m_numVerticesRegionsLeft;
   size_t               m_numIndexesMesh;
   bool                 m_isValidOpenGLVersion;
	bool						m_useLines;
	bool						m_hasTriangleHit;
	bool						m_hasSearchPos;
	
	OpenGLProgram*			m_program;
	OpenGLCamera			m_camera;
	RenderData           m_renderData;
	astring					m_errorMessage;

	static vec4				s_colorResult;
	static vec4				s_colorResultBackground;
	static vec4				s_colorRegion;
	static vec4				s_colorRegionLeft;
	static vec4				s_colorBB;
	static vec4				s_colorTriangleHit;
	static vec4				s_colorAxes;
	static vec4				s_colorBasis;	
	static vec4				s_colorSearchPoint;
	static vec4				s_clearColor;

	static const TCHAR*	SEARCH_CHAPTER;
	static const TCHAR*	CAMERA_CHAPTER;

	struct Vertex_2V3C
	{
		vec2	m_point;
		vec3	m_color;
	};

	struct Vertex_2V6C
	{
		vec2	m_point;
		vec3	m_color;
		vec3	m_backgroundColor;
	};

	struct Vertex_2V
	{
		vec2	m_point;
	};


	enum
	{
		STRIDE_2V	= sizeof(Vertex_2V),		
		STRIDE_2V3C = sizeof(Vertex_2V3C),
		OFFSET_2V3C = sizeof(vec2),
		STRIDE_2V6C = sizeof(Vertex_2V6C),
		OFFSET_2V6C = sizeof(vec2) + sizeof(vec3)
	};
};


#endif