#include"serialDHT.h"

const int MAXTIMINGS = 85;

void sig_handler(int sig);
bool interuptReceived = false;

int dhtDat[5] = {0,0,0,0,0};

int main()
{
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if(sigaction(SIGINT, &sig_struct, NULL) == -1)
	{
		cout << "Problem with sigaction" << endl;
		exit(1);
	}

	float far;
	GPIO dhtPin(SERIALPIN, OUTPUT);
	clock_t begin, dif;

	usleep(10000);
	dhtPin.setValue(LOW);
	usleep(18000);
	dhtPin.setValue(HIGH);
	usleep(30);
	dhtPin.setDirection(INPUT);
	begin = clock();
	int lastState;
	lastState = dhtPin.getValue();

	cout << "lastState " << lastState << endl;
	for(;;)
	{
		if(dhtPin.getValue() != lastState)
		{
			dif = clock() - begin;
			begin = clock();
			lastState = dhtPin.getValue();
			if(lastState == 0)
			{

				cout <<  lastState << " time: " << (((float)dif)/(CLOCKS_PER_SEC)) * 1000000 << endl;
			}
		}
		/*
		while(dhtPin.getValue() != LOW){}
		while(dhtPin.getValue() != HIGH)
		{
			cout << "Start response received\n";
		}
		*/
			
		if(interuptReceived)
		{
			cout << "SIGINT received. Unexporting pins.\n";
			dhtPin.~GPIO();
			break;
		}
	}

	return 0;
}

void sig_handler(int sig)
{
	write(0, "SIGINT received", 32);
	interuptReceived = true;
}
