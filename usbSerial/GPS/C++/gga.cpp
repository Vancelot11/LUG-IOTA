#include"header.hpp"

void GGA::init()
{
	inFile.open("/dev/ttyUSB0");
}

void GGA::getDat()
{
	string test, buf;
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
				if(!(inFile.peek() == ',')){inFile >> time; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> latitude; inFile.ignore();}  else inFile.ignore();
				latitude /= 100;
				if(!(inFile.peek() == ',')){inFile >> ns; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> longitude; inFile.ignore();}  else inFile.ignore();
				longitude /= 100;
				if(!(inFile.peek() == ',')){inFile >> ew; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> gpsQual; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> numSats; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> horDil; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> alt; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> altUnit; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> geoSep; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> geoUnit; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> datAge; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> refStation; inFile.ignore();}  else inFile.ignore();
				if(!(inFile.peek() == ',')){inFile >> checkSum; inFile.ignore();}  else inFile.ignore();
				break;
			}
			else
			{
				check = ' ';
				sentence = "";
				inFile.ignore(100, '\n');
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
