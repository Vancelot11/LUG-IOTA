#include"serialDHT11.h"

void sig_handler(int sig);
bool interuptReceived = false;

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

	GPIO dht(INPUTPIN, OUTPUT);
	string buffer;
	bool sensorRes;

	for(;;)
	{
		dht.setDirection(OUTPUT);
		dht.setValue(HIGH);
		usleep(20000);
		dht.setValue(LOW);
		usleep(20000);
		dht.setValue(HIGH);
		usleep(20);
		dht.setDirection(INPUT);
		sensorRes = false;
		while(!sensorRes)
		{
			if(dht.getValue())
			{
				
			}
		}
		usleep(80000);
		cout << "Data: " << buffer << endl;

		if(interuptReceived)
		{
			cout << "SIGINT received. Unexporting pins.\n";
			dht.~GPIO();
			cout << "Deallocating GPIO objects.\n";
			break;
		}
	}

	cout << "Exiting...\n";
	return 0;
}

void sig_handler(int sig)
{
	write(0, "SIGINT received", 32);
	interuptReceived = true;
}
