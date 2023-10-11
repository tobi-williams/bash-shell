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
    if (argc > 1){
    	fprintf(stderr, "Error: Do not enter command line arguments\n");
    	return 1;
    }
    
    else{
    	user_prompt_loop();
    	return 0;
    }
}


void user_prompt_loop() 
{
    char *input;
    char **parsedInput;
    
    while(1){
    	printf(">> ");
    	input = get_user_command();
    	if (input == NULL){
    	    exit(1);
    	}
    	//parsedInput = parse_command(input);
    	
    	break;
    }
    
    for(int i = 0; input[i] != '\0'; i++){ //testing input reading
    	printf("%c", input[i]);
    }
    
    exit(0);
}


char * get_user_command()
{
    char* input = NULL;
    size_t length = 0;
    
    ssize_t read = getline(&input, &length, stdin);
    
    if (read == -1){
    	perror("Input read failed: ");
    	free(input);
    	return NULL;
    }
    
    return input;
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

/*parse_command()*/
/////////////////////{
    /*
    Functions you may need: 
        malloc(), realloc(), free(), strlen(), first_unquoted_space(), unescape()
    */

    /*
    ENTER YOUR CODE HERE
    */
///////////////////}

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
