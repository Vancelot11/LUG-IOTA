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

	int lastState = HIGH;
	int counter = 0;
	int j = 0;
	float far;
	dhtDat[0] = dhtDat[1] = dhtDat[2] = dhtDat[3] = dhtDat[4] = 0;
	GPIO dhtPin(INPUTPIN, OUTPUT);

	dhtPin.setValue(LOW);
	usleep(18000);
	dhtPin.setValue(HIGH);
	usleep(40);
	dhtPin.setDirection(INPUT);

	for(;;)
	{
		for(int i = 0; i < MAXTIMINGS; i++)
		{
			counter = 0;
			while(dhtPin.getValue() == lastState)
			{
				counter++;
				usleep(1);
				if(counter >= 255)
				{
					break;
				}
			}
			lastState = dhtPin.getValue();

			if(counter >= 255)
			{
				break;
			}

			if((i >= 4) && (i % 2 == 0))
			{
				dhtDat[j/8] <<=1;
				if(counter > 16)
				{
					dhtDat[j/8] |= 1;
				}
				j++;
			}
		}

		if((j >= 40) && (dhtDat[4] == ((dhtDat[0] + dhtDat[1] + dhtDat[2] + dhtDat[3]) & 0xFF))) 
		{
			far = dhtDat[2] * 9. / 5. + 32;
			cout << "Humidity: " << dhtDat[0] << '.' << dhtDat[1] << " Temperature: " << dhtDat[2] << 'C' << far << "F\n";
		}
		else
		{
			far = dhtDat[2] * 9. / 5. + 32;
			cout << "Humidity: " << dhtDat[0] << '.' << dhtDat[1] << " Temperature: " << dhtDat[2] << 'C' << far << "F\n";
			cout << "Bad data, skip\n";
		}
		sleep(1);
		if(interuptReceived)
		{
			cout << "SIGINT received. Unexporting pins.\n";
			dhtPin.~GPIO();
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
