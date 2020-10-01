#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h> 
#include <readline/history.h> 

#define MAXLENGTH 1000           // max line length
#define MAXLIST 100              // max commands
#define TOK_BUFSIZE 64           // max token size 
#define TOK_DELIM " \t\r\n\a"    // valid token delimiters

HIST_ENTRY **historyList;
char* env_home;

int getInput(char* str) {
    char* inputBuffer; 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd));
    printf("%s", cwd); 
    inputBuffer = readline("> "); 
    if (strlen(inputBuffer) != 0) { 
        add_history(inputBuffer); 
        strcpy(str, inputBuffer); 
        return 0; 
    } 
    else { 
        return 1; 
    } 
}

int processInput(char* str, char** tokens) {
    int inputArgCount = 0;
    int bufsize = TOK_BUFSIZE;
    int position = 0;
    char *token;
    token = strtok(str, TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "msh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    return inputArgCount;
}

int internalCmdHandler(char** tokens) {
    int match = -1; 
    int internalCmdCount = 4;
    char* internalCmdList[internalCmdCount];

    internalCmdList[0] = "exit";
    internalCmdList[1] = "cd";
    internalCmdList[2] = "help";
    internalCmdList[3] = "history";

    for (int i = 0; i < internalCmdCount; i++) {
        if (strcmp(internalCmdList[i], tokens[0]) == 0) {
            match = i; 
            break;
        }
    }

    switch (match) {
        case 0: 
            write_history(".msh_history");
            exit(EXIT_SUCCESS);
            break;
        case 1:
            chdir(tokens[1]);
            break;
        case 2:
            // to-do
            break;
        case 3:
            historyList = history_list();
            if (historyList) {
                for (int i = 0; historyList[i]; i++) {
                    printf ("%d: %s\n", i + 1, historyList[i]->line);
                }
            }
            break;
    }
    return match;
}

int processCmd(char** str) {
    pid_t pid;
    pid_t wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(str[0], str) == -1) {
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) {
        printf("ERROR!");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main(int argc, char **argv)
{
    env_home = getenv("HOME");
    using_history();
    if (read_history(".msh_history") != 0) {
        printf("error reading history file\n");
    }
    char userInput[MAXLENGTH];
    char** tokens = malloc(TOK_BUFSIZE * sizeof(char*));
    int inputArgCount = 0; 
    char exitStr[5];
    int internalCmd;

    strcpy(exitStr, "exit"); 
    // main loop 
    while (1) {
        if (getInput(userInput)) {
            continue;
        }
        inputArgCount = processInput(userInput, tokens);
        if (internalCmdHandler(tokens) == -1) {
            processCmd(tokens);
        }        
    }
    return EXIT_SUCCESS;
}
