export WORKLOAD=main
TRACER_HOME=/workspace/LLVM-Tracer

SRCS = $(wildcard *.c)
EXEC = is
CC = gcc

C_INC = -I ../common
CPPFLAGS =
LOGGER = $(TRACER_HOME)/lib/trace_logger.llvm
TRACER = $(TRACER_HOME)/lib/full_trace.so

LLVM_OBJS = $(patsubst %.c, %-opt.llvm, $(SRCS))

$(EXEC)-instrumented: full.s
	$(CC) -O0 -fno-inline -mcmodel=medium -o $@ $< -lm -lz

full.s: full.llvm
	llc -O0 -disable-fp-elim -filetype=asm -o $@ $<

full.llvm: $(LLVM_OBJS)
	llvm-link -o $@ $^ $(LOGGER)

%-opt.llvm: %.c
	clang -g ${C_INC} -Wall -O0 -mcmodel=medium -S -static -emit-llvm  $(CPPFLAGS)  -o $*.llvm $<
	opt -S -load=$(TRACER) -fulltrace -trace-all-callees $*.llvm -o $@

clean:
	rm -f *.ll* *.s *.core *-instrumented *.gz

