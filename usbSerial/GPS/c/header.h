#ifndef __HEADER__
#define __HEADER__

#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>
#include<strings.h>
#include<string.h>
#include<unistd.h>

#define BAUDRATE B4800
#define SERIALPORT "/dev/ttyUSB0"
#define BUFSIZE 255

struct GGA
{
	int gpsFD;
	struct termios oldtio, newtio;
	char *sentence;
	double time;
	double latitude;
	char ns;
	double longitude;
	char ew;
	int gpsQual;
	int numSats;
	double horDil;
	double alt;
	char altUnit;
	double geoSep;
	char geoUnit;
	double datAge;
	int refStation;
	int checkSum;
};

#endif
