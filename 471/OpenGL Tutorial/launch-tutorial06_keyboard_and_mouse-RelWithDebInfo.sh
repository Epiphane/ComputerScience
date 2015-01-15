#!/bin/sh
bindir=$(pwd)
cd /Users/thomassteinke/Downloads/OpenGL-tutorial_v0014_21/tutorial06_keyboard_and_mouse/
export DYLD_LIBRARY_PATH=:$DYLD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/RelWithDebInfo/tutorial06_keyboard_and_mouse 
	else
		"/Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/RelWithDebInfo/tutorial06_keyboard_and_mouse"  
	fi
else
	"/Users/thomassteinke/Code/ComputerScience/471/OpenGL\ Tutorial/RelWithDebInfo/tutorial06_keyboard_and_mouse"  
fi
