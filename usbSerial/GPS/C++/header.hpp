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
