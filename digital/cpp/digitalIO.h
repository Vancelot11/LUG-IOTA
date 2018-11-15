#include<iostream>
#include<fstream>
#include<string>
#include<unistd.h>
#include<signal.h>

using namespace std;

const int INPUTPIN = 19;
const int OUTPUTPIN = 26;
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
		fstream gpioDirection;
		fstream gpioValue;
		filebuf* valBuf;
		filebuf* dirBuf;

	public:
		void setDirection(bool);
		int getValue();
		void setValue(bool);
		void readDHT11();

		GPIO(int, bool); // INPUT=true OUTPUT=false
		~GPIO();
};

void GPIO::setDirection(bool dir)
{
	if(dir == INPUT)
	{
		dirBuf->sputn("in",5);
	}
	else 
	{
		dirBuf->sputn("out",5);
	}
	dirBuf->pubsync();
}

int GPIO::getValue()
{
	valBuf->pubsync();
	valBuf->pubseekpos(0);
	return valBuf->sgetc() - '0';
}

void GPIO::setValue(bool val)
{
	valBuf->pubseekpos(0);
	if(val)
	{
		valBuf->sputc('1');
	}
	else
	{
		valBuf->sputc('0');
	}	
	valBuf->pubsync();
}

GPIO::GPIO(int pinNumber, bool direction)
{
	char *buffer = new char[5];
	ofstream exportFile;

	pin = pinNumber;
	gpioFolderPath.append(to_string(pin));
	gpioDirectionPath = gpioFolderPath;
	gpioDirectionPath.append("/direction");
	gpioValuePath = gpioFolderPath;
	gpioValuePath.append("/value");


	exportFile.open(GPIOEXPORTPATH);
	if(!exportFile.is_open())
	{
		cout << "EXPORT FAILED unable to open export path.\n" << endl;
	}
	exportFile << pin;
	exportFile.close();
	sleep(1);

	gpioDirection.open(gpioDirectionPath);
	if(!gpioDirection.is_open())
	{
		cout << "DIRECTION OPEN FAILED unable to open direction file.\n";
	}
	else
	{
		cout << "Direction opened successfully.\n";
	}
	dirBuf = gpioDirection.rdbuf();
	switch(direction)
	{
		case INPUT:
			dirBuf->sputn("in",5);
			break;
		case OUTPUT:
			dirBuf->sputn("out",5);
			break;
	}
	dirBuf->pubsync();
	dirBuf->pubseekpos(0);
	cout << "GPIO Direction: " << dirBuf->sgetn(buffer, 5) << endl;

	gpioValue.open(gpioValuePath);
	if(!gpioValue.is_open())
	{
		cout << "VALUE OPEN FAILED unable to open value file.\n";
	}
	else
	{
		cout << "Value opened successfully.\n";
	}
	valBuf = gpioValue.rdbuf();
	valBuf->pubsync();
	valBuf->pubseekpos(0);
	cout << "GPIO Value: " << valBuf->sgetc() - '0' << endl;
}

GPIO::~GPIO()
{
	ofstream unexportFile(GPIOUNEXPORTPATH);

	gpioDirection.close();
	gpioValue.close();
	unexportFile << pin;
	unexportFile.close();
}
