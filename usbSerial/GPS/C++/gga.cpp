#include"header.hpp"

void GGA::init()
{
	inFile.open("/dev/ttyUSB0");
}

void GGA::getDat()
{
	string test;
	char check;

	while(inFile)
	{
		inFile >> check;
		if(check  == '$')
		{
			getline(inFile, test, ',');
			if(test == "GPGGA")
			{
				sentence = test;
				inFile.ignore();
				inFile >> time;
				inFile.ignore();
				inFile >> latitude;
				latitude /= 100;
				inFile.ignore();
				inFile >> ns;
				inFile.ignore();
				inFile >> longitude;
				longitude /= 100;
				inFile.ignore();
				inFile >> ew;
				inFile.ignore();
				inFile >> gpsQual;
				inFile.ignore();
				inFile >> numSats;
				inFile.ignore();
				inFile >> horDil;
				inFile.ignore();
				inFile >> alt;
				inFile.ignore();
				inFile >> altUnit;
				inFile.ignore();
				inFile >> geoSep;
				inFile.ignore();
				inFile >> geoUnit;
				inFile.ignore();
				inFile >> datAge;
				inFile.ignore();
				inFile >> refStation;
				inFile.ignore();
				inFile >> checkSum;
				break;
			}
			else
			{
				check = ' ';
				getline(inFile, test);
			}
		}
	}
}

void GGA::print()
{
	cout << fixed << showpoint << setprecision(6) << "Sentence: " <<
sentence << endl << "Time: " << time << endl <<
"Latitude: " << latitude << ' ' << ns << endl <<
"Longitude: " << longitude << ' ' << ew << endl <<
"Fix Quality: " << gpsQual << endl << "Number of Satelites: " <<
numSats << endl << "Horizontal Dillution: " << horDil <<
endl << "Altitude: " << alt << ' ' << altUnit <<
endl << "Height of Geoid: " << geoSep << ' ' <<
geoUnit << endl << "Age of Data: " << datAge << endl <<
"Differential Reference Station: " << refStation << endl <<
"Checksum: " << checkSum << endl << endl;
}

void GGA::rawStream()
{
	char check;
	string buf; 

	while(inFile)
	{
		inFile >> check;
		if(check == '$')
		{
			getline(inFile, sentence, ',');
			if(sentence == "GPGGA")
			{
				getline(inFile, buf, ',');
				time = stod(buf);
				inFile.ignore();
				inFile >> latitude;
				latitude /= 100;
				inFile.ignore();
				inFile >> ns;
				inFile.ignore();
				inFile >> longitude;
				longitude /= 100;
				inFile.ignore();
				inFile >> ew;
				inFile.ignore();
				inFile >> gpsQual;
				inFile.ignore();
				inFile >> numSats;
				inFile.ignore();
				inFile >> horDil;
				inFile.ignore();
				inFile >> alt;
				inFile.ignore();
				inFile >> altUnit;
				inFile.ignore();
				inFile >> geoSep;

				break;
			}
		}
	}
}
