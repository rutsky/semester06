# Makefile.
# Building assembly hacks.
# Vladimir Rutsky <altsysrq@gmail.com>
# 15.03.2009

FASMARM     = fasmarm
ARM_DISASS  = ../arm_disass/arm_disass
ASM_TARGETS = jump func nop version_hook fw_version_inj \
  resident_return resident_draw resident_text resident_clear_screen \
  fw_clear_screen resident_draw2 resident_wait_key resident_direct_draw \
  resident_random_points resident_random_rectangles resident_random_rectangles

BIN_TARGETS := $(foreach A,$(ASM_TARGETS),$(A).bin)

asm: $(BIN_TARGETS)

firmware: asm
	./build_fw.sh

all: asm

%.bin: %.S resident_common.S
	$(FASMARM) $(basename $@).S $(basename $@).bin
	$(ARM_DISASS) $(basename $@).bin > $(basename $@).disasm.S

resident_random_points.S: resident_common.S resident_random.S
resident_random_rectangles.S: resident_common.S resident_random.S resident_division.S

unlock:
	rm -rf .lock

clean:
	rm -rf $(BIN_TARGETS)
	rm -rf $(foreach A,$(ASM_TARGETS),$(A).disasm.S)
