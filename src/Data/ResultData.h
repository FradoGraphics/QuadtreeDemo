/**
* Class name:    QuadTreeData

* Description:   Keeps result data of a quad tree search traversal.

* Author:        Frank Dockhorn
*/


#ifndef _RESULTDATA
#define _RESULTDATA


class ResultData
{
public:
	ResultData()
		: m_depth(0),
		m_triangles(NULL)
	{}

	char					m_depth;
	const IntArray*	m_triangles;
	vec2					m_boundingBox[2];
};

#endif

