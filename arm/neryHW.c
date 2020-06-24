//cc -o hw neryHW.c
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/stat.h>

#define OK			0
#define INIT_ERR	1
#define REQ_ERR		2

//#define URL		"https://my-json-server.typicode.com/typicode/demo"

char user_input[100];

int main(void) {
	CURL 		*curl;
	CURLcode	res;

/*	Get input string*/
    char user_input[512]; 
   	printf("Enter your curl command (enter -h/--help for valid commands): \n");
    fgets(user_input,sizeof(user_input),stdin);
    user_input[strlen(user_input)-1] = '\0';

/*  Quick and dirty regx parsing*/
    char commands[512]; 
    char url[512];
    char message[512];

    int FLAG_NO_MESSAGE = 0; 

    //Parse string for commands + url + message
    if ( sscanf(user_input, "%[^/]%*2c%[^ ]%*1c%[^\n]", commands, url, message) == 3) { 
        //printf("commands = \"%s\"\n", commands);
        //printf("url = \"%s\"\n", url);
        //printf("message = \"%s\"\n", message);
    }
    //Parse string for commands + url
    else if ( sscanf(user_input, "%[^/]%*2c%[^ ]", commands, url) == 2) {
        FLAG_NO_MESSAGE = 1;	//No message
        //printf("commands = \"%s\"\n", commands);
        //printf("url = \"%s\"\n", url);
    }
    //Parse string for commands only
     else if ( sscanf(user_input, "%[^/]%*2c", commands) == 1) {
        FLAG_NO_MESSAGE = 1;
        //printf("commands = \"%s\"\n", commands);
    }

/*  Check commands for help*/
    if(strcmp(commands, "-h") == 0 || strcmp(commands, "--help") == 0) {
        printf("\nYou asked for help!\n\n");
        printf("Valid commands:\n"
        	"-u/--url\n"
        	"-g/--get to GET\n"
        	"-o/--post to POST\n"
        	"-p/--put to PUT\n"
        	"-d/--delete to DETLETE\n"
        	"-h/--help for HELP\n\n");
        printf("Valid example:\n\n--post -u http://<some url> string to post\n\n");
        exit(1);
    }

/*  Keywords*/
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
    
/*  Initialize flags for curl calls*/
	int FLAG_URL = 		0; 
	int FLAG_GET = 		0; 
	int FLAG_POST = 	0; 
	int FLAG_PUT = 		0; 
	int FLAG_DEL = 		0; 

/*	Search 'commands' string for keywords, then set curl flags for found keywords*/
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
		if (FLAG_POST == 1) {
			FLAG_PUT = 0; 
		}
	}

	//Check flag outputs 
	// printf("FLAG_URL:  %i\n", FLAG_URL);
	// printf("FLAG_GET:  %i\n", FLAG_GET );
	// printf("FLAG_POST: %i\n", FLAG_POST );
	// printf("FLAG_PUT : %i\n", FLAG_PUT);
	// printf("FLAG_DEL : %i\n", FLAG_DEL);

/*	Check for error states*/
	if (FLAG_URL == 0) {
		printf("ERROR: No URL provided.\n");
		exit(1);
	}
	else if (FLAG_POST == 1 && FLAG_NO_MESSAGE == 1) {
		printf("ERROR: No string submitted for POST.\n");
		exit(1);
	}
	else if (FLAG_PUT == 1 && FLAG_NO_MESSAGE == 1) {
		printf("ERROR: No string submitted for PUT.\n");
		exit(1);
	}
	else if (FLAG_DEL == 1 && FLAG_NO_MESSAGE == 1) {
		printf("ERROR: No string submitted for DELETE.\n");
		exit(1);
	}

	//Add http:// to url
	char URL[] = "http://"; 
	strcat(URL, url);
	//printf("URL = %s\n", URL);

/*	Construct file for PUT upload*/
	FILE *hd_src;

	if (FLAG_PUT == 1) {
		hd_src = fopen("program.txt", "w");
		if (hd_src == NULL) {
			printf("Error!");
			exit(1);
		}

		//Write message to file
		fprintf(hd_src, "%s", message);
		fclose(hd_src);
	}

/*	Get filesize for PUT upload*/
	long filesize;
	FILE* fp = fopen("program.txt", "r");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fclose(fp);
	}
	//printf("fileseize: %li\n", filesize );

	//Alternative method for getting file size (did not work) 
	// char *file; 
	// hd_src = fopen(file, "r");
	// struct stat file_info;
	// stat(file, &file_info); 

/*	Build curl based on flags*/
	curl = curl_easy_init();
	if (curl) {
		if (FLAG_GET == 1) {
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			//printf("GET request\n");
		}
		else if (FLAG_POST == 1) {
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *message);
			//printf("POST request\n");
		}
		else if (FLAG_PUT == 1) {
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
			//curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
			curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)filesize);
			//printf("PUT request\n");
		}
		else if (FLAG_DEL == 1) {
			curl_easy_setopt(curl,CURLOPT_CUSTOMREQUEST,"DELETE");
			curl_easy_setopt(curl, CURLOPT_URL,URL);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, *message);
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




