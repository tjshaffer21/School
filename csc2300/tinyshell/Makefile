# Makefile for the CSC 2300 Programming #4 Assignment
#
# CSC 2300
# Thomas Shaffer
# Programming Assignment #4
# Due: December 4, 2008

DRIVER = ./sdriver.pl
TSH = ./tsh
TSHREF = ./tshref
TSHARGS = "-p"

# <create a "CC" macro that uses the GNU C Compiler>
# <create a "CFLAGS" macro that specifies the "-O2" and "-Wall" compiler options>
# <create FILES macro that uses TSH macro above and specifies "myspin" program to be built>
CC=/usr/bin/gcc
CFLAGS= -O2 -Wall
FILES= $(CC) $(CFLAGS)

# <create an "all" target that uses the "FILES" macro definition you defined above>
all:
	$(CC) $(CFLAGS) -o tsh tsh.c
	$(FILES) -o myspin myspin.c
##################
# Regression tests
##################

# Run tests using the your shell program
test01:
	$(DRIVER) -t trace01.txt -s $(TSH) -a $(TSHARGS)
test02:
	$(DRIVER) -t trace02.txt -s $(TSH) -a $(TSHARGS)
test03:
	$(DRIVER) -t trace03.txt -s $(TSH) -a $(TSHARGS)
test04:
	$(DRIVER) -t trace04.txt -s $(TSH) -a $(TSHARGS)
test05:
	$(DRIVER) -t trace05.txt -s $(TSH) -a $(TSHARGS)
test06:
	$(DRIVER) -t trace06.txt -s $(TSH) -a $(TSHARGS)
test07:
	$(DRIVER) -t trace07.txt -s $(TSH) -a $(TSHARGS)
test08:
	$(DRIVER) -t trace08.txt -s $(TSH) -a $(TSHARGS)
test09:
	$(DRIVER) -t trace09.txt -s $(TSH) -a $(TSHARGS)
test10:
	$(DRIVER) -t trace10.txt -s $(TSH) -a $(TSHARGS)
test11:
	$(DRIVER) -t trace11.txt -s $(TSH) -a $(TSHARGS)

# <create a "tests" target that will run all of the above tests with one command>
# <specify on TWO lines, using the continuation marker>
tests: test01 test02 test03 test04 test05 test06 test07 \
	test08 test09 test10 test11
# Run tests using the reference shell program
rtest01:
	$(DRIVER) -t trace01.txt -s $(TSHREF) -a $(TSHARGS)
rtest02:
	$(DRIVER) -t trace02.txt -s $(TSHREF) -a $(TSHARGS)
rtest03:
	$(DRIVER) -t trace03.txt -s $(TSHREF) -a $(TSHARGS)
rtest04:
	$(DRIVER) -t trace04.txt -s $(TSHREF) -a $(TSHARGS)
rtest05:
	$(DRIVER) -t trace05.txt -s $(TSHREF) -a $(TSHARGS)
rtest06:
	$(DRIVER) -t trace06.txt -s $(TSHREF) -a $(TSHARGS)
rtest07:
	$(DRIVER) -t trace07.txt -s $(TSHREF) -a $(TSHARGS)
rtest08:
	$(DRIVER) -t trace08.txt -s $(TSHREF) -a $(TSHARGS)
rtest09:
	$(DRIVER) -t trace09.txt -s $(TSHREF) -a $(TSHARGS)
rtest10:
	$(DRIVER) -t trace10.txt -s $(TSHREF) -a $(TSHARGS)
rtest11:
	$(DRIVER) -t trace11.txt -s $(TSHREF) -a $(TSHARGS)

# <create a "rtests" target that will run all of the above ref tests with one command>
# <specify on TWO lines, using the continuation marker>
rtests: rtest01 rtest02 rtest03 rtest04 rtest05 rtest06 \
	rtest07 rtest08 rtest09 rtest10 rtest11
# <create a "clean" target that will remove your tsh executable and all .o files>
clean:
	rm -f tsh *.o
