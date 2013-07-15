#include "hadoopgis.h"

vector<string> split(string str, string separator);

int getJoinIndex ()
{
    char * filename = getenv("map_input_file");
    //char * filename = "astroII.1.1";
    
    if ( NULL == filename ){
        cerr << "map.input.file is NULL." << endl;
        return 1 ;
    }

    cout << "filename = " << filename << endl;

    int len = strlen(filename);
    cout << "len = " << len << endl;

    int index = filename[len-1] - '1' ;
    cout << "index = " << index << endl;

    return index;
}

/*
#define OSM_ID 0
#define OSM_TILEID 1
#define OSM_OID 2
#define OSM_ZORDER 3
#define OSM_POLYGON 4
*/

int main(int argc, char **argv) 
{
    cout << "cout =========================================================================" << endl;
    cout.flush();
    
    cerr << "cerr =========================================================================" << endl;
    cerr.flush();


    int index = getJoinIndex();
    // cerr << "Index: " << index << endl; 
    if (index < 0) {
        cerr << "InputFileName index is corrupted.. " << endl;
        return 1; //failure 
    }

    string input_line;
    vector<string> fields;

    while (cin && getline(cin, input_line) && !cin.eof()) {
        fields = split(input_line, BAR);

        if (fields[OSM_TILEID].size() > 2 ) {
            cout << fields[OSM_TILEID] << TAB << index << BAR << fields[OSM_ID] << BAR << fields[OSM_POLYGON]<< endl;
        }
        fields.clear();
    }

    cout.flush();
    return 0; // success
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
