CFLAGS = -lcurl -g -Wall -o serverTest
COFLAGS = -Wall -c -g

all: program

program: main.cpp ../dataHandler.o
	g++ main.cpp ../dataHandler.o $(CFLAGS)

