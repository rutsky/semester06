# Makefile.
# Building assembly hacks.
# Vladimir Rutsky <altsysrq@gmail.com>
# 15.03.2009

FASMARM=fasmarm
ARM_DISASS=../arm_disass/arm_disass

all: jump.bin func.bin
	./build_fw.sh

jump.bin: jump.asm
	$(FASMARM) jump.asm jump.bin
	$(ARM_DISASS) jump.bin > jump.disasm

func.bin: func.asm
	$(FASMARM) func.asm func.bin
	$(ARM_DISASS) func.bin > func.disasm

unlock:
	rm -rf .lock

clean:
	rm -rf jump.bin
	rm -rf jump.disasm
	rm -rf func.bin
	rm -rf func.disasm
