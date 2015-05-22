PROBLEM = andrew

TEMPLATE = main
# or
MODULES =

# accepted values: d r
COMPILE_MODE := d

INPUTS := $(sort $(wildcard tests/*.in))

# --- end of usual config ---

TEMPLATES_DIR := ~/Dropbox/coding/templates

T_empty := wtfpl
T_main := wtfpl header intmain return0
T_ftxt := wtfpl header intmain ftxt return0
T_finout := wtfpl header intmain finout return0
T_multi := wtfpl header solve intmain multi return0

OUTPUTS := $(INPUTS:.in=.out)
ERRS := $(INPUTS:.in=.err)

SOURCE := $(PROBLEM).cc
TARGET := $(PROBLEM).$(COMPILE_MODE)
TARGET_D := $(PROBLEM).d

CC := g++
FLAGS_COMMON := -Wall -Wextra --std=gnu++11 -Dmoskupols -fstack-protector -Wno-char-subscripts -pthread
FLAGS_D := $(FLAGS_COMMON) -O0 -ggdb
FLAGS_R := $(FLAGS_COMMON) -O2

empty =
SPACE = $(empty) $(empty)

default: $(TARGET)

tests: $(OUTPUTS)
	@echo all tests are ready

view-tests:
	cd tests && vim -p $(notdir $(INPUTS)) -c '$(patsubst %.out,vs %.err | tabn |, $(notdir $(OUTPUTS))) $(patsubst %.out,sp %.out | tabn | , $(notdir $(OUTPUTS)))'

run: $(TARGET)
	./$(TARGET)

gdb: $(TARGET_D)
	gdb -q $(TARGET_D)

source:
	if ! [ -w $(SOURCE) ] ; then cat $(patsubst %,$(TEMPLATES_DIR)/%.mod,$(MODULES) $(T_$(TEMPLATE))) >$(SOURCE); fi
	@vim ${PROBLEM}.cc

clean:
	rm -f *.d *.r $(OUTPUTS) $(ERRS)

%.d: %.cc
	$(CC) $(FLAGS_D) -o $@ $<

%.r: %.cc
	$(CC) $(FLAGS_R) -o $@ $<

%.out: %.in $(TARGET)
	./$(TARGET) <$< >$@ 2>$(@:.out=.err)

.PHONY : view-tests run gdb source clean

