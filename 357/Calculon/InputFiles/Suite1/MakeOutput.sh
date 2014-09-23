#! /bin/sh

gcc main.c -o a.out
gcc bprogram.c SmartAlloc.c -o b.out

a.out -a -b -c < test1.in > test1.out
a.out -xyz < test2.in > test2.out
a.out < test3.in > test3.out

b.out -verbose < test4.in > test4.out
b.out < test5.in > test5.out

rm a.out
rm b.out
