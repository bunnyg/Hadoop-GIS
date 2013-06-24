#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <stdlib.h> 

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp> 

#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/domains/gis/io/wkt/wkt.hpp>

using namespace std;
using namespace boost;

// data type declaration 
typedef boost::geometry::model::d2::point_xy<int> point;
typedef boost::geometry::model::polygon<point> polygon;
typedef boost::geometry::model::box<point> box;

typedef map<string,map<int,vector<polygon*> > > polymap;
typedef map<string,map<int,vector<box*> > > boxmap;
typedef map<string,map<int,vector<string> > > datamap;

// data block 
polymap markup;
boxmap outline;
datamap data;
const string tab = "\t";
const string sep =  "\x02";
vector<int> join_idx ; 


bool cleanup();
bool readSpatialInput();


bool readSpatialInput() {
    string input_line;
    string record;
    string tile_id;
    string row ;
    vector<string> sp;
    vector<string> fields;
    
    polygon * shape= NULL;
    box * mbb = NULL;
    
    int index = 0;
    int pos   = -1;
    
    while(cin && getline(cin, input_line) && !cin.eof()) {
	//cerr << input_line << endl;
	split( sp,input_line, is_any_of("\t")); // SplitVec == { "hello abc","ABC","aBc goodbye" }

	index = boost::lexical_cast<int>(sp[2]);
	
	pos = join_idx[index];

	split(fields,sp[1], is_any_of(sep));
	
	record= boost::algorithm::join(fields, tab);
	
	shape = new polygon();
	mbb   = new box();
	boost::geometry::read_wkt(fields[pos],*shape);
	boost::geometry::correct(*shape);
	boost::geometry::envelope(*shape, *mbb);
	//assert(boost::geometry::area(*mbb)>=0);
	outline[sp[0]][index].push_back(mbb);
	markup[sp[0]][index].push_back(shape);
	data[sp[0]][index].push_back(record);
	
	cerr <<"Field Count = " << fields.size() << " , index = " << index << endl;
	fields.clear();
	sp.clear();
    }
    return true;
}
bool cleanup() {
}

int main(int argc, char** argv)
{
    if (argc< 3)
    {
	cerr << "usage: join [shape_idx 1] [shape_idx 2] " <<endl;
	return 0;
    }

    // prepare join parameters 
    for (int i =1 ; i <argc ;i++)
	join_idx.push_back(atoi(argv[i]));


    boxmap::iterator iter;
    string key ;
    int size = -1;

    if (readSpatialInput())
    {
	// for each key(tile) in the input stream 
	for (iter= outline.begin(); iter != outline.end(); iter++)
	{
	    key  = iter->first;
	    map<int ,vector<box*> > &mbbs = outline[key];
	    map<int ,vector<polygon*> > &shapes = markup[key];

	    size = mbbs.size(); // join cardinality 
	    vector<polygon> output;

	    int outerloop_size = (mbbs[0]).size(); //src tile polygon size
	    for (int i =1; i< size ;i++)
	    {
		int innerloop_size = (mbbs[i]).size(); // tar tile polygon size 
		for (int j =0; j< outerloop_size; j++)
		{
		    for (int k=0; k< innerloop_size; k++)
		    {
			if (boost::geometry::intersects( *(mbbs[0][j]), *(mbbs[i][k])) && 
				boost::geometry::intersects(*(shapes[0][j]), *(shapes[i][k])) ) // if the mbb && shapes intersects 
			{
			    cout << data[key][0][j]<< tab << data[key][i][k] << endl; // cross product
			}
		    }
		}
	    }

	}
	return 0;
    }
    else 
	return 1;

}

