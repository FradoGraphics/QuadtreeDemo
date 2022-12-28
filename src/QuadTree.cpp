/**
 * Class name:    QuadTree

 * Description:   Builds the tree depending on threshold values. Each node of the tree 
                  represents a region. Each region in turn stores the indices of triangles
                  located in this region. Regions are bounded by axis-aligned bounding boxes.
                  Triangles of a region can be searched for by performing a search with (x,y)
                  coordinates.

 * Author:        Frank Dockhorn
 */

#include "stdafx.h"
#include "Vector.h"
#include "QuadTree.h" 
#include "QuadTreeData.h"
#include "ResultData.h"
#include "trace.h"
#include <time.h>

// *** For diagnostics ***
//#define _MEASURE_TIME
// #define _DEBUG_QUADTREE
// #define _SHOW_TRIANGLES
// #define _SHOW_SEARCH_RESULTS


#if !defined _DEBUG && !defined _DEBUG_RELEASE && (defined _MEASURE_TIME \
|| defined _DEBUG_QUADTREE \
|| defined _SHOW_TRIANGLES \
|| defined _SHOW_SEARCH_RESULTS)
#error Debugging macros are NOT allowed in release mode
#endif

const int      QuadTree::BITVECTOR[] = { 0x1, 0x2, 0x4, 0x8 };
const REAL     QuadTree::D_EPSILON_2 = 1e-6;

#define TRIANGLE_TEMPLATE  _T("TRIANGLE %d: (%6.3f, %6.3f) | (%6.3f, %6.3f) | (%6.3f, %6.3f) STARTINDEX: %d\n")
#define BOUNDING_BOX       _T("BOUNDING BOX: (%6.3f, %6.3f) | (%6.3f, %6.3f)\n")
#define CURRENT_DEPTH      _T("CURRENT DEPTH: %d\n")
#define NUM_TRIANGLES      _T("NUMBER TRIANGLES: %d\n")



QuadTree::QuadTree(const QuadTreeData* data)
:	m_root						(NULL),
	m_triangleIndexes			(NULL),
	m_points						(NULL),
	m_realMaxDepth				(0),
	m_maxDepth					(0),
	m_sizeBound					(0),
	m_totalTris					(0),
	m_deviationValue			(0),
	m_deviationMedian			(0),
	m_deviationPercent		(0.0),
	m_deviationMedianPercent(0.0),
	m_ratio(0.0)
{ 
   createTree(data);
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadTree::QuadTree() 
:	m_root						(NULL),
	m_triangleIndexes			(NULL),
	m_points						(NULL),
	m_realMaxDepth				(0),
	m_maxDepth					(0),
	m_sizeBound					(0),
	m_totalTris					(0),
	m_deviationValue			(0),
	m_deviationMedian			(0),
	m_deviationPercent		(0.0),
	m_deviationMedianPercent(0.0),
	m_ratio(0.0)
{
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadTree::~QuadTree()
{
   cleanup();
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::cleanup()
{
   m_BBArray.resize(0);
	m_regionsLeft.resize(0);
	m_trianglesLeft.resize(0);
   delete m_root;
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::createTree(const QuadTreeData* data)
{
   int currentDepth				= 0;
	m_realMaxDepth					= 0;	
	m_deviationValue				= 0;
	m_deviationMedian				= 0;
	m_deviationPercent			= 0.0;
	m_deviationMedianPercent	= 0.0;
	m_maxDepth						= data->getMaxDepth();
   m_sizeBound						= data->getSizeBound();
	m_ratio							= data->getRatio();
   m_points							= &data->getMesh()->getPointArray();
   m_triangleIndexes				= &data->getMesh()->getIndexArray();
	m_totalTris						= m_triangleIndexes->size() / 3;
   m_root							= new QuadNode(data->getBoundingBox(), this, true);   

	if (!data->isSizeBoundEnabled())
	{
		// assign new bound in case the percentage ratio has been chosen		
		m_sizeBound	= m_totalTris * m_ratio / 100.0;
	}
	if (m_sizeBound < 1)
	{
		m_sizeBound = 1;
	}

#if defined _MEASURE_TIME
   clock_t start = clock(), diff;
#endif
	// check conditions for region subdivision
	if (m_root->getNumTris() > m_sizeBound && currentDepth < m_maxDepth)
	{
		m_root->subdivideRegion(++currentDepth, m_realMaxDepth);
	}

	computeStatistics(data->isSizeBoundEnabled());
#if defined _MEASURE_TIME
   diff = clock() - start;
   int msec = diff * 1000 / CLOCKS_PER_SEC;
   TRACE(_T("\n\n***Time measured for creating quad tree: %d.%d seconds***\n\n"), msec/1000, msec%1000);
#endif
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::computeStatistics(bool isSizeBound)
{
	if (m_trianglesLeft.size() == 0)
	{
		return;
	}

	// compute final cumulative deviation value
	size_t size = m_trianglesLeft.size();
	for (size_t i = 0; i < size; i++)
	{
		size_t numTriangles	=	m_trianglesLeft.at(i);	
		m_deviationValue		+= numTriangles - m_sizeBound;
	}
	
	m_deviationPercent = ((REAL)(m_deviationValue)) / m_totalTris * 100.0;

	// compute median of deviation
	for (size_t k = 1; k < size; k++)
	{
		size_t value = m_trianglesLeft.at(k);	
		size_t j = k;

		while (j > 0 && m_trianglesLeft.at(j-1) > value)
		{
			m_trianglesLeft.at(j) = m_trianglesLeft.at(j-1);
			j--;
		}
		m_trianglesLeft.at(j) = value;
	}
	
	REAL median;
	if ((size % 2) > 0)
	{
		// size is an odd number
		median = m_trianglesLeft.at(size/2);
	}
	else
	{
		// size is an even number
		median = (m_trianglesLeft.at(size/2) + m_trianglesLeft.at((size-1)/2));
		median /= 2;
	}

	m_deviationMedian				= median - m_sizeBound;
	m_deviationMedianPercent	= ((REAL)(m_deviationMedian)) / m_totalTris * 100.0;
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadTree::findTriangles(const vec2& searchPoint, 
												ResultData& resultSet)
{
   if (m_root->isPointinBoundingBox(searchPoint))
	{	
		m_root->findTriangles(searchPoint, resultSet.m_depth, resultSet);   	
	}	
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

QuadTree::QuadNode::QuadNode(const vec2* bb, QuadTree* tree, bool init)
{
   m_pivot.x        = (bb[1].x + bb[0].x) / 2; 
   m_pivot.y        = (bb[1].y + bb[0].y) / 2; 
   m_boundingBox[0]  = bb[0];
   m_boundingBox[1]  = bb[1];
   m_tree            = tree;
	m_bitRegions		= 0;

   m_region[0] = NULL; 
   m_region[1] = NULL;
   m_region[2] = NULL;
   m_region[3] = NULL;
   
   if (init)
   {
      size_t size =  tree->m_triangleIndexes->size();       
      for (unsigned int i = 0; i < size; i+=3)
      {         
         m_tries.push_back(i);
      }
#if defined _DEBUG_QUADTREE
      traceQuadtree(0);
#endif
   }      
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


QuadTree::QuadNode::~QuadNode()
{
   for (int i = 0; i < 4; i++)
   {
      delete m_region[i]; 
   }
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

bool QuadTree::QuadNode::isPointinBoundingBox(const vec2& searchPoint)
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


void QuadTree::QuadNode::findTriangles(const vec2& searchPoint, char& depth, 
															 ResultData& resultSet)
{
   int region = findRegion(searchPoint);
   if (!m_region[region])
   {        
#if defined _SHOW_SEARCH_RESULTS
      traceSearchResults(searchPoint);
#endif
		resultSet.m_boundingBox[0] = m_boundingBox[0];
		resultSet.m_boundingBox[1] = m_boundingBox[1];
      resultSet.m_triangles		= &m_tries;
		return;
   }
   m_region[region]->findTriangles(searchPoint, ++depth, resultSet);
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


// the four sub regions of a region
//+--------------+b[1]
//|  nw  |  ne   |
//|------+pivot--|
//|  sw  |  se   |
//+b[0]----------+


void QuadTree::QuadNode::subdivideRegion(int currentDepth, int& realMaxDepth)
{
   vec2 boundingBox[4][2];
   size_t startIdx;   

	if (realMaxDepth < currentDepth)
	{
		realMaxDepth = currentDepth;
	}

   // 1. region south west
   boundingBox[SW][0].x = m_boundingBox[0].x;
   boundingBox[SW][0].y = m_boundingBox[0].y;
   boundingBox[SW][1].x = m_pivot.x;
   boundingBox[SW][1].y = m_pivot.y;

   // 2. region south east
   boundingBox[SE][0].x = m_pivot.x;
   boundingBox[SE][0].y = m_boundingBox[0].y;
   boundingBox[SE][1].x = m_boundingBox[1].x;
   boundingBox[SE][1].y = m_pivot.y;

   // 3. region north west
   boundingBox[NW][0].x = m_boundingBox[0].x;
   boundingBox[NW][0].y = m_pivot.y;
   boundingBox[NW][1].x = m_pivot.x;
   boundingBox[NW][1].y = m_boundingBox[1].y;

   // 4. region north east
   boundingBox[NE][0].x = m_pivot.x;
   boundingBox[NE][0].y = m_pivot.y;
   boundingBox[NE][1].x = m_boundingBox[1].x;
   boundingBox[NE][1].y = m_boundingBox[1].y;   

   size_t i;
   for (i = 0; i < 4; i++)
   {
      m_region[i] = new QuadNode(boundingBox[i], m_tree); 
   }

   // find at least one region for each triangle 
   size_t	size = m_tries.size();
	vec2 triangle[3];

   for (i = 0; i < size; i++)
   {      
      startIdx    = m_tries[i];      
      triangle[0] = m_tree->m_points->at(m_tree->m_triangleIndexes->at(startIdx));
      triangle[1] = m_tree->m_points->at(m_tree->m_triangleIndexes->at(startIdx + 1));
      triangle[2] = m_tree->m_points->at(m_tree->m_triangleIndexes->at(startIdx + 2));
      addTriangleToRegion(triangle, startIdx);
   }
	m_tries.clear();

	// save the dimension of the parent region for OpenGL display purposes
   m_tree->m_BBArray.push_back(m_boundingBox);
   m_tree->m_BBArray.push_back(&m_pivot);

   for (i = 0; i < 4; i++)
   {
#if defined _DEBUG_QUADTREE
		m_region[i]->traceQuadtree(currentDepth);
#endif
		// check conditions for region subdivision
      if (m_region[i]->m_tries.size() > m_tree->m_sizeBound)
      {
			if (currentDepth < m_tree->m_maxDepth)
			{
				m_region[i]->subdivideRegion(currentDepth+1, realMaxDepth);
			}
			else
			{
				// update number of non-conforming triangles and regions
				m_tree->m_trianglesLeft.push_back(m_region[i]->m_tries.size());
				m_tree->m_regionsLeft.push_back(m_region[i]->m_boundingBox);				
			}			
      }
   }
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadTree::QuadNode::addTriangleToRegion(vec2* triangle, size_t startIdx)
{
   m_bitRegions = 0;
	if (m_pivot.isPointInTriangle(triangle[0], triangle[1], triangle[2]))
	{
		addToRegion(startIdx, SW);
		addToRegion(startIdx, SE);
		addToRegion(startIdx, NW);
		addToRegion(startIdx, NE);
		return;
	}
	
	findTriangleIntersections(triangle, startIdx);

	if (!m_bitRegions)
	{
		// we didn't find any intersection so we add this triangle to a point's region		
		int region = findRegion(triangle[0]);
		m_region[region]->m_tries.push_back(startIdx);		
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::QuadNode::findTriangleIntersections(vec2* triangle, size_t startIdx)
{	

	// PLEASE NOTE:   Handling of component comparison is tightly associated with the implementation 
	//                of the findRegion() function. That means when the point to be compared equals 
	//                the pivot point the triangle must be put at least into region 2.
	// Linear equations are in parametric form.
	//                m_pivot.dx = triangle[0].dx + t * (triangle[1].dx - triangle[0].dx)
	//                m_pivot.dy = triangle[0].dy + t * (triangle[1].dy - triangle[0].dy)
	
	int k				= 2;	   

	vec2 direction;
	// Iterate through all triangle laterals and find bounding box intersections
	for (int i = 0; i < 3; k = i++)
	{         
		direction = triangle[i] - triangle[k];     

		if (direction.x)
		{      
			findIntersectionsWithX(direction, triangle, startIdx, k);
		}		
		if (direction.y) 
		{      
			findIntersectionsWithY(direction, triangle, startIdx, k);
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::QuadNode::findIntersectionsWithX(vec2&		direction, 
																vec2*		triangle, 
																size_t	startIdx,
																int		k)
{
	// find intersection with plane x = m_pivot.x
	REAL t = (m_pivot.x - triangle[k].x) / (direction.x);

	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL yComponent = triangle[k].y + t * (direction.y);

		if (yComponent < m_pivot.y)
		{
			if (yComponent >= m_boundingBox[0].y)
			{					
				addToRegion(startIdx, SW);
				addToRegion(startIdx, SE);
			}
		}
		else if (yComponent <= m_boundingBox[1].y)
		{
			addToRegion(startIdx, NW);
			addToRegion(startIdx, NE);
		}
	}
	// find intersection with plane x = m_boundingBox[0].x
	t = (m_boundingBox[0].x - triangle[k].x) / (direction.x);
	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL yComponent = triangle[k].y + t * (direction.y);

		if (yComponent < m_pivot.y)
		{
			if (yComponent >= m_boundingBox[0].y)
			{					
				addToRegion(startIdx, SW);
			}
		}
		else if (yComponent <= m_boundingBox[1].y)
		{
			addToRegion(startIdx, NW);
		}
	}
	// find intersection with plane x = m_boundingBox[1].x
	t = (m_boundingBox[1].x - triangle[k].x) / (direction.x);
	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL yComponent = triangle[k].y + t * (direction.y);

		if (yComponent < m_pivot.y)
		{
			if (yComponent >= m_boundingBox[0].y)
			{					
				addToRegion(startIdx, SE);
			}
		}
		else if (yComponent <= m_boundingBox[1].y)
		{
			addToRegion(startIdx, NE);
		}
	}
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/

void QuadTree::QuadNode::findIntersectionsWithY(vec2&		direction, 
															   vec2*		triangle, 
																size_t	startIdx,
																int		k)
{
	// find intersection with plane y = m_pivot.y
	REAL t = (m_pivot.y - triangle[k].y) / (direction.y);
	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL xComponent = triangle[k].x + t * (direction.x);

		if (xComponent > m_pivot.x)
		{
			if (xComponent <= m_boundingBox[1].x)
			{
				addToRegion(startIdx, SE);
				addToRegion(startIdx, NE);
			}
		}
		else if (xComponent >= m_boundingBox[0].x)
		{
			addToRegion(startIdx, SW);
			addToRegion(startIdx, NW);
		}
	}
	// find intersection with plane y = m_boundingBox[0].y
	t = (m_boundingBox[0].y - triangle[k].y) / (direction.y);
	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL xComponent = triangle[k].x + t * (direction.x);

		if (xComponent > m_pivot.x)
		{
			if (xComponent <= m_boundingBox[1].x)
			{
				addToRegion(startIdx, SE);				
			}
		}
		else if (xComponent >= m_boundingBox[0].x)
		{
			addToRegion(startIdx, SW);			
		}
	}
	// find intersection with plane y = m_boundingBox[1].y
	t = (m_boundingBox[1].y - triangle[k].y) / (direction.y);
	if (t < (1 + D_EPSILON_2) && t > -D_EPSILON_2)
	{
		// we have an intersection
		REAL xComponent = triangle[k].x + t * (direction.x);

		if (xComponent > m_pivot.x)
		{
			if (xComponent <= m_boundingBox[1].x)
			{
				addToRegion(startIdx, NE);				
			}
		}
		else if (xComponent >= m_boundingBox[0][0])
		{
			addToRegion(startIdx, NW);			
		}
	}
}

/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


inline int QuadTree::QuadNode::findRegion(const vec2& point)
{
   int base = 2;
   if (point.y < m_pivot.y)
   {
      base = 0;
   }
   if (point.x > m_pivot.x)
   {
      base++;
   }
   return base;
}



/*-------------------------------------------------------------------------*\
   Functions for debugging the quad tree
\*-------------------------------------------------------------------------*/

void QuadTree::QuadNode::traceSearchResults(const vec2& searchPoint)
{
   vec2 triangle[3];   
   TRACE(_T("\nRESULTS FOR SEARCH POINT: (%6.3f, %6.3f)\n"), searchPoint.x, searchPoint.y);
   TRACE(BOUNDING_BOX, m_boundingBox[0].x, m_boundingBox[0].y, \
      m_boundingBox[1].x, m_boundingBox[1].y);

   for (unsigned int i = 0; i < m_tries.size(); i++)
   {				
      size_t startIdx   = m_tries.at(i);
      triangle[0]    = m_tree->m_points->at(m_tree->m_triangleIndexes->at(startIdx));
      triangle[1]    = m_tree->m_points->at(m_tree->m_triangleIndexes->at(++startIdx));
      triangle[2]    = m_tree->m_points->at(m_tree->m_triangleIndexes->at(++startIdx));	
      TRACE(TRIANGLE_TEMPLATE, i, triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, \
         triangle[2].x, triangle[2].y, startIdx);      
   }
}


/*-------------------------------------------------------------------------*\
\*-------------------------------------------------------------------------*/


void QuadTree::QuadNode::traceQuadtree(int currentDepth)
{
   tstring offset = _T("");
   for (int z = 0; z < currentDepth; z++)
   {
      offset += _T("                 ");
   }
   tstring text = offset + _T("NUMBER TRIANGLES: %d\n");
   TRACE(text.data(), m_tries.size());
   text = offset + CURRENT_DEPTH;
   TRACE(text.data(), currentDepth);
   text = offset + BOUNDING_BOX;
   TRACE(text.data(), m_boundingBox[0].x, m_boundingBox[0].y, \
            m_boundingBox[1].x, m_boundingBox[1].y);
#if defined _SHOW_TRIANGLES
   text = offset + TRIANGLE_TEMPLATE;
   for (unsigned a = 0; a < m_tries.size(); a++)
   {
      vec2 triangle[3];
      int startIdx   = m_tries[a];      
      triangle[0]    = m_points->at(m_triangleIndexes->at(startIdx));
      triangle[1]    = m_points->at(m_triangleIndexes->at(startIdx + 1));
      triangle[2]    = m_points->at(m_triangleIndexes->at(startIdx + 2));
      TRACE(text.data(), a, triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, \
         triangle[2].x, triangle[2].y, startIdx);
   }
#endif
}

