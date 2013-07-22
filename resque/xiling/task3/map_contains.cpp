#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <stdlib.h> 
#include <fstream>


// geos
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTReader.h>
#include <geos/opBuffer.h>

#include <spatialindex/SpatialIndex.h>

using namespace std;
using namespace geos;
using namespace geos::io;
using namespace geos::geom;
using namespace geos::operation::buffer; 

#define OSM_SRID 4326

// Constants
#define ST_INTERSECTS 1
#define ST_TOUCHES 2
#define ST_CROSSES 3
#define ST_CONTAINS 4
#define ST_ADJACENT 5
#define ST_DISJOINT 6
#define ST_EQUALS 7
#define ST_DWITHIN 8
#define ST_WITHIN 9
#define ST_OVERLAPS 10

#define DATABASE_ID_ONE 1
#define DATABASE_ID_TWO 2

// data type declaration 
typedef map<string, map<int, Geometry*> > polymap;
typedef map<string,map<int, string> > datamap;

polymap polydata;
datamap data;
vector<Geometry*> query_polygon_set;

const string bar= "|";
const string tab = "\t";
const string comma = ",";
const string sep =  "\x02"; // ctrl+a


// data block 
string TILE_ID_ONE = "oligoastroIII.2_40x_20x_NS-MORPH_1";
string TILE_ID_TWO = "oligoastroIII.2_40x_20x_NS-MORPH_2";

int PREDICATE = 0;
int shape_idx_1 = -1;
int shape_idx_2 = -1;

bool readQueryPolygon(const char *query_polygon);
bool filterByContains(const char *data_file);

vector<string> split(string str, string separator);

int main(int argc, char** argv)
{
    cerr << endl;
    cerr << "argv[0] = " << argv[0] << endl;
    cerr << "argv[1] = " << argv[1] << endl;
    cerr << "argv[2] = " << argv[2] << endl;
    cerr << "argv[3] = " << argv[3] << endl;

    if (argc < 4) {
        cerr << "usage: map_contains polygon dataset [shape_idx 1]" << endl;
	    return 0;
    }

    shape_idx_1 = strtol(argv[3], NULL, 10);

    const char *query_polygon = new char[1024];
    query_polygon = argv[1];

    if (!readQueryPolygon(query_polygon)) {
        return 1;
    }
    
    const char *data_file = new char [1024];
    data_file = argv[2];
    if (!filterByContains(data_file)) {
	    return 1;
    }

    return 0;
}

bool readQueryPolygon(const char *query_polygon)
{
    ifstream query_polygon_in(query_polygon, ios::in);

    if (query_polygon_in == NULL) {
        cerr << "query polygon file is empty" << endl;
        return false;
    }
    else {
        GeometryFactory *gf = new GeometryFactory(new PrecisionModel(), OSM_SRID);
        WKTReader *wkt_reader = new WKTReader(gf);
        Geometry *poly = NULL; 
        string polygon_line;

        // vector<Geometry*> query_polygon_set;
        while (getline(query_polygon_in, polygon_line)) {
            // cerr << "polygon_line = " << polygon_line << endl;
            poly = wkt_reader->read(polygon_line);
            query_polygon_set.push_back(poly);
        }
    }
    cerr << "query_polygon_set size = " << query_polygon_set.size() << endl;
    return true;
}

bool filterByContains(const char *data_file)
{
    ifstream data_in(data_file, ios::in);

    if (data_in == NULL) {
        cerr << "data file is empty" << endl;
        return false;
    }
    else {
        cerr << "read data file: " << data_file << endl;

        string input_line;
        vector<string> fields;

        GeometryFactory *gf = new GeometryFactory(new PrecisionModel(),OSM_SRID);
        WKTReader *wkt_reader = new WKTReader(gf);
        Geometry *poly = NULL; 

        int index = 0;
        while (getline(data_in, input_line)) {
            
            fields = split(input_line, tab);

            // cerr << "input_line: " << input_line << endl;
            // cerr << "fields size = " << fields.size() << endl;
            // cerr << "fields[0] = " << fields[0] << endl; 
            // cerr << "fields[1] = " << fields[1] << endl; 
            // cerr << "fields[2] = " << fields[2] << endl; 

            poly = wkt_reader->read(fields[shape_idx_1+1]);

            for (vector<Geometry *>::iterator iter = query_polygon_set.begin(); iter != query_polygon_set.end(); iter++) {
                if ((*iter)->contains(poly)) {
                    cout << "cout: " << input_line << endl;
                    cerr << "cerr: " << input_line << endl;
                    index++;
                    break;
                }
            }
            fields.clear();
            cerr.flush();
        }
        cerr << "index = " << index << endl;
    }
    return true;
}

vector<string> split(string str, string separator)  
{  
    vector<string> result;  
    int cutAt;  

    while((cutAt = str.find_first_of(separator)) != str.npos) {  
        if(cutAt >= 0) {  
            result.push_back(str.substr(0, cutAt));  
        }  
        str = str.substr(cutAt + 1);  
    }  

    if(str.length() > 0) {  
        result.push_back(str);  
    }  
    return result;  
}  
