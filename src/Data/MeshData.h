/**
 * Class name:    MeshData

 * Description:   Keeps data related to the mesh. 

 * Author:        Frank Dockhorn
 */



#ifndef _MESHDATA
#define _MESHDATA

class MeshData
{
public:
	MeshData() : m_scanToUse(NULL) {}
	~MeshData(){}

	bool checkData				();
	bool parseVertexValues	(const char* buf);
	bool parseFaceValues		(const char* buf);
	bool filterData			();

	void cleanup()
	{
		PointArray().swap(m_points);
		IntArray().swap(m_indexes);
	}

	const PointArray& getPointArray() const
	{
		return m_points;
	}
	const IntArray& getIndexArray() const
	{
		return m_indexes;
	}
	void addPoint(vec2& val)
	{
		m_points.push_back(val);
	}
	void addIndex(int& val)
	{
		m_indexes.push_back(val);
	}
private:
	static const char* SCAN_PARAM[2];
	const char*	m_scanToUse;
	PointArray	m_points;
	IntArray		m_indexes;
};

#endif