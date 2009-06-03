# Makefile.
# Simple makefile for building C++/Assembler based project.
# Bob Rutsky <rutsky_v@rambler.ru>
# 05.03.2005

PROGRAM_NAME = pcx_decode
OBJECT_FILES = main.o pcx_decode.o \
  pcx_decode_1.o  pcx_decode_2.o  pcx_decode_3.o  pcx_decode_4.o \
  pcx_decode_5.o  pcx_decode_6.o  pcx_decode_7.o  pcx_decode_8.o \
  pcx_decode_9.o  pcx_decode_10.o pcx_decode_11.o pcx_decode_12.o
CC = g++

MAIN_LIB_COMPILER_OPTIONS = $(shell sdl-config --cflags)
MAIN_OPTIMIZATION_OPTIONS = -Wall -O3 
MAIN_COMPILER_OPTIONS = -c $(MAIN_OPTIMIZATION_OPTIONS) $(MAIN_LIB_COMPILER_OPTIONS)

OPTIMIZATION_OPTIONS = -Wall -O2 -masm=intel -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-sse5 -mno-3dnow
LIB_COMPILER_OPTIONS = 
COMPILER_OPTIONS = -c $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)
ASSEMBLER_OPTIONS = -S $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)

LIB_LINK_OPTIONS = $(shell sdl-config --libs)
LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

.cpp.o:
	$(CC) $(ASSEMBLER_OPTIONS) $<
	$(CC) $(COMPILER_OPTIONS) $<

clean:
	rm -f *.o
	rm -f *~ 
	rm -f $(PROGRAM_NAME)

main.o: main.cpp pcx.h mtimer.hpp
	$(CC) $(MAIN_COMPILER_OPTIONS) $<

pcx_decode.o: pcx_decode.cpp pcx.h

pcx_decode_1.o : pcx_decode_1.cpp  pcx.h
pcx_decode_2.o : pcx_decode_2.cpp  pcx.h
pcx_decode_3.o : pcx_decode_3.cpp  pcx.h
pcx_decode_4.o : pcx_decode_4.cpp  pcx.h
pcx_decode_5.o : pcx_decode_5.cpp  pcx.h
pcx_decode_6.o : pcx_decode_6.cpp  pcx.h
pcx_decode_7.o : pcx_decode_7.cpp  pcx.h
pcx_decode_8.o : pcx_decode_8.cpp  pcx.h
pcx_decode_9.o : pcx_decode_9.cpp  pcx.h
pcx_decode_10.o: pcx_decode_10.cpp pcx.h
