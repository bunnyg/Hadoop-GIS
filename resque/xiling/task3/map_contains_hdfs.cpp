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

// spatial index
#include <spatialindex/SpatialIndex.h>

// libhdfs
#include "hdfs.h"

using namespace std;
using namespace geos;
using namespace geos::io;
using namespace geos::geom;
using namespace geos::operation::buffer; 

#define OSM_SRID 4326

// Constants
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
bool filterByContains();

vector<string> split(string str, string separator);

int main(int argc, char** argv)
{
    cerr << endl;
    cerr << "argv[0] = " << argv[0] << endl;
    cerr << "argv[1] = " << argv[1] << endl;
    cerr << "argv[2] = " << argv[2] << endl;

    if (argc < 3) {
        cerr << "usage: map_contains polygon [shape_idx 1]" << endl;
	    return 0;
    }

    shape_idx_1 = strtol(argv[2], NULL, 10);

    const char *query_polygon = new char[1024];
    query_polygon = argv[1];

    if (!readQueryPolygon(query_polygon)) {
        return 1;
    }
    
    if (!filterByContains()) {
	    return 1;
    }

    return 0;
}

bool readQueryPolygon(const char *query_polygon)
{
    hdfsFS fs = hdfsConnect("localhost:9000", 0);

    if(!fs) {
        cerr << "Oops! Failed to connect to hdfs!" << endl;
        return false;
    }

    hdfsFile readFile = hdfsOpenFile(fs, query_polygon, O_RDONLY, 0, 0, 0);
    if (!readFile) {
        cerr << "Failed to open " << query_polygon << "for writing" << endl;
        return false;
    }

    char* buffer = (char*) malloc(sizeof(char) * 1024);
    if(buffer == NULL) {
        return false;
    }

    hdfsRead(fs, readFile, buffer, 1024);
    cerr << buffer << endl;
    cout << buffer << endl;

    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);
    return true;

    /*
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
             cerr << "polygon_line = " << polygon_line << endl;
            poly = wkt_reader->read(polygon_line);
            query_polygon_set.push_back(poly);
        }
    }
    cerr << "query_polygon_set size = " << query_polygon_set.size() << endl;
    return true;
    */
}

bool filterByContains()
{
    string input_line;
    vector<string> fields;

    GeometryFactory *gf = new GeometryFactory(new PrecisionModel(),OSM_SRID);
    WKTReader *wkt_reader = new WKTReader(gf);
    Geometry *poly = NULL; 

    int index = 0;
    // while (getline(data_in, input_line)) {
    while (cin && getline(cin, input_line) && !cin.eof()) {
        
        fields = split(input_line, tab);

        cerr << "input_line: " << input_line << endl;
        cerr << "fields size = " << fields.size() << endl;
        cerr << "fields[0] = " << fields[0] << endl; 
        cerr << "fields[1] = " << fields[1] << endl; 
        cerr << "fields[2] = " << fields[2] << endl; 

        poly = wkt_reader->read(fields[shape_idx_1+1]);

        for (vector<Geometry *>::iterator iter = query_polygon_set.begin(); iter != query_polygon_set.end(); iter++) {
            if ((*iter)->contains(poly)) {
                cout << input_line << endl;
                index++;
                break;
            }
        }
        fields.clear();
        cerr.flush();
    }
    cerr << "index = " << index << endl;
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
