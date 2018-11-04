#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main()
{
	/*
	string exportPath = "/sys/class/gpio/export";
	ofstream exportGPIO(exportPath);
	exportGPIO << 25;
	*/
	string output = "out";
	string directionPath = "/sys/class/gpio/gpio25/direction";
	ofstream directionGPIO(directionPath);
	if(directionGPIO.is_open())
	{
		cout << "direction file open\n";
	}
	else
	{
		cout << "not open\n";
	}
	directionGPIO << "in";
	return 0;	
}
