EXPLAIN EXTENDED SELECT ta.outline, tb.outline FROM ajointable ta JOIN ajointable tb ON (Intersects(ta.outline, tb.outline) = TRUE);

