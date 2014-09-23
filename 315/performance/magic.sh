#!/bin/bash

# Make templates
echo "<html><body>" > head.html
echo "</body></html>" > tail.html

# Set up list of programs to empty
TABLES=''

for directory in $(ls -d */)
do
   cd $directory

   # Copy template file to directory
   cp ../template.html output.html
   echo -Compiling $directory

   # FILE = directory minus the '/' at the end
   FILE=${directory%?}

   # Flags for analysis
   LOWESTCYCLE=0
   LOWESTOPT=0
   GOODFILE=0
   FILEFOUND=0

   # Edit table header
   sed -i -e "s/PROGRAM/$FILE/" -e "s/OS/x86_64/" output.html
   
   # For compiler options O0, O1, O2, O3
   for op in 0 1 2 3
   do
      # Haven't found the correct c file yet
      FILEFOUND=0

      #Cycle through all .c files
      for CFILE in $(ls *.c)
      do
         # Only compile it if we haven't succeeded yet
         if [ $FILEFOUND -eq 0 ]
         then
   
            gcc -O$op -o $FILE-O$op $CFILE &> idontcare

            # Success?
            if [ $? -eq 0 ]
            then
               # Update flags
               GOODFILE=$CFILE
               FILEFOUND=1

               echo $FILE-O$op

               # Compute output and cut it to be two lines:
               # (# cycles)
               # (# instructions)
               OUTPUT=$(perf stat $FILE-O$op 2>&1 /dev/null | grep -E '[0-9] cycles|instruction' | sed 's/[\t ]*\([0-9,]*\).*/\1/')
               CYCLES=$(echo $OUTPUT | cut -f1 -d\ | sed 's/,//g')
               INSNS=$(echo $OUTPUT | cut -f2 -d\ | sed 's/,//g')
               RUNTIME=$((time $FILE-O$op) 2>&1 | grep real | sed -e 's/.*0m\([.0-9]*\)s/\1/' )

               # Insert values into table
               sed -i -e "s/O${op}CYCLE/$CYCLES/" -e "s/O${op}INSNS/$INSNS/" output.html
               sed -i -e "s/O${op}RUNTIME/$RUNTIME/" output.html

               # Update flags it is the fastest
               if [ $LOWESTCYCLE -eq 0 ] || [ $CYCLES -lt $LOWESTCYCLE ]
               then
                  LOWESTCYCLE=$CYCLES
                  LOWESTOPT=$op
               fi
            fi
         fi
      done
   done

   # Populate the fastest function row
   for op in 0 1 2 3
   do
      if [ $op -eq $LOWESTOPT ]
      then
         # Put main as the default (It doesnt show up in gprof)
         sed -i -e "s/O${op}FASTEST/main/" output.html
         
         gcc -pg -O$op -o $FILE-prof $GOODFILE &> idontcare
         echo Profiling $FILE-prof

         # Run it before we profile it
         $FILE-prof &> idontcare
         HIGHEST=$(gprof $FILE-prof | grep "time   seconds" -A1)
         HIGHEST=${HIGHEST##*0}

         # Put the function in the table
         sed -i -e "s/main/$HIGHEST/" output.html
      else
         # Remove O#FASTEST
         sed -i -e "s/O${op}FASTEST//" output.html
      fi
   done
   cp output.html ../$FILE.html

   # Add it to the list of tables
   TABLES="$TABLES $FILE.html"
   cd ..
done

# Compile them all together into a final file
cat head.html $TABLES tail.html > document.html
