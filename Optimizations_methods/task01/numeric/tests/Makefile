# Makefile.
# Based on Jaco Kroon example make file (http://www.tlug.org.za/wiki/index.php?title=Writing_a_Makefile).
# Vladimir Rutsky <altsysrq@gmail.com>
# 06.03.2008

# Define the targets.

TARGET_BINS = run_tests
MODS_run_tests = test_runner iterator_test li_vectors_test invert_matrix_test submatrix_test

LIBS_run_tests = 

# 0 == debug off, anything else switches DEBUG on
DEBUG ?= 1
STATIC ?= 0

CC := g++

CFLAGS ?= -W -Wall -std=c++98 -pedantic

CFLAGS += -I/usr/src/boost_1_38_0
LDFLAGS += 

CFLAGS += $(OPTIM)
CFLAGS += $(DEFS)

##############################################################################################
#### From here on there should be no reason to change unless you know what you are doing. ####
##############################################################################################
.PHONY: default
default: all

BIN_TARGET := $(foreach B,$(TARGET_BINS),$(B))

ALL_TARGET :=
ALL_TARGET += $(BIN_TARGET)

BINMODS := $(foreach B,$(TARGET_BINS),$(MODS_$(B)))
ALLMODS := $(BINMODS)

.PHONY: all
all: $(ALL_TARGET)
	@echo "Everything has been brought up to date"

# Delete targets if/when updating them fails.
.DELETE_ON_ERROR:

ifneq ($(DEBUG),0)
DEFS += -DDEBUG
CFLAGS += -g -ggdb
LDFLAGS += -g -ggdb
OPTIM = -O0
else
DEFS += -DNDEBUG
OPTIM = -O2
endif

ifneq ($(STATIC),0)
$(BIN_TARGET) : LDFLAGS += -static
endif

ifneq ($(OPTIM),)
CFLAGS += $(OPTIM)
endif

##########################################
# How to build actual binaries/libraries
#
$(BIN_TARGET): | build/.d
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $(foreach L,$(LIBS) $(LIBS_$(patsubst /%,%,$@)),-l$(L)) $(foreach M,$(MODS_$(notdir $(basename $@))),build/$(M).o)

##########################################
# Pattern matched build targets.
#

build/%.o : %.c Makefile
	$(CC) -x c $(CFLAGS) -o $@ -c $<

build/%.o : %.C Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.o : %.cpp Makefile
	$(CC) -x c++ $(CFLAGS) -o $@ -c $<

build/%.o : %.CPP Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.d : %.c Makefile | build/.d
	$(CC) -x c $(CFLAGS) -o $@ -MM $<
	sed -e 's,$(notdir $*.o):,$@ build/$*.o:,' -i $@

build/%.d : %.C Makefile | build/.d
	$(CC) $(CFLAGS) -o $@ -MM $<
	sed -e 's,$(notdir $*.o):,$@ build/$*.o:,' -i $@

build/%.d : %.cpp Makefile | build/.d
	$(CC) -x c++ $(CFLAGS) -o $@ -MM $<
	sed -e 's,$(notdir $*.o):,$@ build/$*.o:,' -i $@

build/%.d : %.CPP Makefile | build/.d
	$(CC) $(CFLAGS) -o $@ -MM $<
	sed -e 's,$(notdir $*.o):,$@ build/$*.o:,' -i $@

.PRECIOUS: %/.d
%/.d :
	mkdir -p $(dir $@)
	@touch $@

##########################################
# Dependancy files.  For example, we need a way to tell make
# that objects needs to be brough up to date before the binaries.
#
depfiles := build/Makefile
build/Makefile : Makefile | build/.d
	@echo "Bringing dependancy Makefile up to date..."
	@mkdir -p $(dir $@)
	@>$@
	@$(foreach B,$(TARGET_BINS),echo "$(B) : $(patsubst %,build/%.o,$(MODS_$(B)))" >> $@;)
	@$(foreach M,$(ALLMODS),echo "build/$(M).o : | $(dir build/$(M)).d" >> $@;)

depfiles += $(patsubst %,build/%.d,$(ALLMODS))
build/.d : $(filter-out build/.d,$(addsuffix .d,$(dir $(depfiles))))

.PHONY: clean
clean :
	rm -rf build
	rm -f *~

.PHONY: distclean
distclean : clean
	rm -rf $(TARGET_BINS)

.PHONY: svnignore
svnignore :
	svn propset svn:ignore "`svn propget --strict svn:ignore . | sed '/^$$/d' | sed '/^build$$/d' | tac | xargs --null printf "build\n%s" | tac | sed '/^$$/d' `" .

##########################################
# This whole thing is to determine whether we need the depfiles or not.
#
NODEP_TARGETS := clean distclean svnignore
depinc := 1
ifneq (,$(filter $(NODEP_TARGETS),$(MAKECMDGOALS)))
depinc := 0
endif
ifneq (,$(fitler-out $(NODEP_TARGETS),$(MAKECMDGOALS)))
depinc := 1
endif

ifeq ($(depinc),1)
-include $(depfiles)
endif

# END OF 'Makefile' FILE
