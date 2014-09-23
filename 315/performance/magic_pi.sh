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
   GOODFILE=0
   FILEFOUND=0
   sed -i -e "s/PROGRAM/$FILE/" -e "s/OS/Pi/" output.html
   for op in 0 1 2 3
   do
      FILEFOUND=0
      for file in $(ls *.c)
      do
         if [ $FILEFOUND -eq 0 ]
         then
         gcc -O$op -o $FILE-O$op $file &> idontcare
         if [ $? -eq 0 ]
         then
            GOODFILE=$file
            FILEFOUND=1
            echo $FILE-O$op
            rpistat $FILE-O$op
            output=$(cat rpistat.txt | grep -E 'Cycles|Instruction' | sed 's/.*: \([0-9]*\).*/\1/')
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
         fi
      done
   done
   for op in 0 1 2 3
   do
      if [ $op -eq $LOWESTOPT ]
      then
         sed -i -e "s/O${op}FASTEST/main/" output.html
        
         gcc -pg -O$op -o $FILE-prof $GOODFILE &> idontcare
         echo Profiling $FILE-prof
         $FILE-prof
         HIGHEST=$(gprof $FILE-prof | grep "time   seconds" -A1)
         HIGHEST=${HIGHEST##*0}

         sed -i -e "s/main/$HIGHEST/" output.html
      else
         sed -i -e "s/O${op}FASTEST//" output.html
      fi
   done
   cp output.html ../$FILE.html
   TABLES="$TABLES $FILE.html"
   cd ..
done
cat head.html $TABLES tail.html > document.html
