#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <vector>
#include <map>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// geos 
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTReader.h>

#include <spatialindex/SpatialIndex.h>


#define PAIS_SRID 4326
#define OSM_SRID 4326

#define FillFactor 0.7
#define IndexCapacity 100
#define LeafCapacity 100
#define COMPRESS true


using namespace std;
using namespace SpatialIndex;

using namespace geos;
using namespace geos::io;
using namespace geos::geom;

using boost::lexical_cast;
using boost::bad_lexical_cast;


typedef map<string,map<int,vector<string> > > infomap;

typedef map<string,map<int,vector<Geometry*> > > polymap;


const string bar= "|";
const string tab = "\t";
const string comma = ",";

polymap polydata;
infomap infodata;


vector<string> joinresults;

const int DEBUG=0;

string current_key = "" ;

// example of a Visitor pattern.
class MRJVisitor : public IVisitor
{
    public:
	size_t m_indexIO;
	size_t m_leafIO;
	vector<string> coll;

    private: 
	int m_count;

    public:
	MRJVisitor() : m_indexIO(0), m_leafIO(0) {m_count=0;}

	void visitNode(const INode& n)
	{   
	    if (n.isLeaf()) m_leafIO++;
	    else m_indexIO++;
	}  
	  
	void getInfo(){
	    cerr << "[intersecting pairs=" <<coll.size() <<"]" << endl;
	    for (int i=0; i< coll.size();i++)
	    {
		std::vector<std::string> strs;
		std::vector<int> indices;

		boost::split(strs, coll[i], boost::is_any_of(comma));
		for (int j=0; j< strs.size();j++)
		    indices.push_back(boost::lexical_cast<int>(strs[j]));

		for (int j=1; j<indices.size();j++){ 
		    if (polydata[current_key][0][indices[0]]->intersects(polydata[current_key][j][indices[j]]))
			cout << current_key << tab << infodata[current_key][0][indices[0]]<< bar <<infodata[current_key][j][indices[j]] <<endl;
		}
	    }
	}

	void visitData(const IData& d)
	{   
	    // data should be an array of characters representing a Region as a string.
	    byte* pData = 0;
	    uint32_t cLen = 0;
	    d.getData(cLen, &pData);
	    // do something.
	    //string s = reinterpret_cast<char*>(pData);
	    //cout << s << endl;
	    delete[] pData;

	    cout << "answer: "<<d.getIdentifier() << endl;
	    // the ID of this data entry is an answer to the query. I will just print it to stdout.
	}

	void visitData(std::vector<const IData*>& v)
	{
	    // to be filled with logic ;
	}

	void visitData(std::vector<uint32_t>& v)
	{
	    v.push_back(1);
	    //coll.push_back(v);
	}

	void visitData(string & s)
	{
	    coll.push_back(s);
	}

};




RTree::Data* parseInputPolygon(Geometry *p, id_type m_id) {
    double low[2], high[2];
    const Envelope * env = p->getEnvelopeInternal();

    low [0] = env->getMinX();
    low [1] = env->getMinY();

    high [0] = env->getMaxX();
    high [1] = env->getMaxY();

    Region r(low, high, 2);

    //std::cerr << " parseInputPolygon m_id: "  << m_id << std::endl;
    return new RTree::Data(0, 0 , r, m_id);// store a zero size null poiter.
}


class MRJDataStream : public IDataStream
{
    public:
	MRJDataStream(vector<Geometry*> * invec, int tag ) : m_pNext(0), index(0), len(0),m_id(0)
    {
	if ( invec->empty())
	    throw Tools::IllegalArgumentException("Input size is ZERO.");
	vec = invec;
	len = vec->size();
	readNextEntry();
	tagg= tag;
    }

	virtual ~MRJDataStream()
	{
	    if (m_pNext != 0) delete m_pNext;
	}

	virtual IData* getNext()
	{
	    if (m_pNext == 0) return 0;

	    RTree::Data* ret = m_pNext;
	    m_pNext = 0;
	    readNextEntry();
	    return ret;
	}

	virtual bool hasNext()
	{
	    return (m_pNext != 0);
	}

	virtual uint32_t size()
	{
	    return vec->size();
	    //throw Tools::NotSupportedException("Operation not supported.");
	}

	virtual void rewind()
	{
	    if (m_pNext != 0)
	    {
		delete m_pNext;
		m_pNext = 0;
	    }

	    index = 0;
	    m_id  = 0;
	    readNextEntry();
	}

	void readNextEntry()
	{
	    if (index < len)
	    {
		//std::cout << "readNextEntry m_id == " << m_id << std::endl;
		m_pNext = parseInputPolygon((*vec)[index], m_id);
		index++;
		m_id++;
	    }
	}

	RTree::Data* m_pNext;
	vector<Geometry*> * vec; 
	int len;
	int index ;
	id_type m_id;
	int tagg ;
};

