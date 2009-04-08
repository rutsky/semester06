# Makefile.
# Building assembly hacks.
# Vladimir Rutsky <altsysrq@gmail.com>
# 15.03.2009

FASMARM     = fasmarm
ARM_DISASS  = ../arm_disass/arm_disass
ASM_TARGETS = jump func nop version_hook fw_version_inj resident_return resident_draw resident_text clear_screen

BIN_TARGETS := $(foreach A,$(ASM_TARGETS),$(A).bin)

asm: $(BIN_TARGETS)

firmware: asm
	./build_fw.sh

all: asm

%.bin: %.asm
	$(FASMARM) $(basename $@).asm $(basename $@).bin
	$(ARM_DISASS) $(basename $@).bin > $(basename $@).disasm

unlock:
	rm -rf .lock

clean:
	rm -rf $(BIN_TARGETS)
	rm -rf $(foreach A,$(ASM_TARGETS),$(A).disasm)
