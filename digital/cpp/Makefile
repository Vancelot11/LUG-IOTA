CFLAGS = -Wall -g -o digitalIO

all: program

program: digitalIO.cpp GPIO.cpp
	g++ digitalIO.cpp GPIO.cpp $(CFLAGS)

clean:
	rm digitalIO
