#include <curl/curl.h>

CURL *curl;
CURLcode res;


struct curl_slist *headerlist = nullptr;
headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

strTargetURL = "https://www.example.com/bind";
strResourceJSON = "{\"snippet\": {\"title\": \"this is title\", \"scheduledStartTime\": \"2017-05-15\"},\"status\": {\"privacyStatus\": \"private\"}}";

curl_global_init(CURL_GLOBAL_ALL);

curl = curl_easy_init();
if (curl)
{
	curl_easy_setopt(curl, CURLOPT_URL, strTargetURL.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strResourceJSON.c_str());

	// ��� ���
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_slist_free_all(headerlist);

	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		return false;
	}

	std::cout << "------------Result" << std::endl;
	std::cout << chunk.memory << std::endl;

	return true;
}
return false;