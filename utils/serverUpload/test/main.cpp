#include"../dataHandler.h"


//Tests main.h
int main()
{
	std::cout << "starting" << std::endl;
	dataHandler dh("http://flip3.engr.oregonstate.edu:2031");
	
	for(int i = 0; i < 10; i++)
	{
		if(i % 2 == 0 )
		{
			dh.buttonValPost(true);
		}
		else
		{
			dh.buttonValPost(false);
		}
		dh.tempValPost(std::to_string(i));
		dh.knobValPost(std::to_string(i));
		dh.lightValPost(std::to_string(i));
		dh.rhValPost(std::to_string(i));
		sleep(1);
	}
}
