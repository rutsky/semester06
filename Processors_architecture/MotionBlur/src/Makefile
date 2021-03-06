# Makefile.
# Simple makefile for building C++/Assembler based project.
# Vladimir Rutsky <altsysrq@gmail.com>
# 03.06.2009

# TODO: Never tested on Windows.

PROGRAM_NAME = motion_blur
OPTIMIZATION_MODULES = motion_blur dummy_output \
  motion_blur_01  motion_blur_02  motion_blur_03  motion_blur_04  \
  motion_blur_06
ASSEMBLER_MODULES_64_linux = \
  dummy_output_64 motion_blur_05_64 motion_blur_07_64
#ASSEMBLER_MODULES_32 = \
#  dummy_output_32 motion_blur_05_32 motion_blur_07_32
OBJECT_FILES = main.o \
  $(foreach M,$(OPTIMIZATION_MODULES),$(M).o) \
  $(foreach M,$(ASSEMBLER_MODULES_64_linux),$(M).o)
#  $(foreach M,$(ASSEMBLER_MODULES_32),$(M).o)

CC = g++
AS = as
NASM = nasm

TEMPFILE = .tempfile

# TODO: On Windows platform SDL stuff not working. #LINUX-DEPENDENT
MAIN_LIB_COMPILER_OPTIONS = $(shell sdl-config --cflags) 
#MAIN_LIB_COMPILER_OPTIONS =

MAIN_OPTIMIZATION_OPTIONS = -Wall -O3 
MAIN_COMPILER_OPTIONS = $(MAIN_OPTIMIZATION_OPTIONS) $(MAIN_LIB_COMPILER_OPTIONS)

#OPTIMIZATION_OPTIONS = -Wall -O2 -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-sse5 -mno-3dnow
OPTIMIZATION_OPTIONS = -Wall -O2 -mmmx -msse
#LINUX-DEPENDENT
LIB_COMPILER_OPTIONS = $(shell sdl-config --cflags) 
#LIB_COMPILER_OPTIONS = 
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

%.o: %.asm
	@#LINUX-DEPENDENT
	$(NASM) -f elf64 -Xgnu -g $(basename $@).asm -o $(basename $@).o
	@#LINUX-DEPENDENT
	objdump -d $(basename $@).o > $(basename $@).o.s

clean:
	rm -f $(OBJECT_FILES)
	rm -f $(foreach M,$(OPTIMIZATION_MODULES),$(M).s)
	rm -f $(foreach M,$(OPTIMIZATION_MODULES),$(M).lst)
	rm -f $(foreach M,$(ASSEMBLER_MODULES_64_linux),$(M).o.s)
	rm -f *~
	rm -f $(PROGRAM_NAME)
	rm -f $(TEMPFILE)
	
	rm -f MotionBlur.ncb
	rm -f MotionBlur.opt
	rm -f MotionBlur.plg
	rm -rf Debug/
	rm -rf Release/

main.o: main.cpp motion_blur.h
	$(CC) -c $(MAIN_COMPILER_OPTIONS) $<
