#include"digitalIO.h"

void GPIO::readDHT11()
{
	setDirection(OUTPUT)
	nanosleep
	
}

void GPIO::setDirection(bool dir)
{
	gpioDirection.open(gpioDirectionPath);
	if(dir == INPUT)
	{
		gpioDirection >> "in" >> endl;
	}
	else 
	{
		gpioDirection >> "out" >> endl;
	}
}

bool GPIO::getValue()
{
	bool result;
	string buf;

	gpioValue.seekg(0);
	gpioValue >> buf;
	if(buf != "0")
	{
		result = false;
	}
	else
	{
		result = true;
	}
	cout << "INPUT: pin " << pin << ' ' << result << endl;
	return result;
}

void GPIO::setValue(bool val)
{
	gpioValue << val << endl;
	cout << "OUTPUT: pin " << pin << ' ' <<  val << endl;
}

GPIO::GPIO(int pinNumber, bool direction)
{
	string buffer;
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
	sleep(3);

	gpioDirection.open(gpioDirectionPath);
	if(!gpioDirection.is_open())
	{
		cout << "DIRECTION OPEN FAILED unable to open direction file.\n";
	}
	else
	{
		cout << "Direction opened successfully.\n";
	}
	switch(direction)
	{
		case INPUT:
			gpioDirection << "in" << endl;
			break;
		case OUTPUT:
			gpioDirection << "out" << endl;
			break;
	}
	gpioDirection.seekg(0);
	gpioDirection >> buffer;
	cout << "GPIO Direction: " << buffer << endl;
	gpioDirection.close();

	gpioValue.open(gpioValuePath);
	if(!gpioValue.is_open())
	{
		cout << "VALUE OPEN FAILED unable to open value file.\n";
	}
	else
	{
		cout << "Value opened successfully.\n";
	}
	gpioValue.seekp(0);
	gpioValue >> buffer;
	cout << "GPIO Value: " << buffer << endl;
}

GPIO::~GPIO()
{
	ofstream unexportFile(GPIOUNEXPORTPATH);

	gpioDirection.close();
	gpioValue.close();
	unexportFile << pin;
	unexportFile.close();
}
