SHELL=/bin/sh
BENCHMARK=cg
BENCHMARKU=CG

include ../config/make.def

include ../sys/make.common

FTI_DIR = /usr/bin/fti
FTI_LIB = $(FTI_DIR)/lib/libfti.a
FTI_INC = -I$(FTI_DIR)/include


OBJS = cg.o \
       ${COMMON}/print_results.o  \
       ${COMMON}/${RAND}.o \
       ${COMMON}/c_timers.o \
       ${COMMON}/wtime.o


${PROGRAM}: config ${OBJS}
	${CLINK} ${CLINKFLAGS} -o ${PROGRAM} ${OBJS} ${C_LIB} ${FTI_LIB} -lz -lssl -lcrypto

.c.o:
	 ${CCOMPILE}  ${FTI_INC} $<

cg.o:		cg.c  globals.h npbparams.h
	${CCOMPILE} ${FTI_INC} $<

clean:
	- rm -f *.o *~ 
	- rm -f npbparams.h core
