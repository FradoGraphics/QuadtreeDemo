/**
 * Class name:    QuadTree

 * Description:   Builds the tree depending on threshold values. Each node of the tree 
                  represents a region. Each region in turn stores the indices of triangles
                  located in this region. Regions are bounded by axis-aligned bounding boxes.
                  Triangles of a region can be searched for by performing a search with (x,y)
                  coordinates.

 * Author:        Frank Dockhorn
 */


#ifndef _QUADTREE 
#define _QUADTREE

class QuadTreeData;
class ResultData;

class QuadTree
{
public:
	QuadTree						(const QuadTreeData* data);
	QuadTree						();
	~QuadTree					();


	bool							hasTree							() {return m_root != NULL;}
	void                    createTree						(const QuadTreeData* data);
	void                    cleanup							();
	void							findTriangles					(	const vec2& searchPoint, 																			 
																			ResultData& resultSet);
	const PointPtrArray*    getBoundingBoxes				() {return &m_BBArray;}
	const PointPtrArray*    getRegionsLeft					() {return &m_regionsLeft;}
	void                    setSizeBound					(int val) {m_sizeBound = val;}
	void                    setMaxDepth						(int val) {m_maxDepth = val;}
	size_t                  getSizeBound					() {return m_sizeBound;}
	size_t						getDeviation					() {return m_deviationValue;}
	size_t						getDeviationMedian			() {return m_deviationMedian;}
	int                     getMaxDepth						() {return m_maxDepth;}   
	int                     getRealMaxDepth				() {return m_realMaxDepth;}   
	REAL							getDeviationPercent			() {return m_deviationPercent;}
	REAL							getDeviationMedianPercent	() {return m_deviationMedianPercent;}


protected:
	void							computeStatistics				(bool isSizeBound);

private:   
   class QuadNode
   {
   public:
      QuadNode				(const vec2* bb, QuadTree* tree, bool init = false);       
      ~QuadNode			();  

		size_t				getNumTris					() {return m_tries.size();}
      inline int			findRegion					(const vec2& point);
		void					findTriangles				(const vec2& searchPoint, char& depth, 
																				ResultData& resultSet);
		void					addTriangleToRegion		(vec2* triangle, size_t startIdx);
      void					addToRegion					(size_t startIdx, int region);
      void					traceQuadtree				(int currentDepth);
      void					traceSearchResults		(const vec2& searchPoint);
      void					subdivideRegion			(int currentDepth, int& finalDepth);
		bool					isPointinBoundingBox		(const vec2& searchPoint);
      
		enum 
		{
			SW = 0,
			SE = 1,
			NW = 2,
			NE = 3
		};

	protected:
		void findTriangleIntersections				(vec2* triangle, size_t startIdx);
		void findIntersectionsWithX					(vec2& direction, vec2*	triangle, 
																							size_t startIdx, int k);
		void findIntersectionsWithY					(vec2& direction, vec2*	triangle, 
																							size_t startIdx, int k);
	private:
		vec2	m_boundingBox[2];
		Vec2D			m_pivot;
		QuadNode*   m_region[4]; 
		QuadTree*   m_tree;      
		char			m_bitRegions;
		IntArray		m_tries;    
   };   

   const PointArray*       m_points;
	const IntArray*         m_triangleIndexes;
	QuadNode*               m_root;	
	PointPtrArray           m_BBArray;
	PointPtrArray           m_regionsLeft;
	IntArray						m_trianglesLeft;
	size_t						m_totalTris;
	size_t						m_sizeBound;
	size_t						m_deviationValue;
	size_t						m_deviationMedian;
	int                     m_maxDepth;
	int							m_realMaxDepth;
	REAL							m_deviationPercent;
	REAL							m_deviationMedianPercent;	
	REAL							m_ratio;
   static const REAL       D_EPSILON_2;
   static const int        BITVECTOR[];
};


inline void QuadTree::QuadNode::addToRegion(size_t startIdx, int region)
{
   if (!(m_bitRegions & BITVECTOR[region]))
   {         
      m_region[region]->m_tries.push_back(startIdx);
      m_bitRegions |= BITVECTOR[region];
   }
}


#endif