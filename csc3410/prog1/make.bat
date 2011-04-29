@echo off
del *.exe
del *.ilk
del *.pdb

PATH = PATH;"G:/CSC 3410/masm"
ml -Zi -c -coff -Fl distance.asm
link distance.obj macros/round.obj macros/sqrt.obj macros/io.obj lib/kernel32.lib /debug /out:distance.exe /subsystem:console /entry:start
distance <input.txt