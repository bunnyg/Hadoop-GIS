#! /bin/bash

make -f makefile

hadooppath=/usr/local/hadoop-0.20.2
enginepath=/Users/hixiaoxi/Documents/GitHub/hivesp/resque/xiling/task3/map_contains

hdfsoutdir=/user/hixiaoxi/task3/output

query_polygon=${1}
input=${2}
index=${3}

hadoop dfs -rmr ${hdfsoutdir}

hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'map_contains "'"${query_polygon}"'" '${index}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${hdfsoutdir} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="map_contains"

make clean
