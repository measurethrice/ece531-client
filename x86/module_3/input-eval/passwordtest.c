#include <stdio.h>
#include <string.h>

int main(void)
{
    int pass = 0;
    char password[] = "password";
    char buffer[3+ sizeof password];

    printf("\n Enter your password : \n");
    fgets(buffer, sizeof buffer, stdin);
    buffer[strcspn(buffer,"\r\n")]=0;

    if(strcmp(buffer, password))
    {
        printf ("\n Incorrect Password \n");
    }
   
    else
    {
        printf ("\n Correct Password \n");
        pass = 1;
    }

    if(pass)
    {
        printf ("\n Root privileges authorized \n");
    }

    return 0;
}