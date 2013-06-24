EXPLAIN EXTENDED FROM ta SELECT TRANSFORM (ta.rec_id ,ta.tile_id ) USING '/usr/bin/tee' ;
