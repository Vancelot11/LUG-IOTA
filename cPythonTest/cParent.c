#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void callPy();

int main()
{
	pid_t pyPid;

	printf("Parent now calling child.\n");
	pyPid = vfork();
	if(pyPid == 0)
	{
		callPy();
		exit(1);
	}
	return 0;
}

void callPy()
{
	printf("Child calling python script...\n");
	system("./childPython.py");
}
