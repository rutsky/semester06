# Makefile.
# Simple makefile for building C++/Assembler based project.
# Bob Rutsky <rutsky_v@rambler.ru>
# 05.03.2005

PROGRAM_NAME = pcx_decode
OBJECT_FILES = main.o pcx_decode.o
CC = g++
LIB_COMPILER_OPTIONS =
COMPILER_OPTIONS = -c -g -Wall -O3 $(LIB_COMPILER_OPTIONS)
LIB_LINK_OPTIONS =
LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

.cpp.o:
	$(CC) $(COMPILER_OPTIONS) $<

clean:
	rm -rf *.o *~ $(PROGRAM_NAME)

main.o: main.cpp pcx.h

pcx_decode.o: pcx_decode.cpp pcx.h
