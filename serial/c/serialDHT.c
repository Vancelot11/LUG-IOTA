#include"serialDHT.h"

void sig_handler(int sig);
int interuptReceived = 0;


int main()
{
	wiringPiSetup();
	piHiPri(99);

	int result[6] = {0,0,0,0,0,0};
	result[6] = result[5] = result[4] = result[3] = result[2] = result[1] = result[0] = 0;
	int lastState = HIGH;
	struct timeval begin, end;

	pinMode(SERIALPIN, OUTPUT);
	digitalWrite(SERIALPIN, LOW);
	usleep(18000);
	digitalWrite(SERIALPIN, HIGH);
	pinMode(SERIALPIN, INPUT);

	for(int i = 40; i >= 0;)
	{
		if(digitalRead(SERIALPIN) != lastState)
		{
			lastState = digitalRead(SERIALPIN);
			if(lastState == 1)
			{
				gettimeofday(&end, NULL);
				if((end.tv_usec - begin.tv_usec) >= 70)
				{
					result[i/8] += 1<<(i%8);
				}
				else
				{
					result[i/8] += 0<<(i%8);
				}
				printf("result[%d]: %d\n", i/8, result[i/8]);
				i--;
				gettimeofday(&begin, NULL);
			}
		}
	}
	printf("RH: %d.%d\tT: %d.%d\tChecksum: %d\tExpected %d\n", result[4], result[3], result[2], result[1], result[0], result[4] + result[3] + result[2] + result[1]);

	return 0;
}

void sig_handler(int sig)
{
	write(0, "SIGINT received", 32);
	interuptReceived = 1;
}
