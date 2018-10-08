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

void getDat(struct GGA *dat, char buffer[BUFSIZE])
{
	int result = 0;
	char *fields[20];
	char check[] = "$GPGGA";

	for(;;)
	{
		result = read(dat->gpsFD, buffer, BUFSIZE);
		buffer[result] = 0;
		fields[0] = strtok(buffer, ",*");
		if(strcmp(fields[0], check) == 0)
		{
			while(fields[0] != NULL)
			{
				printf("%s\n", fields[0]);
				fields[0] = strtok(NULL, ",*");
			}
		}
	}
}

void finalize(struct GGA *dat)
{
	tcsetattr(dat->gpsFD, TCSANOW, &dat->oldtio);
}
