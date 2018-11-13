#include"serialDHT.h"

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

	GPIO dhtPin(SERIALPIN, OUTPUT);
	int *result[5] = {0,0,0,0,0};
	int *lastState = new int;
	int count;

	dhtPin.setValue(LOW);
	usleep(18000);
	dhtPin.setValue(HIGH);
	dhtPin.setDirection(INPUT);
	*lastState = HIGH;
	cout << "Start signal sent.\n";

	for(int i = 0; i < 85; i++)
	{
		count = 0;
		while(dhtPin.getValue() == *lastState)
		{
			count++;
			usleep(1);
			if(count == 255)
			{
				break;
			}
		}
		*lastState = dhtPin.getValue();
		if(count == 255)
		{
			break;
		}

//		if((i >= 3))
//		{
			cout << "i: " << i << " count: " << count << endl;
//		}
			
		if(interuptReceived)
		{
			cout << "SIGINT received. Unexporting pins.\n";
			dhtPin.~GPIO();
			delete result;
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
