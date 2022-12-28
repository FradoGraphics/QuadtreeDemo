/**
 * Class name:    QuadTreeData

 * Description:   Keeps necessary data for creating the quad tree.

 * Author:        Frank Dockhorn
 */


/*-------------------------------------------------------------------------*\
   Class QuadTreeData member description:
   m_points            -> an array holding the triangle vertices 
   m_bb                -> the 2-dimensional bounding box based on the triangle vertices
   m_triangleIndexes   -> integer array keeping the triangle indices 
\*-------------------------------------------------------------------------*/


#ifndef _QUADTREEDATA
#define _QUADTREEDATA

#include "MeshData.h"

class QuadTreeData
{
public:
   QuadTreeData()
   :  m_maxDepth			(DEPTH_DEFAULT_VALUE), 
      m_sizeBound			(NUMTRI_DEFAULT_VALUE),
		m_ratio				(RATIO_DEFAULT_VALUE),		
		m_enableSizeBound	(true),
      m_bb					(NULL)
   {
   }

	void validateData				();
	void resetInputData			();
	bool readSettingsFromFile	(const TCHAR* userdatPath);
	bool writeSettingsToFile	(const TCHAR* userdatPath);

	// default values
	static const float	RATIO_DEFAULT_VALUE;
	static const int		DEPTH_DEFAULT_VALUE;
	static const int		NUMTRI_DEFAULT_VALUE;

	// upper and lower bounds
	static const float		LOWER_RATIO_BOUND;
	static const float		UPPER_RATIO_BOUND;
	static const int			LOWER_DEPTH_BOUND;
	static const int			UPPER_DEPTH_BOUND;
	static const unsigned	LOWER_NUMTRI_BOUND;
	static const unsigned	UPPER_NUMTRI_BOUND;
	static const TCHAR*		QUAD_CHAPTER;

	// query states
	bool isSizeBoundEnabled() const
	{
		return m_enableSizeBound;
	}

	// get functions
	const vec2* getBoundingBox() const
	{
		return m_bb;
	}
	const MeshData* getMesh() const
	{
		return m_mesh;
	}
	int getMaxDepth() const
	{
		return m_maxDepth;
	}
	unsigned getSizeBound() const
	{
		return m_sizeBound;
	}
	REAL getRatio() const
	{
		return m_ratio;
	}

	// set functions
	void setMaxDepth(int val) 
	{
		m_maxDepth = val;
	}
	void setSizeBound(unsigned val)
	{
		m_sizeBound = val;
	}
	void setRatio(REAL val)
	{
		m_ratio = val;
	}
	void setSizeBoundState(bool val)
	{
		m_enableSizeBound = val;
	}
	void setBoundingBox(const vec2* val) 
	{
		m_bb = val;
	}
	void setMesh(const MeshData* val)
	{
		m_mesh = val;
	}

private:
	const vec2*				m_bb;
	const MeshData*		m_mesh;
	int						m_maxDepth; 
	unsigned					m_sizeBound;
	REAL						m_ratio;	
	bool						m_enableSizeBound;
};

#endif
