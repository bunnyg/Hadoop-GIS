SELECT ta.rec_id, tb.rec_id FROM ta JOIN tb ON (Intersects(ta.outline, tb.outline) = TRUE);

