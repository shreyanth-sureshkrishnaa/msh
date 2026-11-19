#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main(void) {
    char input[MAX_INPUT];

    while(1){

        // The user prompt:
        printf("msh$ ");
        fflush(stdout); // Flushing out the entire stdout buffer

        // Obtaining user input: 
        if (fgets(input, sizeof(input), stdin) == NULL){
            printf("\n");
            break;
        }

        // Strip the trailing newline: 
        input[strcspn(input, "\n")] = '\0';

        // Ignore any empty commands:
        if(input[0] == '\0'){
            continue;
        }

        // Tokenize the input string:
        char *args[MAX_ARGS];
        int argc = 0;

        char *saveptr;
        char *token = strtok_r(input, " \t", &saveptr);

        while (token != NULL && argc < MAX_ARGS - 1){
            args[argc++] = token;
            token = strtok_r(NULL, " \t", &saveptr);
        }

        args[argc] = NULL;

        if (token != NULL) {
            fprintf(stderr, "Too many arguments!\n");
            continue;
        }

        // Handle exits:
        if (strcmp(args[0], "exit") == 0){
            break;
        }

        // Create child processes:
        pid_t pid = fork();

        if (pid < 0){
            perror("fork");
            continue;
        }

        if (pid == 0){
            execvp(args[0], args);

            perror("execvp");
            _exit(127);
        }

        // Parent should wait for child:
        int status;
        pid_t result;

        do {
            result = waitpid(pid, &status, 0);
        } while (result == -1 && errno == EINTR); // Do not treat interruptions as fatal errors.

        if (result == -1){
            perror("waitpid");
        }

        else if (WIFEXITED(status)) {
            printf("[msh] Exit status: %d\n", WEXITSTATUS(status));
        }

        else if (WIFSIGNALED(status)) {
            printf("[msh] Terminated by signal: %d\n", WTERMSIG(status));
        }
    }

    printf("Exiting msh.\n");
    return 0;
}