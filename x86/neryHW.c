#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define OK			0
#define INIT_ERR	1
#define REQ_ERR		2

//#define URL		"https://my-json-server.typicode.com/typicode/demo"

char user_input[100];

int main(void) {
	CURL 		*curl;
	CURLcode	res;
	char command[] = "--u";

    //const char text[] = "--get --url http://www.cnn.com";
    char user_input[512]; 
   	printf("Enter your curl command: \n");
    fgets(user_input,sizeof(user_input),stdin);
    user_input[strlen(user_input)-1] = '\0';

    //Quick and dirty regx parser
    char commands[512]; 
    char url[512];
    char message[512];
    //Parse string for commands + url + message
    if ( sscanf(user_input, "%[^/]%*2c%[^ ]%*1c%[^\n]", commands, url, message) == 3) { 
        printf("commands = \"%s\"\n", commands);
        printf("url = \"%s\"\n", url);
        printf("message = \"%s\"\n", message);
    }
    //Parse string for commands + url
    else if ( sscanf(user_input, "%[^/]%*2c%[^ ]", commands, url) == 2) {
        printf("commands = \"%s\"\n", commands);
        printf("url = \"%s\"\n", url);
    }
    //Parse string for commands only
     else if ( sscanf(user_input, "%[^/]%*2c", commands) == 1) {
        printf("commands = \"%s\"\n", commands);
    }

    //Check commands for help
    if(strcmp(commands, "-h") == 0 || strcmp(commands, "--help") == 0) {
        printf("\nYou asked for help. \n");
    }

    //Keywords
    int num_keywords = 10; 
    const char *keywords[] = {
    	"-u",
		"--url",
		"-o",
		"--post",
		"-g",
		"--get",
		"-p",
		"--put",
		"-d",
		"--delete",
	};
    
    //Initialize flags for curl calls
	int FLAG_URL = 		0; 
	int FLAG_GET = 		0; 
	int FLAG_POST = 	0; 
	int FLAG_PUT = 		0; 
	int FLAG_DEL = 	0; 

	//Search 'commands' string for keywords, then set curl flags for found keywords
	char *tmp; 
	char evaluate; 
	for (int i = 0; i < num_keywords; i++) {
		tmp = strstr(commands, keywords[i]);
		if(tmp != NULL) {
			//printf("Yes %s flag present\n", keywords[i]);

			if(strcmp(keywords[i], "-u") == 0 || strcmp(keywords[i], "--url") == 0) {
				FLAG_URL = 1; 
				//printf("FLAG_URL is now %i\n", FLAG_URL);
			}
			else if (strcmp(keywords[i], "-o") == 0 || strcmp(keywords[i], "--post") == 0) { 
				FLAG_POST = 1; 
				//printf("FLAG_POST is now %i\n", FLAG_POST);
			}
			else if (strcmp(keywords[i], "-g") == 0 || strcmp(keywords[i], "--get") == 0) {
				FLAG_GET = 1; 
				//printf("FLAG_GET is now %i\n", FLAG_GET );
			}
			else if (strcmp(keywords[i], "-p") == 0 || strcmp(keywords[i], "--put") == 0) {
				FLAG_PUT = 1; 
				//printf("FLAG_PUT is now %i\n", FLAG_PUT );
			}
			else if (strcmp(keywords[i], "-d") == 0 || strcmp(keywords[i], "--delete") == 0) {
				FLAG_DEL = 1; 
				//printf("FLAG_DEL is now %i\n", FLAG_DEL);
			}
		}

		// else {
		// 	printf("No %s flag present\n", keywords[i]);
		// }

		//For special case where --post also flags -p 
		if (FLAG_POST == 1){
			FLAG_PUT = 0; 
		}
	}

	//Check flag outputs 
	printf("FLAG_URL: %i\n", FLAG_URL);
	printf("FLAG_GET: %i\n", FLAG_GET );
	printf("FLAG_POST: %i\n", FLAG_POST );
	printf("FLAG_PUT : %i\n", FLAG_PUT);
	printf("FLAG_DEL : %i\n", FLAG_DEL);


	//Add http:// to url
	char URL[] = "http://"; 
	strcat(URL, url);
	printf("URL = %s\n", URL);

	//Build curl based on flags
	curl = curl_easy_init();
	if (curl) {
		
		// if (FLAG_URL == 1){
		// 	curl_easy_setopt(curl, CURLOPT_URL,URL);
		// 	printf("URL request\n");
		// }
		if (FLAG_GET == 1){
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			printf("GET request\n");
		}
		if (FLAG_POST == 1){
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			printf("POST request\n");
		}
		if (FLAG_PUT == 1){
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_READDATA, message);
			printf("PUT request\n");
		}
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			return REQ_ERR;
		}
		curl_easy_cleanup(curl);
	} 

	else {
		return INIT_ERR;
	}
	return OK;

	return 0; 
}




