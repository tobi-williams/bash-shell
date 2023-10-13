/*
project: 01
author: 
email: 
student id: 
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/

/* 
DEFINE THE FUNCTION PROTOTYPES
user_prompt_loop()
get_user_command()
parse_command()
execute_command()
*/

void user_prompt_loop();
char * get_user_command();
char ** parse_command(char *);

int main(int argc, char **argv)
{
    if (argc > 1){		//check if user enters arguments while launching the shell, and print an error message accordingly
    	fprintf(stderr, "Error: Do not enter command line arguments --- ");
    	for (int i = 0; argv[i] != NULL; i++){
    		printf("%s ", argv[i]);
    	}
    	printf("\n");
    	return 1;
    }
    
    else{				//if user enters only one argument, run command prompt loop
    	user_prompt_loop();
    	return 0;
    }
}


void user_prompt_loop() 
{
    char *input;			//array of characters for collecting our command
    char **parsedInput;		//array of strings to hold the parsed command
    
    while(1){
    	printf(">> ");
    	input = get_user_command();		//accept user command
    	if (input == NULL){
    	    exit(1);
    	}
    	
    	parsedInput = parse_command(input);		//parse command
    	
    	break;
    }
    
    for(int i = 0; input[i] != '\0'; i++){ //testing input reading
    	printf("%c", input[i]);
    }
    
    printf("\n");
    
    
    for(int i = 0; parsedInput[i] != NULL; i++){ //testing input reading
    	printf("%s\n", parsedInput[i]);
    }
    
    exit(0);
}


char* get_user_command()
{
    char* input = NULL;
    size_t length = 0;
    
    ssize_t read = getline(&input, &length, stdin);
    
    if (read == -1){		//check if getline returns a value -1, indicating read failure
    	perror("Input read failed: ");
    	free(input);
    	return NULL;
    }
    
    return input;
}


char** parse_command(char *input)
{
	char *unescapedInput = unescape(input, stderr);		
	//free(input);
	
	char **parsedInput = (char **)malloc(strlen(unescapedInput) * sizeof(char *));		//declare an array of strings and allocate enough memory
	char *token;		//character array to store each command token
	token = strtok(unescapedInput, " ");		//break command into tokens delimited by whitespaces
	int i;
	for(i = 0; token != NULL; i++){		//populate string array with the tokens
    	parsedInput[i] = token;
    	token = strtok(NULL, " ");
    }
    
    parsedInput[i] = NULL;				//terminate the string array with NULL
    
    return parsedInput;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

/*execute_command()*/
//////{
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function
    */

    /*
    ENTER YOUR CODE HERE
    */
///////}
