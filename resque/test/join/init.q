
add jar jts-1.12.jar ;
add jar emory.jar ;

CREATE TEMPORARY FUNCTION ST_Intersectc   AS 'edu.emory.cci.hive.udf.UDFIntersects' ;
CREATE TEMPORARY FUNCTION Intersects      AS 'edu.emory.cci.hive.udf.UDFIntersects' ;
CREATE TEMPORARY FUNCTION ST_Intersection AS 'edu.emory.cci.hive.udf.UDFIntersection' ;
CREATE TEMPORARY FUNCTION Intersection    AS 'edu.emory.cci.hive.udf.UDFIntersection' ;
CREATE TEMPORARY FUNCTION ST_Area         AS 'edu.emory.cci.hive.udf.UDFArea' ;
CREATE TEMPORARY FUNCTION Area            AS 'edu.emory.cci.hive.udf.UDFArea' ;

