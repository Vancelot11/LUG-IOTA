#include"header.hpp"


int main()
{
	GGA GGADat;

//	system("stty -F /dev/ttyUSB0 ignbrk icrnl ixon ispeed 4800");

	GGADat.init();

	for(;;)
	{
		GGADat.rawStream();
//		GGADat.getDat();
		GGADat.print();
	}

	return 0;
}

