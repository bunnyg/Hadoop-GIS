SELECT tc.rec_id, td.rec_id FROM tc JOIN td ON (st_overlaps(tc.outline, td.outline) = TRUE);

SELECT tc.rec_id, td.rec_id FROM tc JOIN td ON (st_touches(tc.outline, td.outline) = TRUE); 

SELECT tc.rec_id, td.rec_id FROM tc JOIN td ON (st_intersects(tc.outline, td.outline) = TRUE);
