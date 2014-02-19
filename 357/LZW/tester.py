import os, sys
import difflib
import webbrowser
import subprocess
import re

setup = False
makeMakefile = False

if not os.path.exists("Testfile") or "setup" in sys.argv:
   setup = True
   makeMakefile = True

if setup:
   newTestfile = open("Testfile","w")
   print "Welcome to Thomas Steinke's super cool Testing Script!\n"

   scripts = os.popen("ls").readlines()
   if os.path.exists("tests"):
      new = os.popen("ls tests").readlines()
      for script in new:
         scripts.append("tests/"+script)

   print "What's the name of your program?"
   progname = raw_input()
   newTestfile.write(progname+"\n")

   print "Does this program pipe its input or use command line args?"

   compileList = []
   i = 0
   while i < len(scripts):
      if re.match("[test\/]+test[0-9]+.in$",scripts[i]):
         scripts[i] = scripts[i][:len(scripts[i])-1]
         i += 1
      elif re.match("[a-zA-Z0-9]+.c$",scripts[i]):
         compileList.append(scripts[i][:len(scripts[i])-1])
         scripts.remove(scripts[i])
      else:
         scripts.remove(scripts[i])

   print "First off, are these your testing scripts?"
   for script in scripts:
      print script

   print "\nIf there is a file missing, type it's relative path here"
   print "or type -<filename> to take it off this list (press Enter to confirm list)"

   response = raw_input()
   while response:
      response = response.split(" ")
      for filename in response:
         if filename[0] == "-":
            filename = filename[1:]
            if filename in scripts:
               scripts.remove(filename)
               print filename,"removed from tests list."
            else:
               print filename,"not found as a test file."
         else:
            if os.path.exists(filename):
               scripts.add(filename)
            else:
               print filename,"does not exist."

      response = raw_input()


   for line in scripts:
      newTestfile.write(line+"\n")
   newTestfile.close()

   if makeMakefile:
      print "\nGreat! Now enter the c files you need to include. I will"
      print "automatically create a Makefile for you!"
      print "Again, type -<filename to take one off the list (Press Enter to confirm list)"
      for script in compileList:
         print script
      
      response = raw_input()
      while response:
         response = response.split(" ")
         for filename in response:
            if filename == "-":
               if filename in compileList:
                  compileList.remove(filename)
                  print filename,"removed from compiles list"
               else:
                  print filename,"not found in compiles list"
            else:
               compileList.add(filename)
         print "Any more? (Press Enter to confirm list, -)"
         response = raw_input()


      # Create Makefile
      makefile = open("Makefile","w")
      makefile.write("# Autmatically generated Makefile\n")
      makefile.write("# tester.py by Thomas Steinke\n")
      makefile.write("\nCPP = gcc\n")
      makefile.write("CPPFLAGS = -O3\n")

      fileString = ""
      for script in compileList:
         fileString += script[:len(script)-1]+"o "

      makefile.write("\nCMPFILES = "+fileString) 
      makefile.write("\nCLEARFILES = "+fileString+"\n\n")

      makefile.write(progname + ": $(CMPFILES)\n")
      makefile.write("\t$(CPP) $(CPPFLAGS) $(CMPFILES) -o "+progname+"\n\n")

      makefile.write("debug: $(CMPFILES)\n")
      makefile.write("\t$(CPP) -g $(CPPFLAGS) $(CMPFILES) -o "+progname+"\n\n")

      makefile.write("clear:\n")
      makefile.write("\trm $(CLEARFILES)\n\n")

      makefile.write("%.o: %.c\n")
      makefile.write("\t$(CPP) $(CPPFLAGS) -c $< -o $@\n\n")

      for script in compileList:
         makefile.write(script+": ")
         thisFile = open(script,"r")
         for line in thisFile.readlines():
            if re.match("#include \"",line):
               makefile.write(line[10:len(line)-2]+" ")
         makefile.write("\n\ttouch "+script+"\n\n")

      print "Makefile created!\n\n"
      # End create Makefile
      makefile.close()

# START THE REAL PROGRAM
print "Running make"
res = os.system("make")

if res > 0:
   print "Fix your program first doofus"
else:
   differ = difflib.HtmlDiff()

   testfile = open("Testfile","r")
   lines = testfile.readlines()
   progname = lines[0][:len(lines[0])-1]

   piped = True
   if lines[1] == "Arguments\n":
      piped = False

   lines = lines[2:]

   for line in lines:
      line = line[:len(line)-1]
      
      print "\nTesting ",line
      
      head = "./"
      inTestDir = False
      if line[:6] == "tests/":
         line = line[6:]
         os.chdir("tests")
         inTestDir = True
         head = "./../"

      if piped:
         print head+progname+" < "+line + " > "+ line[:len(line)-2]+"res"
         subprocess.Popen(head+progname+" < "+line + " > "+ line[:len(line)-2]+"res",shell=True)
      else:
         print head+progname+" "+line+" > "+line[:len(line)-2]+"res"
         subprocess.Popen(head+progname+" "+line+" > "+line[:len(line)-2]+"res",shell=True)

      resultFile = open(line[:len(line)-2]+"res","U")
      lines = resultFile.readlines()
      correctFile = open(line[:len(line)-2]+"out","U")
      table = differ.make_file(correctFile.readlines(), lines, "CORRECT", "YOU")

      pwd = os.popen("pwd")
      pwd = pwd.read()
      pwd = "file://"+pwd[:len(pwd)-1]+"/"
      print pwd+line[:len(line)-2]+"html"

      if len(table) > 0:
         newFile = open(line[:len(line)-2]+"html","w")
         newFile.write(table)
         newFile.close()
         webbrowser.open(pwd+line[:len(line)-2]+"html")

      if inTestDir:
         os.chdir("..")