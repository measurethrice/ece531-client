#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define OK		0
#define INIT_ERR	1
#define REQ_ERR		2

//#define URL		"https://my-json-server.typicode.com/typicode/demo"

char input[100];

int main(void) {
	CURL 		*curl;
	CURLcode	res;
	char command[] = "--u";
	char URL; 

	//Get input
	printf("Enter your curl request: ");
	fflush(stdin);
	fgets(input,100,stdin);
	input[strlen(input)-1] = '\0';

	printf("You entered: %s\n", input);


	//Clean strtok invocation
	// char delim[] = " ";
	// char* token;
	// char *p 

	// for (token = strtok(input, delim); token; token = strtok(NULL, delim)){
 //    	printf("token=%s\n", token);
	// }

	//Another strtok that writes to array
	char * tokens[50];
	size_t n = 0;

	for (char * p = strtok(input, " "); p; p = strtok(NULL, " "))
	{	
    	if (n >= 50)
    	{
        // maximum number of storable tokens exceeded
        break;
    	}
    tokens[n++] = p;
	}

	for (size_t i = 0; i != n; ++i)
	{
     	printf("Token %zu is '%s'.\n", i, tokens[i]);
	}


	printf("You are here (1)\n");
	// //Rip URL from string
	char url[512]; 
	char token[512]; 
	
	for(int i = 0; i < sizeof(tokens); i++){
		printf("You made it here (2)\n");
		if (sscanf(tokens[i], "http://%511[^/\n]",url) == 1){
			printf("You made it here (3)\n");
			sprintf(token, "%s\n", tokens[i] );
			break;
		}
	}
	
	printf("%s\n", token);
	// if (sscanf(tokens[1], "http://%511[^/\n]",url) == 1){

	// 	printf("url = \"%s\"\n", url);
	// }

	//Analyze string for flags
	char findstring[] = "--url"; 
	char *tmp; 
	tmp = strstr(input,findstring);
	if(tmp != NULL){
		printf("Word is present");
	}
	else{
		printf("No flags present\n\n");
	}



	//Build curl based on flags
	curl = curl_easy_init();
	if (curl) {
		
		if(strcmp(command, "--url") == 0){
			curl_easy_setopt(curl, CURLOPT_URL,token);
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L); 
			printf("%s\n", command );
		}
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




