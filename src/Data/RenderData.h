/**
 * Class name:    RenderData

 * Description:   Keeps data necessary to render meshes by the renderer. 

 * Author:        Frank Dockhorn
 */



#ifndef _RENDERDATA
#define _RENDERDATA

#include "MeshData.h"

class RenderData
{
public:
	enum LoadErrorCode
	{
		NONE,
		INVALID_MESH_DATA,
		INVALID_FILE_FORMAT		
	};

   RenderData(): 
		m_indexResults		(NULL), 
		m_bbArray			(NULL),
		m_lineNumber		(0),
		m_loadErrorCode	(NONE)
   {}
   ~RenderData()
	{ 
	}

	void			extractEdges();
	void			computeBoundingBox();
	void			getSceneDiagonal		(REAL&);
	void			getMaxSceneEdge		(REAL&);
	void			getSceneCenter			(vec2&);
	void			getDiagonalResultBB	(REAL&);
	void			getMaxEdgeResultBB	(REAL&);	
	void			getCenterResultBB		(vec2&);
	bool			loadData					(const TCHAR* filename);
	bool			isPointinBoundingBox	(const vec2& searchPoint);
	bool			setSearchPos			(Vec2D& val);
   
	void cleanup()
   {
      m_mesh.cleanup();
      m_indexResults = NULL;
		m_bbArray		= NULL;
		m_regionsLeft	= NULL;
   }
	void resetResults()
	{
		m_indexResults = NULL;
		m_bbArray		= NULL;
		m_regionsLeft	= NULL;
	}
	void cleanupEdgeArray()
	{
		IntArray().swap(m_edgeArray);
	}
	bool hasSearchResults()
	{
		return m_indexResults != NULL;
	}
	// get functions
	const MeshData& getMesh() const
	{
		return m_mesh;
	}
	const IntArray& getEdgeArray() const
	{
		return m_edgeArray;
	}
	const IntArray* getIndexResults() const
	{
		return m_indexResults;
	}
	const PointPtrArray* getBBArray() const
	{
		return m_bbArray;
	}
	const PointPtrArray* getRegionsLeft() const
	{
		return m_regionsLeft;
	}
	const Vec2D& getSearchPos() const
	{
		return m_searchPos;
	}
	const vec2* getBBResult() const
	{
		return m_boundingBoxResult;
	}
	const vec2* getBoundingBox() const
	{
		return m_boundingBox;
	}
	int getLineNumber()
	{
		return m_lineNumber;
	}
	bool isInvalidFileFormat()
	{
		return m_loadErrorCode == INVALID_FILE_FORMAT;
	}
	// set functions
	void setIndexResults(const IntArray* val)
	{
		m_indexResults = val;
	}
	void setBBResult(const vec2(&val)[2])
	{
		m_boundingBoxResult[0] = val[0];
		m_boundingBoxResult[1] = val[1];
	}
	void setRegionsLeft(const PointPtrArray* val)
	{
		m_regionsLeft = val;
	}
	void setBBArray(const PointPtrArray* val)
	{
		m_bbArray = val;
	}

private:
	MeshData							m_mesh;
   IntArray							m_edgeArray;
   const IntArray*				m_indexResults;
	const PointPtrArray*       m_bbArray;
	const PointPtrArray*       m_regionsLeft;
	Vec2D								m_searchPos;
	vec2								m_boundingBoxResult[2];
	vec2								m_boundingBox[2];
	int								m_lineNumber;
	LoadErrorCode					m_loadErrorCode;
};

#endif