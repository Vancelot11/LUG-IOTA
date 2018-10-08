C=gcc
CFLAGS=-Wall -g
COFLAGS=-Wall -c -g

all: CGPS

CGPS: cGpsDemo.o cGga.o
	$(C) cGpsDemo.o cGga.o -o CGPS $(CFLAGS)

cGpsDemo.o: cGpsDemo.c
	$(C) $(COFLAGS) cGpsDemo.c

cGga.o: cGga.c
	$(C) $(COFLAGS) cGga.c

clean:
	rm CGPS *.o
