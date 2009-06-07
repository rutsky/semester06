# Makefile.
# Simple makefile for building C++/Assembler based project.
# Vladimir Rutsky <altsysrq@gmail.com>
# 03.06.2009

PROGRAM_NAME = motion_blur
OPTIMIZATION_MODULES = \
  motion_blur
OBJECT_FILES = main.o $(foreach M,$(OPTIMIZATION_MODULES),$(M).o)
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
LIB_COMPILER_OPTIONS = $(shell sdl-config --cflags) 
COMPILER_OPTIONS = -fverbose-asm -masm=intel $(OPTIMIZATION_OPTIONS) $(LIB_COMPILER_OPTIONS)
ASSEMBLER_OPTIONS = 

#LINUX-DEPENDENT
LIB_LINK_OPTIONS = $(shell sdl-config --libs) -lSDL_image
#LIB_LINK_OPTIONS =

LINK_OPTIONS = -o $(PROGRAM_NAME) $(LIB_LINK_OPTIONS)

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(LINK_OPTIONS) $(OBJECT_FILES)

%.o: %.cpp motion_blur.h
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

main.o: main.cpp motion_blur.h
	$(CC) -c $(MAIN_COMPILER_OPTIONS) $<
