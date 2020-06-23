#include <stdio.h>
#include <string.h>
 
int main(void)
{
    char input[512]; 
   
    //const char text[] = "--get --url http://www.cnn.com";
   printf("Enter your curl command: \n");
    fgets(input,sizeof(input),stdin);
    input[strlen(input)-1] = '\0';
   
    char commands[512]; 
    char url[512];
    char message[512];


    //Parse string for commands + url + message
    if ( sscanf(input, "%[^/]%*2c%[^ ]%*1c%[^\n]", commands, url, message) == 3) { 
        printf("commands = \"%s\"\n", commands);
        printf("url = \"%s\"\n", url);
        printf("message = \"%s\"\n", message);
    }
    //Parse string for commands + url
    else if ( sscanf(input, "%[^/]%*2c%[^ ]", commands, url) == 2) {
        printf("commands = \"%s\"\n", commands);
        printf("url = \"%s\"\n", url);
    }
    //Parse string for commands only
     else if ( sscanf(input, "%[^/]%*2c", commands) == 1) {
        printf("commands = \"%s\"\n", commands);
    }

    //Check commands for help
    if(strcmp(commands, "-h") == 0){
        printf("\nYou asked for help. \n");
    }

    if(strcmp(commands, "--help") == 0){
        printf("\nYou asked for help. \n");
    }

    return 0;
}
