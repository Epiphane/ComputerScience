#!/bin/sh

TEST_DIR=/home/jworkman/www/225/Assignments/Lab8
EXEC_NAME=a.out

    cp -f $TEST_DIR/BigJim.txt .

    echo Evaluating test case t1: BigJim.txt the
    cp -f $TEST_DIR/t1.out .
    ./$EXEC_NAME BigJim.txt the > my_t1.out
    diff -b t1.out my_t1.out
    echo ------------------------------------------

    echo Evaluating test case t2: BigJim.txt Jim
    cp -f $TEST_DIR/t2.out .
    ./$EXEC_NAME BigJim.txt Jim > my_t2.out
    diff -b t2.out my_t2.out
    echo ------------------------------------------

    echo Evaluating test case t3: too few args
    cp -f $TEST_DIR/t3.out .
    ./$EXEC_NAME > my_t3.out
    diff -b t3.out my_t3.out
    echo ------------------------------------------

    echo Evaluating test case t4: too many args
    cp -f $TEST_DIR/t4.out .
    ./$EXEC_NAME BigJim.txt the hi > my_t4.out
    diff -b t4.out my_t4.out
    echo ------------------------------------------