#! /bin/bash

#if [ ! $# == 1 ]; then
#    echo "Usage: $0 [log_id]"
#    exit 0
#fi

make -f Makefile

hdfsoutdir=/user/hixiaoxi/task2/output

hadoop dfs -rmr ${hdfsoutdir}
hadoop dfs -ls task2

hadoop jar ${HADOOP_HOME}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer reducer -file /bin/cat -file reducer -input /user/hixiaoxi/task2/input -output ${hdfsoutdir} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_intersects"
