#include"digitalIO.h"

int main()
{
	GPIO output(26, OUTPUT);
	for(int i = 0; i < 3; i++)
	{
		output.writeHigh();
		sleep(1);
		output.writeLow();
		sleep(1);
	}

	return 0;
}
