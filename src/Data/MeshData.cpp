
/**
 * Class name:    MeshData

 * Description:   Keeps data related to the mesh. 

 * Author:        Frank Dockhorn
 */


#include "stdafx.h"
#include "MeshData.h"

const char* MeshData::SCAN_PARAM[2] = {
	"%d %d %d", 
	"%d%*s %d%*s %d%*s"};

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool MeshData::checkData()
{
	if (m_points.empty())
	{
		return false;
	}
	else if (m_indexes.empty())
	{
		return false;
	}
	else if (m_indexes.size() % 3 > 0)
	{
		return false;
	}
	// determine min max indexes
	size_t maxIndex   = m_indexes[0];
	size_t minIndex   = m_indexes[0];
	size_t size       = m_indexes.size();
	size_t idx;

	for (size_t i = 1; i < size; i++)
	{
		idx = m_indexes[i];
		if (idx > maxIndex)
		{
			maxIndex = idx;
		}
		else if (idx < minIndex)
		{
			minIndex = idx;
		}
	}
	if (minIndex < 0 || maxIndex > (m_points.size() - 1))
	{
		return false;
	}
	return true;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool MeshData::filterData()
{
	// this function filters out backside triangles not needed by the demo
	// and compresses point and index arrays along the way.

	size_t* indexMap = (size_t*)malloc(m_points.size()*sizeof(size_t));
	if (indexMap == NULL)
	{
		return false;
	}
	bool*	seen = (bool*)malloc(m_points.size()*sizeof(bool));
	if (seen == NULL)
	{
		return false;
	}
	memset(indexMap, 0, m_points.size()*sizeof(size_t));
	memset(seen, false, m_points.size()*sizeof(bool));

	PointArray newPoints;
	IntArray	  newIndexes;	

	size_t numTris = m_indexes.size();	
	
	for (size_t i = 0; i < numTris; i+=3)
	{
		size_t	start	= i;
		size_t	idx1	= m_indexes[start++];
		size_t	idx2	= m_indexes[start++];
		size_t	idx3	= m_indexes[start];
		vec2&		pt1	= m_points[idx1];
		vec2&		pt2	= m_points[idx2];
		vec2&		pt3	= m_points[idx3];
		vec2&		diff1 = pt2-pt1;
		vec2&		diff2 = pt3-pt1;

		REAL zCoord = cross(diff1, diff2);
		// check for triangle orientation
		if (zCoord > 0)
		{			
			if (!seen[idx1])
			{
				seen[idx1]		= true;
				indexMap[idx1] = newPoints.size();
				newPoints.push_back(pt1);
			}
			if (!seen[idx2])
			{
				seen[idx2]		= true;
				indexMap[idx2] = newPoints.size();
				newPoints.push_back(pt2);				
			}
			if (!seen[idx3])
			{
				seen[idx3]		= true;
				indexMap[idx3] = newPoints.size();
				newPoints.push_back(pt3);
			}
			newIndexes.push_back(indexMap[idx1]);
			newIndexes.push_back(indexMap[idx2]);
			newIndexes.push_back(indexMap[idx3]);
		}
	}
	m_points		= newPoints;
	m_indexes	= newIndexes;
	
	free(indexMap);
	free(seen);
	return !m_points.empty() && !m_indexes.empty();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool MeshData::parseVertexValues(const char* buf)
{
	Vec2D	point;
	int	res = sscanf_s(buf, PREC_SCAN, &point.x, &point.y);
	if (res < 2)
	{
		return false;
	}
	point.roundValues(THREE);
	addPoint(point);
	return true;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool MeshData::parseFaceValues(const char* inputBuf)
{	
	int index[3];
	int res	= 0;
	int res2 = 0;
	if (m_scanToUse == NULL)
	{
		res = sscanf_s(inputBuf, SCAN_PARAM[0], &index[0], &index[1], &index[2]);		
		if (res < 3)
		{
			res = sscanf_s(inputBuf, SCAN_PARAM[1], &index[0], &index[1], &index[2]);
			if (res < 3)
			{
				return false;
			}
			m_scanToUse = SCAN_PARAM[1];
		}
		else
		{
			m_scanToUse = SCAN_PARAM[0];
		}
	}
	else
	{
		res = sscanf_s(inputBuf, m_scanToUse, &index[0], &index[1], &index[2]);
	}
	if (res == 3)
	{	
		index[0] -= 1;
		index[1] -= 1;
		index[2] -= 1;
		addIndex(index[0]);
		addIndex(index[1]);
		addIndex(index[2]);
		return true;
	}
	return false;
}