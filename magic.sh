#!/bin/bash
TABLES=''
for directory in $(ls -d */)
do
   cd $directory
   cp ../template.html output.html
   echo -Compiling $directory
   FILE=${directory%?}
   LOWESTCYCLE=0
   LOWESTOPT=0
   sed -i -e "s/PROGRAM/$FILE/" -e "s/OS/x86_64/" output.html
   for op in 0 1 2 3
   do
      for file in $(ls *.c)
      do
         gcc -O$op -o $FILE-O$op $file &> idontcare
         if [ $? -eq 0 ]
         then
            echo $FILE-O$op
            output=$(perf stat $FILE-O$op 2>&1 /dev/null | grep -E '[0-9] cycles|instruction' | sed 's/[\t ]*\([0-9,]*\).*/\1/')
            cycles=$(echo $output | cut -f1 -d\ | sed 's/,//g')
            insns=$(echo $output | cut -f2 -d\ | sed 's/,//g')
            runtime=$((time aes/a.out) 2>&1 | grep real | sed -e 's/.*0m\([.0-9]*\)s/\1/' )

            sed -i -e "s/O${op}CYCLE/$cycles/" -e "s/O${op}INSNS/$insns/" -e "s/O${op}RUNTIME/$runtime/" output.html
            if [ $LOWESTCYCLE -eq 0 ] || [ $cycles -lt $LOWESTCYCLE ]
            then
               LOWESTCYCLE=$cycles
               LOWESTOPT=$op
            fi
         fi
      done
   done
   for op in 0 1 2 3
   do
      if [ $op -eq $LOWESTOPT ]
      then
         for file in $(ls *.c)
         do
            gcc -pg -O$op -o $FILE-prof $file &> idontcare
               if [ $? -eq 0 ]
               then
               fi
            done
         fi
         echo Profiling $FILE-prof
         $FILE-prof
         HIGHEST=$(gprof $FILE-prof | )

         sed -i -e "s/O${op}FASTEST/$FUNC/" output.html
      else
         sed -i -e "s/O${op}FASTEST//" output.html
      fi
   done
   cp output.html ../$FILE.html
   TABLES="$TABLES $FILE.html"
   cd ..
done
cat head.html $TABLES tail.html > document.html
