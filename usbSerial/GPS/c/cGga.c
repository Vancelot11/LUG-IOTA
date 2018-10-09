#include"header.h"

void init(struct GGA *dat)
{
	if((dat->gpsFD = open(SERIALPORT, O_RDONLY | O_NOCTTY)) < 0) 
	{
		perror(SERIALPORT);
		exit(-1);
	}
	
	tcgetattr(dat->gpsFD, &dat->oldtio);
	bzero(&dat->newtio, sizeof(dat->newtio));

	dat->newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	dat->newtio.c_iflag = IGNPAR | ICRNL;
	dat->newtio.c_oflag = 0;
	dat->newtio.c_lflag = ICANON;

	tcflush(dat->gpsFD, TCIFLUSH);
	tcsetattr(dat->gpsFD, TCSANOW, &dat->newtio);
}

void getDat(struct GGA *dat)
{
	int result = 0;
	char buffer1[100], *buffer2;
	char check = '$';
	
	result = read(dat->gpsFD, buffer1, BUFSIZE);
	buffer1[result] = 0;
	buffer2 = strtok(buffer1, ",*");
	if(buffer2[0] == check)
	{
		printf("%s\n", buffer2);
	}
}

void finalize(struct GGA *dat)
{
	tcsetattr(dat->gpsFD, TCSANOW, &dat->oldtio);
}
