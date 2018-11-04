#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>


using namespace std;
/*
using std::endl;
using std::cout;
using std::ofstream;
using std::fstream;
using std::string;
using std::to_string;
*/

const string GPIOEXPORTPATH = "/sys/class/gpio/export";
const string GPIOUNEXPORTPATH = "/sys/class/gpio/unexport";
const string GPIOPATH = "/sys/class/gpio/gpio";
const bool INPUT = true;
const bool OUTPUT = false;

class GPIO
{
	private:
		int pin;
		string gpioFolderPath = GPIOPATH, gpioDirectionPath, gpioValuePath;
		fstream gpioDirection, gpioValue;

	public:
		bool setDirection(bool);
		void writeHigh();
		void writeLow();

		GPIO(int, bool); // INPUT=true OUTPUT=false
		~GPIO();
};

