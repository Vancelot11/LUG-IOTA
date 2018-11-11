#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<curl/curl.h>
#include<unistd.h>

using std::string;

class dataHandler
{
	private:
		CURL *curl;
		struct curl_slist *headers = NULL;
		const string serverURL = "flip3.engr.oregonstate.edu:2031";
		const string analogURL = serverURL + "/iota/analog?value=";
		const string buttonURL = serverURL + "/iota/button/";
		CURLcode res;
	public:
		dataHandler();
		~dataHandler();
		int analogValPost(string);
		int digitalValPost(bool);
};

int dataHandler::digitalValPost(bool val)
{
	string postRequest= buttonURL;
	int result;

	if(val)
	{
		postRequest.append("enable");
	}
	else
	{
		postRequest.append("disable");
	}
	curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

	result = curl_easy_perform(curl);
	return result;
}

int dataHandler::analogValPost(string val)
{
	string postRequest= analogURL;
	int result;

	postRequest.append(val);
	curl_easy_setopt(curl, CURLOPT_URL, postRequest.c_str());

	result = curl_easy_perform(curl);
	return result;
}

dataHandler::dataHandler()
{
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

	headers = curl_slist_append(headers, "cache-control: no-cache");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl);
}

dataHandler::~dataHandler()
{
	curl_easy_cleanup(curl);
}
