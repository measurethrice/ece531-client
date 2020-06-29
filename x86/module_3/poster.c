#include <stdio.h>
#include <curl/curl.h>

#define OK		0
#define INIT_ERR	1
#define REQ_ERR		2

#define URL		"https://postman-echo.com/post"

int main(void) {
	CURL 		*curl;
	CURLcode	res;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL,URL);
		curl_easy_setopt(curl, CURLOPT_POST, 1L); 
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			return REQ_ERR;
		}
		curl_easy_cleanup(curl);
	} else {
		return INIT_ERR;
	}
	return OK;
}




