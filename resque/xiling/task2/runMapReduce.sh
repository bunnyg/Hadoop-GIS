#! /bin/bash

#if [ ! $# == 1 ]; then
#    echo "Usage: $0 [log_id]"
#    exit 0
#fi

make -f Makefile

hdfsoutdir=/user/hixiaoxi/task2/output

hadoop dfs -rmr ${hdfsoutdir}
hadoop dfs -ls task2

hadoop jar /usr/local/hadoop-0.20.2/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_intersects 5 5' -file /bin/cat -file /usr/local/bin/resque -input /user/hixiaoxi/task2/input -output ${hdfsoutdir} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_intersects"

