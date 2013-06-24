SELECT Area(Intersection(ta.outline, tb.outline)) FROM ta JOIN tb ON (Intersects(ta.outline, tb.outline) = TRUE);

