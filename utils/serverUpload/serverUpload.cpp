#include"main.h"

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
