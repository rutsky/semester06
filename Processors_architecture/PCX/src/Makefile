# Makefile.
# Simple makefile for building C++/Assembler based project.
# Vladimir Rutsky <altsysrq@gmail.com>
# 03.06.2009

PROGRAM_NAME = pcx_decode
OPTIMIZATION_MODULES = \
  pcx_decode_01  pcx_decode_02  pcx_decode_03  pcx_decode_04 \
  pcx_decode_05  pcx_decode_06  pcx_decode_07  pcx_decode_08 \
  pcx_decode_09  pcx_decode_09a pcx_decode_10  pcx_decode_11 \
  pcx_decode_11a pcx_decode_11b pcx_decode_12  pcx_decode_13 \
  pcx_decode_13a pcx_decode_14  pcx_decode_15
OBJECT_FILES = main.o pcx_decode.o $(foreach M,$(OPTIMIZATION_MODULES),$(M).o)
CC = g++
AS = as

TEMPFILE = .tempfile

# TODO: On Windows platform SDL stuff not working. #LINUX-DEPENDENT
MAIN_LIB_COMPILER_OPTIONS = $(shell sdl-config --cflags) 
#MAIN_LIB_COMPILER_OPTIONS =

MAIN_OPTIMIZATION_OPTIONS = -Wall -O3 
MAIN_COMPILER_OPTIONS = $(MAIN_OPTIMIZATION_OPTIONS) $(MAIN_LIB_COMPILER_OPTIONS)

#OPTIMIZATION_OPTIONS = -Wall -O2 -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-sse5 -mno-3dnow
OPTIMIZATION_OPTIONS = -Wall -O2 -mmmx -msse
LIB_COMPILER_OPTIONS = 
COMPILER_OPTIONS = -fverbose-asm -masm=intel $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)
ASSEMBLER_OPTIONS = 

#LINUX-DEPENDENT
LIB_LINK_OPTIONS = $(shell sdl-config --libs)
#LIB_LINK_OPTIONS =

LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

%.o: %.cpp pcx.h
	$(CC) -S -g $(COMPILER_OPTIONS) $(basename $@).cpp -o $(basename $@).s
	$(CC) -c  $(ASSEMBLER_OPTIONS) $(basename $@).s -o $(basename $@).o
	@#LINUX-DEPENDENT
	$(AS) -alhndm $(basename $@).s -o /dev/null > $(basename $@).lst
	@#$(AS) -alhndm $(basename $@).s -o $(TEMPFILE) > $(basename $@).lst

clean:
	rm -f $(OBJECT_FILES)
	rm -f $(foreach M,$(OPTIMIZATION_MODULES),$(M).s)
	rm -f $(foreach M,$(OPTIMIZATION_MODULES),$(M).lst)
	rm -f *~
	rm -f $(PROGRAM_NAME)
	rm -f $(TEMPFILE)

main.o: main.cpp pcx.h mtimer.hpp
	$(CC) -c $(MAIN_COMPILER_OPTIONS) $<

pcx_decode.o: pcx_decode.cpp pcx.h
	$(CC) -c $(MAIN_COMPILER_OPTIONS) $<
