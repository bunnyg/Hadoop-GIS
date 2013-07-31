LOAD DATA LOCAL INPATH 'small_data1.tsv' OVERWRITE INTO TABLE ta;
LOAD DATA LOCAL INPATH 'small_data2.tsv' OVERWRITE INTO TABLE tb;

LOAD DATA LOCAL INPATH '/home/aaji/proj/hivetest/join/small_data1.tsv' OVERWRITE INTO TABLE ta;
LOAD DATA LOCAL INPATH '/home/aaji/proj/hivetest/join/small_data2.tsv' OVERWRITE INTO TABLE tb;


