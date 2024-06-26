PROG= himeno
OBJS= himenoBMT.o
SRC= himenoBMT.c
CC= gcc
#
# small -> SMALL
# midium -> MIDDLE
# large -> LARGE
#
MODEL = SMALL

CFLAGS = -O3

all: $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

$(OBJS) : $(SRC)
	$(CC) -c $(CFLAGS) -D$(MODEL) $(SRC)

clean:
	rm -f core *~  *.o $(PROG)

