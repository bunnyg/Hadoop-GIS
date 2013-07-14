#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

// std 
#include <iostream>
#include <string>
#include <vector>
#include <map>

// boost 
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/domains/gis/io/wkt/wkt.hpp>

// spatial index 
#include <spatialindex/SpatialIndex.h>

// geos 
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTReader.h>

#define PAIS_SRID 4326
#define OSM_SRID 4326

#define FillFactor 0.7
#define IndexCapacity 100
#define LeafCapacity 50
#define COMPRESS true
#define TILE_SIZE 4096

#define OSM_ID 0
#define OSM_TILEID 1
#define OSM_OID 2
#define OSM_ZORDER 3
#define OSM_POLYGON 4



using namespace std;

using namespace geos;
using namespace geos::io;
using namespace geos::geom;

using namespace SpatialIndex;

using boost::lexical_cast;
using boost::bad_lexical_cast;

typedef boost::geometry::model::d2::point_xy<int> point;
typedef boost::geometry::model::polygon<point> polygon;
typedef boost::geometry::model::box<point> box;

typedef map<string,map<int,vector<polygon> > > polygonmap;

const string UNDERSCORE = "_";
const string BAR= "|";
const string TAB = "\t";
const string COMMA = ",";
const string SPACE = " ";

const string shapebegin = "POLYGON((";
const string shapeend = "))";

