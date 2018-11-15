CFLAGS = -lcurl -g -Wall -o serverTest
COFLAGS = -Wall -c -g

all: dataHandler.o

dataHandler.o:
	g++ dataHandler.cpp -c $(COFLAGS) -o dataHandler.o

clean:
	rm -f *.o
