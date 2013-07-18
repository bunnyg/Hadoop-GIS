#! /bin/bash

dir=data

if [ -e ${dir}/new_test_1.tsv ] && [ -e ${dir}/new_test_2.tsv ] 
then
    echo "All testing infrastructure is present."
else 
    echo "Test cases are missing... Can't do test without them."
    exit 1
fi

# create the test files.
make -f makefile


# test resque intersects

echo -n "TEST: Resque Intersects --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_intersects 9 9 > ${dir}/intersects_resque_out.txt

diff ${dir}/intersects_resque_out.txt ${dir}/intersects_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/intersects_resque_out.txt
fi


# test resque touches 

echo -n "TEST: Resque Touches --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_touches 9 9 > ${dir}/touches_resque_out.txt

diff ${dir}/touches_resque_out.txt ${dir}/touches_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/touches_resque_out.txt
fi


# test resque crosses

echo -n "TEST: Resque Crosses --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_crosses 9 9 > ${dir}/crosses_resque_out.txt

diff ${dir}/crosses_resque_out.txt ${dir}/crosses_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/crosses_resque_out.txt
fi


# test resque contains

echo -n "TEST: Resque Contains --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_contains 9 9 > ${dir}/contains_resque_out.txt

diff ${dir}/contains_resque_out.txt ${dir}/contains_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/contains_resque_out.txt
fi


# test resque adjacent

echo -n "TEST: Resque Adjacent --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_adjacent 9 9 > ${dir}/adjacent_resque_out.txt

diff ${dir}/adjacent_resque_out.txt ${dir}/adjacent_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/adjacent_resque_out.txt
fi


# test resque disjoint

echo -n "TEST: Resque Disjoint --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_disjoint 9 9 > ${dir}/disjoint_resque_out.txt

diff ${dir}/disjoint_resque_out.txt ${dir}/disjoint_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/disjoint_resque_out.txt
fi


# test resque equals

echo -n "TEST: Resque Equals --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_equals 9 9 > ${dir}/equals_resque_out.txt

diff ${dir}/equals_resque_out.txt ${dir}/equals_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/equals_resque_out.txt
fi


# test resque dwithin

echo -n "TEST: Resque Dwithin --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_dwithin 9 9 > ${dir}/dwithin_resque_out.txt

diff ${dir}/dwithin_resque_out.txt ${dir}/dwithin_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/dwithin_resque_out.txt
fi


# test resque within

echo -n "TEST: Resque Within --- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_within 9 9 > ${dir}/within_resque_out.txt

diff ${dir}/within_resque_out.txt ${dir}/within_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/within_resque_out.txt
fi


# test resque overlaps

echo -n "TEST: Resque Overlaps--- "

cat ${dir}/new_test_1.tsv ${dir}/new_test_2.tsv | ./resque st_overlaps 9 9 > ${dir}/overlaps_resque_out.txt

diff ${dir}/overlaps_resque_out.txt ${dir}/overlaps_psql_out.txt >/dev/null 2>&1

if [ $? -ne 0 ]
then
    echo "failed."
else
    echo "passed."
    rm ${dir}/overlaps_resque_out.txt
fi

make clean
