#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<curl/curl.h>
#include<unistd.h>

using std::string;

//wraps API calls to the web server.
class dataHandler
{
	private:
		CURL *curl;
		struct curl_slist *headers = NULL;
                //API routes to append to server URL.
                string tempURL;
		string knobURL;
		string rhURL;
		string lightURL;
		string buttonURL;
		CURLcode res;
	public:
		dataHandler(string);
		~dataHandler();

		int buttonValPost(bool);
		int tempValPost(string);
		int knobValPost(string);
		int rhValPost(string);
		int lightValPost(string);
                
};

#endif