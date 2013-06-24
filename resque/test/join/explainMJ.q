EXPLAIN SELECT Area(Intersection(ajointable.outline, bjointable.outline)) FROM ajointable JOIN bjointable ON (Intersects(ajointable.outline, bjointable.outline) = TRUE);

