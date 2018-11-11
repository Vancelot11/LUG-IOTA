#include"main.h"

int main()
{
	std::cout << "starting" << std::endl;
	dataHandler dh;
	
	for(int i = 0; i < 10; i++)
	{
		if(i % 2 == 0 )
		{
			dh.digitalValPost(true);
		}
		else
		{
			dh.digitalValPost(false);
		}
		dh.analogValPost(std::to_string(i));
		sleep(1);
	}
}
