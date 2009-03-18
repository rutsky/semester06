# Makefile.
# Building assembly hacks.
# Vladimir Rutsky <altsysrq@gmail.com>
# 15.03.2009

FASMARM=fasmarm
ARM_DISASS=../arm_disass/arm_disass

all: jump.bin func.bin nop.bin version_hook.bin fw_version_inj.bin
	./build_fw.sh

jump.bin: jump.asm
	$(FASMARM) jump.asm jump.bin
	$(ARM_DISASS) jump.bin > jump.disasm

func.bin: func.asm
	$(FASMARM) func.asm func.bin
	$(ARM_DISASS) func.bin > func.disasm

nop.bin: nop.asm
	$(FASMARM) nop.asm nop.bin
	$(ARM_DISASS) nop.bin > nop.disasm

version_hook.bin: version_hook.asm
	$(FASMARM) version_hook.asm version_hook.bin
	$(ARM_DISASS) version_hook.bin > version_hook.disasm

fw_version_inj.bin: fw_version_inj.asm
	$(FASMARM) fw_version_inj.asm fw_version_inj.bin
	$(ARM_DISASS) fw_version_inj.bin > fw_version_inj.disasm

unlock:
	rm -rf .lock

clean:
	rm -rf jump.bin
	rm -rf jump.disasm
	rm -rf func.bin
	rm -rf func.disasm
	rm -rf nop.bin
	rm -rf nop.disasm
	rm -rf version_hook.bin
	rm -rf version_hook.disasm
	rm -rf fw_version_inj.bin
	rm -rf fw_version_inj.disasm
