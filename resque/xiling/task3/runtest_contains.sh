#! /bin/bash

dir=data

if [ -e ${dir}/data1.tsv ] && [ -e ${dir}/data2.tsv ] 
then
    echo "All testing infrastructure is present."
else 
    echo "Test cases are missing... Can't do test without them."
    exit 1
fi

# create the test files.
make -f makefile


# test map contains

echo -n "TEST: Map Contains --- "

cat ${dir}/data1.tsv | ./map_contains ${dir}/query_polygon.tsv 9

#cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_intersects 9 9 > ${dir}/intersects_resque_out.txt
#./map_contains ${dir}/query_polygon.tsv ${dir}/data1.tsv 9 > ${dir}/temp_output.tsv

#diff ${dir}/intersects_resque_out.txt ${dir}/intersects_psql_out.txt >/dev/null 2>&1

#if [ $? -ne 0 ]
#then
#    echo "failed."
#else
#    echo "passed."
#    rm ${dir}/intersects_resque_out.txt
#fi


make clean
