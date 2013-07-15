#! /bin/bash

#if [ ! $# == 1 ]; then
#    echo "Usage: $0 [log_id]"
#    exit 0
#fi

make -f Makefile

reco=scalability
# reco=$1
export HADOOP_HOME=/usr/local/hadoop-0.20.2

hdfsoutdir=/user/hixiaoxi/task2/output

hadoop dfs -rmr ${hdfsoutdir}
hadoop dfs -ls task2

date >> osm.${reco}.log

for j in 1 
    #2 3
do
    echo "round ${j}"
    for reducecount in 200 
        #150 100 80 60 40 20 10
    # for maxmap in 4 2 1
    do
	# reducecount=`expr ${maxmap} \\* 8`
	START=$(date +%s)

	hadoop jar ${HADOOP_HOME}/contrib/streaming/hadoop-streaming-*.jar -mapper mapper -reducer reducer -file mapper -file reducer -input /user/hixiaoxi/task2/input -output ${hdfsoutdir} -numReduceTasks ${reducecount} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="osm_join_${reducecount}"  -jobconf mapred.task.timeout=360000000
	# sudo -u hdfs hadoop jar ${HADOOP_HOME}/contrib/streaming/hadoop-streaming-*.jar -D mapred.tasktracker.map.tasks.maximum=${maxmap} -D mapred.tasktracker.reduce.tasks.maximum=${maxmap} -mapper mapper -reducer reducer -file mapper -file reducer -input /user/aaji/osm/smalltile -output ${hdfsoutdir} -numReduceTasks ${reducecount} -verbose -cmdenv LD_LIBRARY_PATH=/home/aaji/softs/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="osm_join_${reducecount}"  -jobconf mapred.task.timeout=36000000


	END=$(date +%s)
	DIFF=$(( $END - $START ))
	echo "${reducecount},${DIFF}" >> osm.${reco}.log

	# sudo -u hdfs hdfs dfs -copyToLocal /user/aaji/joinout ${OUTDIR}/mjoin_${1}_${reducecount}
	# hadoop dfs -rmr ${hdfsoutdir}
    done
    echo "" >>osm.${reco}.log
done
