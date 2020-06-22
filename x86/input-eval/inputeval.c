#include <stdio.h>
#include <string.h>

//char getInput();

char input[50];

int main(void) {
	int i, found = 0, n;

	const char *valid_inputs[] = {
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
		"-h",
		"--help"
	};

	printf("Welcome. Enter -h/--help for a list of commands:\n\n");

	while(1){
		//Get user input 
		fflush(stdin);
 		fgets(input, 10, stdin);
 		input[strlen(input)-1] = '\0';
 		//getInput();

 		//Check if user needs help
 		if(strcmp(input, "-h") == 0){
 			// printf("\nYou asked for help. \n");
 			printHelp();
 		}

 		if(strcmp(input, "--help") == 0){
 			printf("\nYou asked for help. \n");
 		}

 		//Evaluate if user input is valid
 		else {

 			for(i = 0; i < 11; i++){
 				if(strcmp(input, valid_inputs[i]) == 0){
 					found = 1;
 					break;
 				}
 			}

 			if(found==1){
 				printf("\nValid input: %s \n\n", input);
 				// make call to curl here
 			}

 			else {
 				printf("\nError: invalid input.\nEnter -h/--help for valid commands.\n\n");
 			}

 			found = 0; 
 		}
 	}
 	return 0;
}


// char getInput(){
// 	fflush(stdin);
// 	fgets(input, 10, stdin);
//  	input[strlen(input)-1] = '\0';
// }

void printHelp(void){
	printf("\nYou asked for help. \n");
	printf("Valid commands are:\n-u/--url to \n-)
}




