#!/bin/sh
bindir=$(pwd)
cd /Users/thomassteinke/Downloads/OpenGL-tutorial_v0014_21/tutorial10_transparency/
export DYLD_LIBRARY_PATH=:$DYLD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/MinSizeRel/tutorial10_transparency 
	else
		"/Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/MinSizeRel/tutorial10_transparency"  
	fi
else
	"/Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/MinSizeRel/tutorial10_transparency"  
fi