#!/bin/sh

TEST_DIR=/home/phatalsk/www/225/Assignments/Lab6
EXEC_NAME=a.out

for base in `ls $TEST_DIR/*.in | sed -e sX.*/XX | sed -e s/\\.in//`; do
    echo Evaluating test case $base...
    cp -f $TEST_DIR/$base.in $TEST_DIR/$base.out .
    ./$EXEC_NAME < $base.in > my_$base.out
    diff -b $base.out my_$base.out
    echo ------------------------------------------
done
