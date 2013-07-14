#include "hadoopgis.h"

int getJoinIndex ()
{
    char * filename = getenv("map_input_file");
    //char * filename = "astroII.1.1";
    if ( NULL == filename ){
	cerr << "map.input.file is NULL." << endl;
	return 1 ;
    }
    int len= strlen(filename);
    int index = filename[len-1] - '1' ;
    return index;
}

int main(int argc, char **argv) {

    int index = getJoinIndex();
    // cerr << "Index: " << index << endl; 
    if (index <0)
    {
	cerr << "InputFileName index is corrupted.. " << endl;
	return 1; //failure 
    }

    string input_line;
    vector<string> fields;

    while(cin && getline(cin, input_line) && !cin.eof()){
	
	boost::split(fields, input_line, boost::is_any_of(BAR));
	if (fields[OSM_TILEID].size()> 2 )
	    cout << fields[OSM_TILEID]<< TAB << index<< BAR << fields[OSM_ID] << BAR <<fields[OSM_POLYGON]<< endl;
	fields.clear();
    }
    cout.flush();

    return 0; // success
}

