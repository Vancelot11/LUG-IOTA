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
		
                //direct URL to connect to the server.
                const string serverURL = "flip3.engr.oregonstate.edu:2031";
		
                //API routes to append to server URL.
                const string tempURL = serverURL + "/iota/temp?value=";
		const string knobURL = serverURL + "/iota/knob?value=";
		const string rhURL = serverURL + "/iota/rh?value=";
		const string lightURL = serverURL + "/iota/light?value=";
		const string buttonURL = serverURL + "/iota/button/";
		CURLcode res;
	public:
		dataHandler();
		~dataHandler();

		int buttonValPost(bool);
		int tempValPost(string);
		int knobValPost(string);
		int rhValPost(string);
		int lightValPost(string);
                
};

//POST /iota/button/enable OR POST /iota/button/disable
int dataHandler::buttonValPost(bool val)
{
        //make a copy of the URL
	string postRequest= buttonURL;
	int result;

        //change the route depending on if the button is pressed or unpressed
	if(val)
	{
		postRequest.append("enable");
	}
	else
	{
		postRequest.append("disable");
	}

        //set CURL url to post to our complete URL
	curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

        //actual POST call
	result = curl_easy_perform(curl);
	return result;
}

//POST /iota/temp?value=VALUE
int dataHandler::tempValPost(string val)
{
        //copy the URL
	string postRequest=tempURL;
	int result;

        //add its value to the end (?value=VALUE)
	postRequest.append(val);

        //set the CURL call to the URL
	curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

        //call the API
	result = curl_easy_perform(curl);
	return result;
}

//POST /iota/knob?value=VALUE
int dataHandler::knobValPost(string val)
{
        string postRequest=knobURL;
        int result;

        postRequest.append(val);
        curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

        result = curl_easy_perform(curl);
        return result;
}

//POST /iota/rh?value=VALUE
int dataHandler::rhValPost(string val)
{
        string postRequest=rhURL;
        int result;

        postRequest.append(val);
        curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

        result = curl_easy_perform(curl);
        return result;
}

//POST /iota/light?value=VALUE
int dataHandler::lightValPost(string val)
{
        string postRequest=lightURL;
        int result;

        postRequest.append(val);
        curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

        result = curl_easy_perform(curl);
        return result;
}

//construct the CURL obbject so we do not need to change headers for every call
dataHandler::dataHandler()
{
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

	headers = curl_slist_append(headers, "cache-control: no-cache");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl);
}

//clean up CURL object
dataHandler::~dataHandler()
{
	curl_easy_cleanup(curl);
}
