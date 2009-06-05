# Makefile.
# Simple makefile for building C++/Assembler based project.
# Bob Rutsky <rutsky_v@rambler.ru>
# 05.06.2009

PROGRAM_NAME = division_test
OPTIMIZATION_MODULES = div_multiplication
OBJECT_FILES = main.o $(foreach M,$(OPTIMIZATION_MODULES),$(M).o)
CC = g++

MAIN_LIB_COMPILER_OPTIONS =

MAIN_OPTIMIZATION_OPTIONS = -Wall -O3 -g
MAIN_COMPILER_OPTIONS = -c $(MAIN_OPTIMIZATION_OPTIONS) $(MAIN_LIB_COMPILER_OPTIONS)

#OPTIMIZATION_OPTIONS = -Wall -O2 -masm=intel -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-sse5 -mno-3dnow
OPTIMIZATION_OPTIONS = -Wall -O2 -masm=intel
LIB_COMPILER_OPTIONS = 
COMPILER_OPTIONS = -c $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)
ASSEMBLER_OPTIONS = -S $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)

LIB_LINK_OPTIONS =

LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

%.o: %.cpp div_algs.h
	$(CC) $(ASSEMBLER_OPTIONS) $(basename $@).cpp
	$(CC) $(COMPILER_OPTIONS) $(basename $@).cpp

clean:
	rm -f *.o
	rm -f $(foreach M,$(OPTIMIZATION_MODULES),$(M).s)
	rm -f *~ 
	rm -f $(PROGRAM_NAME)

main.o: main.cpp div_algs.h
	$(CC) $(MAIN_COMPILER_OPTIONS) $<
