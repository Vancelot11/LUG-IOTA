// Author: Kerry Vance
#include"header.hpp"

int main()
{
	GGA GGADat;

	GGADat.init();

	for(;;)
	{
		GGADat.getDat();
		GGADat.print();
	}

	return 0;
}

