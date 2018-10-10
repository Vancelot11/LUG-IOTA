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
} void getDat(struct GGA *dat)
{
	int result = 0, found = 0;
	char buffer1[BUFSIZE], *buffer2, check = ' ', test[] = "GPGGA";
	test[5] = 0;
	
	while(!found)
	{
		result = read(dat->gpsFD, buffer1, BUFSIZE);
		buffer1[result] = '\n';
		check = buffer1[0];

		if(buffer1[0] == check)
		{
			buffer2 = strtok(buffer1, " $,*");
			if(strcmp(buffer2, test) == 0)
			{
				dat->sentence = buffer2;
				buffer2 = strtok(NULL, " $,*");
				dat->time = atof(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->latitude = atof(buffer2);
				dat->latitude /= 100;
				buffer2 = strtok(NULL, " $,*");
				dat->ns = buffer2[0];
				buffer2 = strtok(NULL, " $,*");
				dat->longitude = atof(buffer2);
				dat->longitude /= 100;
				buffer2 = strtok(NULL, " $,*");
				dat->ew = buffer2[0];
				buffer2 = strtok(NULL, " $,*");
				dat->gpsQual = atoi(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->numSats = atoi(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->horDil = atof(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->alt = atof(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->altUnit = buffer2[0];
				buffer2 = strtok(NULL, " $,*");
				dat->geoSep = atof(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->geoUnit = buffer2[0];
				buffer2 = strtok(NULL, " $,*");
				dat->datAge = atof(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->refStation = atoi(buffer2);
				buffer2 = strtok(NULL, " $,*");
				dat->checkSum = atoi(buffer2);

				found = 1;
			}
			else
			{
				buffer1[0] = '\n';
			}
		}
	}
}

void printGGA(struct GGA *dat)
{
	printf("Sentence: %s\n", dat->sentence);
	printf("Time: %f\nLatitude: %f %c\nLongitude: %f %c\nFix Quality: %i\nNumber of Satellites: %i\nHorizontal Dillution: %f\nAltitude: %f %c\nHeight of Geoid: %f %c\nAge of Data: %f\nDifferential Reference Station: %i\nChecksum: %i\n\n", dat->time, dat->latitude, dat->ns, dat->longitude, dat->ew, dat->gpsQual, dat->numSats, dat->horDil, dat->alt, dat->altUnit, dat->geoSep, dat->geoUnit, dat->datAge, dat->refStation, dat->checkSum);
}

void finalize(struct GGA *dat)
{
	tcsetattr(dat->gpsFD, TCSANOW, &dat->oldtio);
}
