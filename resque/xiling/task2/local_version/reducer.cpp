#include "MReducerx.h"

int PREDICATE = 0;
int shape_idx_1 = -1;
int shape_idx_2 = -1;

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

bool readSpatialInput();
vector<string> split(string str, string separator);

bool join_intersects();
bool join_touches();
bool join_crosses();
bool join_contains();
bool join_adjacent();
bool join_disjoint();
bool join_equals();
bool join_dwithin();
bool join_within();
bool join_overlaps();
bool cleanup();

bool readSpatialInput() 
{
    string input_line;
    
    string key;
    string value;

    vector<string> fields;

    int database_id = 0;
    size_t key_pos;

    GeometryFactory *gf = new GeometryFactory(new PrecisionModel(),OSM_SRID);
    WKTReader *wkt_reader = new WKTReader(gf);
    Geometry *poly = NULL; 

    while(cin && getline(cin, input_line) && !cin.eof()) {
        // cerr << "input_line: " << input_line << endl;
        
        key_pos = input_line.find_first_of(tab);
        // cout << "key_pos = " << key_pos << endl;

        key = input_line.substr(0, key_pos);
        // cout << "key = " << key << endl;

        value = input_line.substr(key_pos+1) ;
        // cout << "value = " << value << endl;

        fields = split(value, tab);
        // cout << "fields[0] = " << fields[0] << endl; 
        // cout << "fields[1] = " << fields[1] << endl; 
        // cout << "fields[2] = " << fields[2] << endl; 

        // fiedls[0] is the database id
        database_id = atoi(fields[0].c_str());
        // cout << "database_id = " << database_id << endl;

        // fiedls[shape_idx_1] is the polygon for the 1st input file 
        // fiedls[shape_idx_2] is the polygon for the 2nd input file 
        if (database_id == DATABASE_ID_ONE) {
            poly = wkt_reader->read(fields[shape_idx_1]);
        }
        else if (database_id == DATABASE_ID_TWO) {
            poly = wkt_reader->read(fields[shape_idx_2]);
        }
        else {
            cout << "wrong database id : " << database_id << endl;       
            return false;
        }

        polydata[key][database_id].push_back(poly);

        fields.clear();
        cerr.flush();

        // cout << endl;
    }

    // cerr << "polydata size = " << polydata.size() << endl;
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

int main(int argc, char** argv)
{
    if (argc < 4) {
        cerr << "usage: resque [predicate] [shape_idx 1] [shape_idx 2] " <<endl;
	    return 0;
    }

    cout << "argv[1] = " << argv[1] << endl;
    cout << "argv[2] = " << argv[2] << endl;
    cout << "argv[3] = " << argv[3] << endl;

    shape_idx_1 = strtol(argv[2], NULL, 10);
    shape_idx_2 = strtol(argv[3], NULL, 10);

    if (strcmp(argv[1], "st_intersects") == 0) {
	    PREDICATE = ST_INTERSECTS;
    } 
    else if (strcmp(argv[1], "st_touches") == 0) {
	    PREDICATE = ST_TOUCHES;
    } 
    else if (strcmp(argv[1], "st_crosses") == 0) {
	    PREDICATE = ST_CROSSES;
    } 
    else if (strcmp(argv[1], "st_contains") == 0) {
	    PREDICATE = ST_CONTAINS;
    } 
    else if (strcmp(argv[1], "st_adjacent") == 0) {
	    PREDICATE = ST_ADJACENT;
    } 
    else if (strcmp(argv[1], "st_disjoint") == 0) {
	    PREDICATE = ST_DISJOINT;
    }
    else if (strcmp(argv[1], "st_equals") == 0) {
	    PREDICATE = ST_EQUALS;
    }
    else if (strcmp(argv[1], "st_dwithin") == 0) {
	    PREDICATE = ST_DWITHIN;
    }
    else if (strcmp(argv[1], "st_within") == 0) {
	    PREDICATE = ST_WITHIN;
    }
    else if (strcmp(argv[1], "st_overlaps") == 0) {
	    PREDICATE = ST_OVERLAPS;
    }
    else {
        cerr << "wrong argv[1], return" << endl;
        return 1;
    }


    if (!readSpatialInput()) {
	    return 1;
    }

    switch (PREDICATE){
    case ST_INTERSECTS:
        join_intersects();
        break;
    case ST_TOUCHES:
        join_touches();
        break;
    case ST_CROSSES:
        join_crosses();
        break;
    case ST_CONTAINS:
        join_contains();
        break;
    case ST_ADJACENT:
        join_adjacent();
        break;
    case ST_DISJOINT:
        join_disjoint();
        break;
    case ST_EQUALS:
        join_equals();
        break;
    case ST_DWITHIN:
        join_dwithin();
        break;
    case ST_WITHIN:
        join_within();
        break;
    case ST_OVERLAPS:
        join_overlaps();
        break;
    default:
        cerr << "ERROR: unknown spatial predicate " << endl;
        return 1;
    }

    return 0;
}

bool join_intersects() 
{
    cout << "---------------------------------------------------" << endl;
    bool success = false;
    bool flag = false; 

    string key;
    polymap::iterator iter;
    
    try { 
        // for each tile (key) in the input stream 
        for (iter = polydata.begin(); iter != polydata.end(); iter++) {
            key = iter->first;
            // cerr << "key = " << key << endl;

            map<int,vector<Geometry*> > & polygons = polydata[key];
            // cout << "polygons size = " << polygons.size() << endl;

            vector<Geometry*> poly_set_one = polygons[DATABASE_ID_ONE];
            vector<Geometry*> poly_set_two = polygons[DATABASE_ID_TWO];

            int len1 = poly_set_one.size();
            int len2 = poly_set_two.size();

            for (int i = 0; i < len1 ; i++) {
                const Geometry* geom1 = poly_set_one[i];
                const Envelope * env1 = geom1->getEnvelopeInternal();
                
                for (int j = 0; j < len2 ; j++) {
                    const Geometry* geom2 = poly_set_two[j];
                    const Envelope * env2 = geom2->getEnvelopeInternal();

                    if (env1->intersects(env2) && geom1->intersects(geom2)) {
                        cout << i << " of poly_set_one intersects " << j << " of poly_set_two" << endl; 
                        if (flag == false) {
                            flag = true;
                        }
                    }
                } // end of for (int j = 0; j < len2 ; j++) 
            } // end of for (int i = 0; i < len1 ; i++) 	
        } // end of for (iter = polydata.begin(); iter != polydata.end(); iter++)
    } // end of try
    catch (Tools::Exception& e) {
        std::cerr << "******ERROR******" << std::endl;
        std::string s = e.what();
        std::cerr << s << std::endl;
        return -1;
    } // end of catch

    if (flag == false) {
        cout << "There is no intersection in these two polygon sets." << endl;
    }

    success = true ;
    return success;
}

bool join_touches()
{
    return true;
}
bool join_crosses()
{
    return true;
}
bool join_contains()
{
    return true;
}
bool join_adjacent()
{
    return true;
}
bool join_disjoint()
{
    return true;
}
bool join_equals()
{
    return true;
}
bool join_dwithin()
{
    return true;
}
bool join_within()
{
    return true;
}
bool join_overlaps()
{
    return true;
}
bool cleanup()
{
    return true;
}
