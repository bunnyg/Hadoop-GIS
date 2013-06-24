
# load UDF
add jar jts-1.12.jar ;
add jar emory.jar ;


CREATE TEMPORARY FUNCTION ST_Area AS 'edu.emory.cci.hive.udf.ST_Area' ; 
CREATE TEMPORARY FUNCTION ST_Contains AS 'edu.emory.cci.hive.udf.ST_Contains' ; 
CREATE TEMPORARY FUNCTION ST_MBB AS 'edu.emory.cci.hive.udf.ST_MBB' ; 
CREATE TEMPORARY FUNCTION MBB AS 'edu.emory.cci.hive.udf.ST_MBB' ; 

