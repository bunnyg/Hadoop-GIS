#include "MReducerx.h"

bool readSpatialInput() {
    string input_line;
    int index =0;
    string key ;
    string value;
    vector<string> fields;
    size_t pos;

    GeometryFactory *gf = new GeometryFactory(new PrecisionModel(),OSM_SRID);
    WKTReader *wkt_reader= new WKTReader(gf);
    Geometry *poly = NULL; 
    
    while(cin && getline(cin, input_line) && !cin.eof()) {

	pos = input_line.find_first_of(tab);
	key = input_line.substr(0,pos);
	value = input_line.substr(pos+1) ;
	boost::split(fields, value, boost::is_any_of(bar));
	index = boost::lexical_cast< int >( fields[0]);

	infodata[key][index].push_back(fields[1]);

	poly = wkt_reader->read(fields[2]);
	polydata[key][index].push_back(poly);

	fields.clear();
	cerr.flush();
    }

    cerr << "size" << "\t" << polydata.size() << endl;
    return true;
}


int main(int argc, char** argv)
{
    IStorageManager ** storages = NULL ;//new IStorageManager * [data.size()];
    ISpatialIndex ** forest = NULL ;    //new ISpatialIndex * [data.size()];
    id_type * indexIdentifier = NULL;
    polymap::iterator iter;

    bool initilize = true;
    string key ;
    int size = -1;
    bool skip= false;
    int joincar = 0;

    if (! readSpatialInput()) 
    {
	cerr << "Reduce input parsing error...." << endl;
	return 1;
    }

    for (iter= polydata.begin(); iter !=polydata.end(); iter++)
    {
	if (polydata[iter->first].size() > joincar){
	    joincar=polydata[iter->first].size();
	    cerr << "|join|=" << joincar << endl;
	    cerr << "|T|=" << polydata.size() << endl;
	    // cerr << "  Note: if this prints more than once then probably there is a bug." << endl;
	}
	//cerr << "size=" <<polydata[iter->first].size() << endl;
    }


    try { 
        // for each tile in the input stream 
        for (iter= polydata.begin(); iter !=polydata.end(); iter++) {
            key  = iter->first;
            cerr << "[" << key <<"]"<<endl;
            current_key = key;

            map<int,vector<Geometry*> > & images = polydata[key];
            size = images.size();   // join size

            if (size < joincar) {
                cerr << "skipping [" << key <<"] "<<endl;
                continue ;
            }

            if (initilize) {
                //we assume all the multiwya joins are the same.
                storages = new IStorageManager * [size];
                forest = new ISpatialIndex * [size];
                indexIdentifier = new id_type [size];
                initilize =false;
            }

            // for each spatial data in a set of images 
            for (int i = 0; i < size; i++) {
                storages[i]= StorageManager::createNewMemoryStorageManager();
                MRJDataStream stream(&(images[i]), i+1);

                // Create and bulk load a new RTree with dimensionality 2, 
                // using memory as the StorageManager and the RSTAR splitting policy.
                forest[i]= RTree::createAndBulkLoadNewRTree( 
                    RTree::BLM_STR, stream, *storages[i], FillFactor, IndexCapacity, LeafCapacity, 2, 
                    SpatialIndex::RTree::RV_RSTAR, indexIdentifier[i]);

                bool ret = forest[i]->isIndexValid();

                if (ret == false) {
                    cerr << "ERROR: Structure is invalid!" << endl;
                }
            }

            MRJVisitor vis;
            forest[0]->joinMQuery(vis, forest, size);
            vis.getInfo();

            // clean up the allocated space
            for (int i =0 ; i < size; i++)
            {
                if (NULL != forest[i])
                    delete forest[i];
                if (NULL != storages[i])
                    delete storages[i];
            }
        }
        /*
           delete [] forest;
           delete [] storages;
           delete [] indexIdentifier;
           */
    }
    catch (Tools::Exception& e) {
        std::cerr << "******ERROR******" << std::endl;
        std::string s = e.what();
        std::cerr << s << std::endl;
        return -1;
    }
    return 0;
}

