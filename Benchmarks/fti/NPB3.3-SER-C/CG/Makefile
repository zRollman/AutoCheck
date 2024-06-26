HELL=/bin/sh
BENCHMARK=cg
BENCHMARKU=CG

include ../config/make.def

# FTI configuration
FTI_DIR = /usr/bin/fti/installation
FTI_LIB = $(FTI_DIR)/lib/libfti.a
FTI_INC = -I$(FTI_DIR)/include

include ../sys/make.common

OBJS = cg.o \
       ${COMMON}/print_results.o  \
       ${COMMON}/${RAND}.o \
       ${COMMON}/c_timers.o \
       ${COMMON}/wtime.o

${PROGRAM}: config ${OBJS}
        ${CLINK} ${CLINKFLAGS} -o ${PROGRAM} ${OBJS} ${C_LIB} ${FTI_LIB}

.c.o:
	${CCOMPILE} ${FTI_INC} $<

cg.o: cg.c globals.h npbparams.h
        ${CCOMPILE} ${FTI_INC} $<

clean:
	- rm -f *.o *~ 
	- rm -f npbparams.h core


