

# path should be relative to where this makefile is (repo root)
VER_CPP_INCLUDE_PATH=inc/

VER_HDL_PATH=./hdl

VER_INCLUDE_DIRS=\
-I.

VER_TOP=--top-module $(HDL_TB_TOP) +define+VERILATE +define+VERILATE_DEF


VER_SOURCES=$(ALL_VERILOG)


EXTRA_VERILATOR_ARGS?=""


THIS_FILE=Makefile
CPP_TB_FILES= tb.cpp
CPP_TB_FILES+= $(EXTRA_CPP_FILES)



# this path is relative to where this makefile is because it gets expanded here and
# passed to verilator
CPP_TB_FILES+= $(wildcard $(VER_CPP_INCLUDE_PATH)/*.cpp)

VER_BINARY = V$(HDL_TB_TOP)


# try:
#   --trace-fst
VER_FLAGS = -Wno-PINMISSING --trace 
# VER_FLAGS = --trace  -Wall 


VER_FLAGS += -DVERILATE_DEF


ADDCFLAGS += -CFLAGS -DCS21_IS_DENGINE
ADDCFLAGS += -CFLAGS -pthread
ADDCFLAGS += -CFLAGS -g
ADDCFLAGS += -CFLAGS -fPIC
ADDCFLAGS += -CFLAGS -MMD
ADDCFLAGS += -CFLAGS -MP
ADDCFLAGS += -CFLAGS -std=c++11
ADDCFLAGS += -CFLAGS -Werror=return-type
ADDCFLAGS += -CFLAGS -Werror=overflow
ADDCFLAGS += -CFLAGS -Werror=array-bounds
# ADDCFLAGS += -CFLAGS -Wpedantic
# ADDCFLAGS += -CFLAGS -Wshadow
ADDCFLAGS += -CFLAGS -Werror=shift-count-overflow
ADDCFLAGS += -CFLAGS -Wpointer-arith
ADDCFLAGS += -CFLAGS -Wcast-qual
ADDCFLAGS += -CFLAGS -DVERILATE_TESTBENCH
ADDCFLAGS += -LDFLAGS -pthread

# path is relative to obj_dir (So some variables will have a ../ prepended)
ADDCFLAGS += -CFLAGS -I../inc/
ADDCFLAGS += -CFLAGS -I/usr/local/share/verilator/include





# Extra ld flags, however they need to be in verilator format
# EXTRA_LD_FLAGS=-LDFLAGS -lzmq

ifdef EXTRA_LD_FLAGS
ADDCFLAGS += $(EXTRA_LD_FLAGS)
endif



.PHONY: all run_only run verilate compile cleanall trun
.PHONY: pre_work 


all: cleanall verilate compile trun

# no clean prework, that compiles as much as we can without running test
pre_work: verilate compile

run_only:
	./obj_dir/$(VER_BINARY)

run: compile
	./obj_dir/$(VER_BINARY)

trun: compile
	./obj_dir/$(VER_BINARY) +trace

verilate:
	verilator $(VER_TOP) \
	$(VER_FLAGS) \
	$(VER_INCLUDE_DIRS) \
	-cc \
	$(VER_SOURCES) \
	-O3 \
	${ADDCFLAGS} \
	--gdbbt \
	${VERILATOR_ARGS} \
	-Wno-UNOPTFLAT -Wno-WIDTH \
	--x-assign unique \
	--exe \
	$(CPP_TB_FILES) \
	$(VERILATOR_C_OVERRIDE_DEFINES) \
	$(VERILATOR_TB_INCLUDE_DEFINES) \
	$(EXTRA_VERILATOR_ARGS)


compile:
	make  -j  -C obj_dir/ -f $(VER_BINARY).mk $(VER_BINARY)

.PHONY: clean show show1 quick quickt check

cleanwave:
	rm -f wave_dump.vcd wave_dump.vcd.idx wave_dump1.vcd wave_dump1.vcd.idx

cleanall:
	rm -rf obj_dir
	rm -f wave_dump.vcd wave_dump.vcd.idx

clean: cleanall



# it's possible that --giga is redundant with --fastload
# without realpath, multiple gtkwave windows will all show 'wave_dump.vcd'
# in the title, making it impossible to distinguish which window is from which folder
show: wave_dump.vcd
	gtkwave $(realpath wave_dump.vcd) --fastload dma_out.gtkw --giga &

show1: wave_dump1.vcd
	gtkwave $(realpath wave_dump1.vcd) --fastload dma_out.gtkw --giga &

quickt: compile trun
quick: compile run


foo:
	echo $(VER_BINARY)

foo2:
	echo $(ADDCFLAGS)
