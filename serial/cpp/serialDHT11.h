#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
#include<signal.h>

using namespace std;
/*
using std::endl;
using std::cout;
using std::ofstream;
using std::fstream;
using std::string;
using std::to_string;
*/

const int INPUTPIN = 2;
const string GPIOEXPORTPATH = "/sys/class/gpio/export";
const string GPIOUNEXPORTPATH = "/sys/class/gpio/unexport";
const string GPIOPATH = "/sys/class/gpio/gpio";
const int HIGH = 1;
const int LOW = 0;
const bool INPUT = true;
const bool OUTPUT = false;


class GPIO
{
	private:
		int pin;
		string gpioFolderPath = GPIOPATH, gpioDirectionPath, gpioValuePath;
		fstream gpioDirection, gpioValue;

	public:
		void setDirection(bool);
		int getValue();
		void setValue(bool);
		void readDHT11();

		GPIO(int, bool); // INPUT=true OUTPUT=false
		~GPIO();
};

