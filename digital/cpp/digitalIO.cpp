#include"digitalIO.h"

const int GPIOOUT = 26;
const int GPIOIN = 19;

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

	GPIO output(OUTPUTPIN, OUTPUT);
	GPIO input(INPUTPIN, INPUT);

	for(;;)
	{
		if(input.getValue())
		{
			output.setValue(HIGH);
		}
		else
		{
			output.setValue(LOW);
		}
		sleep(1);
		if(interuptReceived)
		{
			cout << "SIGINT received. Unexporting pins.\n";
			input.~GPIO();
			output.~GPIO();
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
