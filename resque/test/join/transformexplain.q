EXPLAIN FROM (SELECT ta.rec_id AS goo ,tb.rec_id AS ci FROM ta JOIN tb ON (ta.outline= tb.outline)) temp 
SELECT TRANSFORM (temp.goo ,temp.ci) USING '/usr/bin/wc -m' ;
