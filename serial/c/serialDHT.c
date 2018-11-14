#include"serialDHT.h"

void sig_handler(int sig);
int interuptReceived = 0;

int main()
{
	wiringPiSetup();

	int *result[5] = {0,0,0,0,0};
	int lastState = HIGH;
	int count = 0;

	pinMode(SERIALPIN, OUTPUT);
	digitalWrite(SERIALPIN, HIGH);
	usleep(18000);
	digitalWrite(SERIALPIN, LOW);
	usleep(18000);
	digitalWrite(SERIALPIN, HIGH);
	pinMode(SERIALPIN, INPUT);
	lastState = HIGH;
	printf("Start signal sent.\n");

	for(;;)
	{
		if(digitalRead(SERIALPIN) != lastState)
		{
			lastState = digitalRead(SERIALPIN);
			printf("count:%d lastState: %d\n", count, lastState);
			count++;
		}
	}

	return 0;
}

void sig_handler(int sig)
{
	write(0, "SIGINT received", 32);
	interuptReceived = 1;
}
