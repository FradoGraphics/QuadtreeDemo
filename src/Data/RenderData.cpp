
/**
 * Class name:    RenderData

 * Description:   Keeps data necessary to render meshes by the renderer. 

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "RenderData.h"


bool RenderData::loadData(const TCHAR* filename)
{
	MeshData mesh;
	FILE*    file	= NULL; 
	errno_t  res	= _tfopen_s(&file, filename, _T("r"));
	if (res != 0)
	{
		return false;
	}
	
	m_lineNumber		= 0;
	m_loadErrorCode	= NONE;
	bool	hasError		= false;	
	bool	eof			= false;
	bool	resVal		= true;
	bool	resFace		= true;
	char*	valueBuf		= NULL;
	char	typeBuf[MAX_PATH];	
	char	lineBuf[MAX_PATH];

	do 
	{
		m_lineNumber++;
		char* str = fgets(lineBuf, MAX_PATH, file);
		
		if (feof(file))
		{
			eof = true;
			continue;
		}
		if (ferror(file) || str == NULL)
		{
			hasError = true;
			continue;
		}
		if (strcmp(str, "\n") == 0)
		{
			// skip empty line
			continue;
		}
		int res2 = sscanf_s(lineBuf, "%s", typeBuf, (unsigned)_countof(typeBuf));
		if (res2 < 1)
		{
			hasError = true;
			continue;
		}
		if (strcmp(typeBuf, "f") == 0)
		{
			// process face
			valueBuf = lineBuf+1;
			resFace	= mesh.parseFaceValues(valueBuf);
		}
		else if (strcmp(typeBuf, "v") == 0)
		{
			// process vertex
			valueBuf = lineBuf+1;
			resVal	= mesh.parseVertexValues(valueBuf);
		}
		if (!resFace || !resVal)
		{
			hasError = true;			
		}
	} 
	while (!eof && !hasError);
	fclose(file);

	if (hasError)
	{
		m_loadErrorCode = INVALID_FILE_FORMAT;
		return false;
	}
	else if (!mesh.checkData())
	{
		m_loadErrorCode = INVALID_MESH_DATA;
		return false;
	}
	if (!mesh.filterData())
	{
		return false;
	}
	cleanup();
	m_mesh = mesh;
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool RenderData::setSearchPos(Vec2D& val)
{
	if (isPointinBoundingBox(val))
	{
		m_searchPos = val;
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool RenderData::isPointinBoundingBox(const vec2& searchPoint)
{
	if (searchPoint.x >= m_boundingBox[0].x && 
		searchPoint.x <= m_boundingBox[1].x &&
		searchPoint.y >= m_boundingBox[0].y && 
		searchPoint.y <= m_boundingBox[1].y)
	{
		return true;
	}
	return false;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::computeBoundingBox()
{   
	const PointArray& points	= m_mesh.getPointArray();
	vec2					vecInit	= points.at(0);
	size_t				size		= points.size();
	m_boundingBox[0]				= m_boundingBox[1] = vecInit;

	for (unsigned int i = 1; i < size; i++)
	{
		const vec2& vec = points.at(i);
		if (vec.x < m_boundingBox[0].x)
		{
			m_boundingBox[0].x = vec.x;
		}
		else if (vec.x > m_boundingBox[1].x)
		{
			m_boundingBox[1].x = vec.x;
		}

		if (vec.y < m_boundingBox[0].y)
		{
			m_boundingBox[0].y = vec.y;
		}
		else if (vec.y > m_boundingBox[1].y)
		{
			m_boundingBox[1].y = vec.y;
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getSceneDiagonal(REAL& diagonal)
{
	REAL x2	= m_boundingBox[0].x - m_boundingBox[1].x;
	REAL y2	= m_boundingBox[0].y - m_boundingBox[1].y;
	diagonal	= sqrt(x2*x2 + y2*y2);   
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getSceneCenter(vec2& center)
{	
	REAL x		= m_boundingBox[0].x + m_boundingBox[1].x;
	REAL y		= m_boundingBox[0].y + m_boundingBox[1].y;
	center.x		= x / 2; 
	center.y		= y / 2;	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getMaxSceneEdge(REAL& maxEdge)
{		
	REAL edge1	= m_boundingBox[1].x - m_boundingBox[0].x;
	REAL edge2	= m_boundingBox[1].y - m_boundingBox[0].y;
	maxEdge		= edge2 > edge1 ? edge2 : edge1;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getCenterResultBB(vec2& center)
{	
	REAL x		= m_boundingBoxResult[0].x + m_boundingBoxResult[1].x;
	REAL y		= m_boundingBoxResult[0].y + m_boundingBoxResult[1].y;
	center.x		= x / 2; 
	center.y		= y / 2;	
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getMaxEdgeResultBB(REAL& maxEdge)
{		
	REAL edge1	= m_boundingBoxResult[1].x - m_boundingBoxResult[0].x;
	REAL edge2	= m_boundingBoxResult[1].y - m_boundingBoxResult[0].y;
	maxEdge		= edge2 > edge1 ? edge2 : edge1;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::getDiagonalResultBB(REAL& diagonal)
{
	REAL x2	= m_boundingBoxResult[0].x - m_boundingBoxResult[1].x;
	REAL y2	= m_boundingBoxResult[0].y - m_boundingBoxResult[1].y;
	diagonal	= sqrt(x2*x2 + y2*y2);   
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void RenderData::extractEdges()
{
	std::unordered_map<string, int>	edgeMap;

	char							key1[UCHAR_MAX], key2[UCHAR_MAX];
	size_t						idx1, idx2;
	bool							res;
	char*							format	= "%d_%d";
	const IntArray&			indexes	= m_mesh.getIndexArray();

	size_t size = indexes.size();
	// Iterate over all triangle indexes and find all unique edges.
	// These edges are used for later OpenGL line rendering.
	for (size_t i = 0; i < size; i+=3)
	{     		
		for (size_t k = 2, j = 0; j < 3; k = j++)
		{
			idx1  = indexes.at(i+k);
			idx2  = indexes.at(i+j);      
			sprintf_s(key1, UCHAR_MAX, format, idx1, idx2);
			res   =(edgeMap.find(key1) == edgeMap.end()) ? false : true;			
			if (!res)
			{
				// not in map, so we add this edge to the map
				// in reverse index order. This ensures that only one edge between two
				// neighboring triangles will be added into the index array.
				sprintf_s(key2, UCHAR_MAX, format, idx2, idx1);
				edgeMap[key2] = 1;
				m_edgeArray.push_back(idx1);
				m_edgeArray.push_back(idx2);
			}
		}
	}
#if defined _DEBUG
	size_t numEdges		= m_edgeArray.size() / 2;
	size_t numTriangles	= size / 3;	
	size_t upperLimit		= 2 * numTriangles + 1;
	// NOTE: 
	// upperLimit expresses the number of unique edges
	// if and only if two new edges are added to an initial triangle, 
	// resulting in a triangle mesh. 
	//
	// upperLimit = 3 + (n-1) * 2. N = number of triangles.
	_ASSERT(numEdges <= upperLimit);
#endif
}