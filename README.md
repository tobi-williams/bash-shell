[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/xR-cYv8r)
# project1
Answer these questions please:
Tell me what this project is about?
Tell me how your thought process for completing the project?
Any issues you came across?


The aim of this project is to create a C program that imitates some of the basic functionalities of a Unix/Linux shell. This shell implements functionalities like accepting, interpreting and executing commands entered by the user. It also implements functionalities for accessing the /proc file system. It also has other built in features such as "exit" and "history which close the shell and provide the most recent commands up to 10 entries, respectively.

I began by implementing the prompt loop, which led to the user input function, then the parse command function. After those, I worked on the exit command which was where the memory handling really began to show. I implemented the command executions and then went on to build the history and proc functions. I created several helper functions to help with organization and readability.

My biggest issue was memory handling. I still wasnt able to get rid of all the leaks. All the functionalities seem to work fine however i think my memory problems stem from inefficient malloc-ing in get_user_input, though after several hours of debugging I couldnt seem to crack it.
