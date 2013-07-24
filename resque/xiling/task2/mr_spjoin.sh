#! /bin/bash

make -f makefile

hadooppath=/usr/local/hadoop-0.20.2
enginepath=/Users/hixiaoxi/Documents/GitHub/hivesp/resque/xiling/task2/resque

hdfsoutdir=/user/hixiaoxi/task2/output

predicate=${1}
input=${2}
index_1=${3}
index_2=${4}
output=${5}

hadoop dfs -rmr ${hdfsoutdir}

if [[ $predicate == intersects ]]
then
    echo intersects
    #hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_intersects '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_intersects"
    hadoop jar /usr/local/hadoop-0.20.2/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_intersects '${3}' '${4}'' -file /bin/cat -file /Users/hixiaoxi/Documents/GitHub/hivesp/resque/xiling/task2/resque -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_intersects" 
elif [[ $predicate == touches ]]
then
    echo touches 
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_touches '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_touches"
elif [[ $predicate == crosses ]]
then
    echo crosses
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_crosses '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_crosses"
elif [[ $predicate == contains ]]
then
    echo contains
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_contains '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_contains"
elif [[ $predicate == adjacent ]]
then
    echo adjacent
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_adjacent '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_adjacent"
elif [[ $predicate == disjoint ]]
then
    echo disjoint
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_disjoint '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_disjoint"
elif [[ $predicate == equals ]]
then
    echo equals
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_equals '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_equals"
elif [[ $predicate == dwithin ]]
then
    echo dwithin
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_dwithin '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_dwithin"
elif [[ $predicate == within ]]
then
    echo within
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_within '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_within"
elif [[ $predicate == overlaps ]]
then
    echo overlaps
    hadoop jar ${hadooppath}/contrib/streaming/hadoop-streaming-*.jar -mapper 'cat - ' -reducer 'resque st_overlaps '${3}' '${4}'' -file /bin/cat -file ${enginepath} -input ${input} -output ${output} -verbose -cmdenv LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH -jobconf mapred.job.name="join_overlaps"
fi;
