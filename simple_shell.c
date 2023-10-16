/*
project: 01
author: Tobi Williams
email: owillia2@umbc.edu
student id: JM29211
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"


void user_prompt_loop();			//continuously loop and prompt the user for input after every operation
char * get_user_command();			//accept user input of arbitrary length and return a character array
char ** parse_command(char *);		//break user input into tokens/arguments and return a string array
void execute_command(char **);		// and execute an operation based on the tokenized input
void freeArray(char **);			//iteratively free allocated memory in a string array
char * getHistoryPath();			//generate and return the path to the hidden history file (wasnt necessary but made sense at the time)
void writeHistory(char *);			//append every command the user enters to a hidden history file ".421sh" in the user's home dir. create one if it does not already exist
void printHistory();				//print the last 10 commands stored in the history file to stdout, print all if less than 10
void displayProc(char **);			//read information from the /proc filesystem and output to stdout



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
    	
    	writeHistory(input);					//write input to history file
    	execute_command(parsedInput);
    	
		free(input);
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
    	|| (strcmp(commandArr[0], "exit") == 0) 
    	|| (strcmp(commandArr[0], "history") == 0)){		//check if command is proc, exit or history
    	
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
    	
    	else if (strcmp(commandArr[0], "history") == 0){	//check if command is history
    		printHistory();
    	}
    	
    	else{											//check if command is proc
    		if (tokens != 2){
    			fprintf(stderr, "shell error: proc command requires two arguments\n");
    		}
    		else{
    			displayProc(commandArr);
    		}	
    	}
    }
    
	else{												//if command is not proc, history or exit, fork and execute the command
		int code = fork();
		
		if(code == 0){									//if fork returns 0, execute the command in the child
			if (execvp(commandArr[0], commandArr) == -1){		//handle execvp returning -1 on failure
				fprintf(stderr, "shell error: command execution failed\n");
				freeArray(commandArr);
				return;
			}
		}
		else if (code > 0){								//check that the parent process is successfully waiting for the child to complete
			int status;
			if (waitpid(code, &status, 0) == -1){
				fprintf(stderr, "shell error: parent waiting unsuccessful\n");
				freeArray(commandArr);
				return;
			}
		}
		else{
			fprintf(stderr, "shell error: command execution failed\n");
			freeArray(commandArr);
			return;
		}
	}
    
    free(commandArr);
    return;
}


void freeArray(char **commandArr){
	for (int i = 0; commandArr[i] != NULL; i++){
		free(commandArr[i]);
	}
	free(commandArr);
}

char * getHistoryPath(){
	char *historyFile = ".421sh";
	char *homeDir = getenv("HOME");
	char *filePath = malloc(strlen(homeDir) + strlen(historyFile) + 2);		//assign memory to a string to store the file path
	
	if (!filePath){															//if filePath returns NULL then malloc failed
		fprintf(stderr, "shell error: unable to access history file\n");
	}
	else {																	//if successful, format the path appropriately and store in filePath
		sprintf(filePath, "%s%c%s", homeDir, '/', historyFile);
	}
	
	return filePath;
}

void writeHistory(char *input){
	char *filePath = getHistoryPath();									
	if (!filePath){															//check to see if filePath was returned successfully
		fprintf(stderr, "shell error: unable to access history file1\n");
		return;
	}
	
	FILE* file = fopen(filePath, "a");										//open file in append mode
	free(filePath);
	
	if (!file){																//handle instance of file not opening successfully
		fprintf(stderr, "shell error: unable to access history file2\n");
		return;
	}
	
	fprintf(file, "%s\n", input);											//write command to file
	fclose(file);
}

void printHistory(){
	char *filePath = getHistoryPath();
	if (!filePath){															//check to see if filePath was returned successfully
		fprintf(stderr, "shell error: unable to access history file\n");
		return;
	}
	
	FILE* file = fopen(filePath, "r");										//open file in read mode
	free(filePath);
	
	if (!file){																//handle instance of file not opening successfully
		fprintf(stderr, "shell error: unable to access history file\n");
		return;
	}
	
	char *commands[10] = {NULL};
	size_t len = 0;
	char *command = NULL;
	ssize_t size;
	int i = 0;
	
	while ((size = getline(&command, &len, file)) != -1){					//read from history file
		if (command[size - 1] == '\n'){										//
			command[size - 1] = '\0';
		}
		
		commands[i] = strndup(command, size);								//put each read command into our buffer
		if(!commands[i]){
			fprintf(stderr, "shell error: unable to print history\n");
			return;
		}
		i = (i+1) % 10;														//increment or roll back around after every 10 items
	}
	
	for (int j = 0; j < 10; j++){											//print the 10 commands in our buffer
		int k = (i + j) % 10;
		if (commands[k]){
			printf("%s\n", commands[k]);
			free(commands[k]);
		}
		else{
			fprintf(stderr, "shell error: unable to print history\n");
			return;
		}
	}
	
	free(command);
	fclose(file);
}

void displayProc(char **commandArr){
	char arg[128];
	char procPath[256];
	
	sprintf(arg, "%s", commandArr[1]);										
	sprintf(procPath, "%s%s", "/proc/", arg);								//concatenate /proc/ and the second argument to create a path
		
	FILE *file = fopen(procPath, "r");
		
	if(file == NULL){														//error if file returned null
		fprintf(stderr, "shell error: unable to access /proc file\n");
		return;
	}
		
	char *line = NULL;
	size_t len = 0;
	ssize_t size;
		
	while((size = getline(&line, &len, file)) != -1){						//use getline to read input from /proc
		printf("%s", line);													
	}
	printf("\n");
	free(line);
	fclose(file);
}	
	
	
	







