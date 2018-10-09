#ifndef __HEADER__
#define __HEADER__

#include<iostream>
#include<iomanip>
#include<string>
#include<fstream>
#include<cstdlib>

#define SENSORPORT "/dev/ttyUSB0";

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::fixed;
using std::setprecision;
using std::showpoint;
using std::fstream;

class GGA
{
	public:
	void init();
	void getDat();
	void print();
	void rawStream();

	protected:
	ifstream inFile;
	string fileName = SENSORPORT;
	string sentence;
	double time = 0;
	double latitude = 0;
	char ns = ' ';
	double longitude = 0;
	char ew = ' ';
	int gpsQual = 0;
	int numSats = 0;
	double horDil = 0;
	double alt = 0;
	char altUnit = ' ';
	double geoSep = 0;
	char geoUnit = ' ';
	double datAge = 0;
	int refStation = 0;
	int checkSum = 0;
};

#endif
