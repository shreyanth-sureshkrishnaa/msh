#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (void) {

    char *args[] = {"ls", "-l", "-a", NULL}; // Argument Vector

    printf("Shell PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed!");
        return 1;
    }

    if (pid == 0){
        printf("Child PID: %d\n", getpid());
        execvp(args[0], args);
        _exit(127);
    }

    int status;

    if(waitpid(pid, &status, 0) < 0){
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("Command exited with status: %d\n",
               WEXITSTATUS(status));
    }

    else if (WIFSIGNALED(status)) {
        printf("Command terminated by signal: %d\n",
               WTERMSIG(status));
    }

    printf("Shell exiting.\n");

    return 0;
}