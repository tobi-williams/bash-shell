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

void user_prompt_loop();
char * get_user_command();
char ** parse_command(char *);
void execute_command(char **);
void freeArray(char **);

int main(int argc, char **argv)
{
    if (argc > 1){		//check if user enters arguments while launching the shell, and print an error message accordingly
    	fprintf(stderr, "shell error: do not enter command line arguments --- ");
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
    char *input;								//array of characters for collecting our command
    char **parsedInput;							//array of strings to hold the parsed command
    
    while(1){
    	printf(">> ");
    	input = get_user_command();				//accept user command
    	if (input == NULL){
    	    free(input);
    	    exit(1);
    	}
    	
    	parsedInput = parse_command(input);		//parse command
    	
    	for(int i = 0; input[i] != '\0'; i++){ //testing input reading
    		printf("%c", input[i]);
    	}

    	printf("\n\n");
    
    	for(int i = 0; parsedInput[i] != NULL; i++){ //testing input parsing
    		printf("%s\n", parsedInput[i]);
    	}
    
    	execute_command(parsedInput);
		free(input);
		free(parsedInput);
    }
}


char* get_user_command()
{
    char* input = NULL;
    size_t length = 0;
    
    ssize_t size = getline(&input, &length, stdin);
    
    if (size == -1){					//check if getline returns a value -1, indicating read failure
    	fprintf(stderr, "shell error: input read failure\n");
    	free(input);
    	return NULL;
    }	
    
    if (size > 0){
    	input[size - 1] = '\0';			//remove endline character at the end of the string for manipulation purposes
    }
    
    
    return input;
}


char** parse_command(char *input)
{
	char *unescapedInput = unescape(input, stderr);		
	//free(input);
	
	char **parsedInput = (char **)malloc(strlen(unescapedInput) * sizeof(char *));		//declare an array of strings and allocate enough memory
	char *token;								//character array to store each command token
	token = strtok(unescapedInput, " ");		//break command into tokens delimited by whitespaces
	int i;
	for(i = 0; token != NULL; i++){				//populate string array with the tokens
    	parsedInput[i] = token;
    	token = strtok(NULL, " ");
    }
    
    parsedInput[i] = NULL;						//terminate the string array with NULL
    //free(unescapedInput);
    
    return parsedInput;
}


void execute_command(char **commandArr)
{	
	int tokens = 0;								//count how many arguments are in the command
	while (commandArr[tokens] != NULL){
		tokens++;
	}


    if ((strcmp(commandArr[0], "/proc") == 0) 
    	|| (strcmp(commandArr[0], "proc") == 0)
    	|| (strcmp(commandArr[0], "exit") == 0)){		//check if command is proc or exit
    	
    	if (strcmp(commandArr[0], "exit") == 0){		//check if command is exit
    		if (tokens == 1){							//if no other argument is passed exit with code 0
    			freeArray(commandArr);
    			exit(0);
    		}
    		else if (tokens == 2){						//if a second argument is passed
    			char *tmp = commandArr[1];
    			int digit = 1;
    			
    			for (int i = 0; tmp[i] != '\0'; i++){	
    				if (!isdigit(tmp[i])){				//check if the string token is made up entirely of integers
    					fprintf(stderr, "shell error: exit only accepts none or one integer arguments\n");
    					digit = 0;
    				}
    			}
    			if (digit){								//convert the string into its integer equivalent and return that as the exit code
    				exit(atoi(tmp));
    			}
    		}
    		else{										//if more than 2 arguments is passed, print an error message
    			fprintf(stderr, "shell error: exit only accepts none or one integer arguments\n");
    		}
    	}
    	
    	else{											//check if command is proc
    		printf("/proc or proc\n");
    	}
    }
    
	else{												//if command if not proc or exit, fork and execute the command
		printf("not proc or exit\n");
	}
    
    return;
}


void freeArray(char **arr){
	for (int i = 0; arr[i] != NULL; i++){
		free(arr[i]);
	}
	free(arr);
}

















