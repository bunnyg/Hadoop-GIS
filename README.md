HadoopGIS v0.1.0
===========

HadoopGIS is a scalable and high performance spatial data warehousing 
system for running large scale spatial queries on Hadoop. Hadoop-GIS 
supports multiple types of spatial queries on MapReduce through space 
partitioning, customizable spatial query engine RESQUE, implicit parallel 
spatial query execution on MapReduce, and effective methods for amending 
query results through handling boundary objects on MapReduce. 
HadoopGIS takes advantage of global partition indexing and customizable 
on demand local spatial indexing to achieve efficient query processing. 
HadoopGIS is integrated into Hive to support declarative spatial queries with an integrated architecture. 

HadoopGIS comes with two versions: 
* Hadoop-GIS library: a set of libraries on performing MapReduce based spatial queries, which can be called from users' applications.
* HiveSP: we integrate Hadoop-GIS with Hive, to support both structured queries and spatial queries with a unified query language and interface. 

HiveSP defines a simple SQL-like query language, that enables
users familiar with SQL to query large spatial data. 

General Info
--------

For the latest information about HadoopGIS, please visit out website at:

  http://confluence.cci.emory.edu:8090/display/HadoopGIS/Home



Requirements
--------

- Java 1.6

- Hadoop 0.20.x (x >= 1)

- GEOS 3.x (x >= 3)

- libspatialindex 1.8.x (x >= 0)


Contributing
--------

1. Fork it.
2. Create a branch (`git checkout -b my_hadoopgis`)
3. Commit your changes (`git commit -am "Added ST_Regression"`)
4. Push to the branch (`git push origin my_hadoopgis`)
5. Open a Pull Request
6. Enjoy a refreshing Turkish tea and wait


Licence
--------
All Hadopo-GIS software is freely available, and all source code 
is available under GNU public [copyleft](http://www.gnu.org/copyleft/ "copyleft") licenses.

