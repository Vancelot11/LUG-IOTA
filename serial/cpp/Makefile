CFLAGS = -Wall -g -o serialDHT
COFLAGS = -Wall -g -c

all: program

program: serialDHT.o GPIO.o
	g++ serialDHT.o GPIO.o $(CFLAGS)

serialDHT.o: serialDHT.cpp
	g++ serialDHT.cpp $(COFLAGS)

GPIO.o: GPIO.cpp
	g++ GPIO.cpp $(COFLAGS)

clean:
	rm serialDHT *.o
