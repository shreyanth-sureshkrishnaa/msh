
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];

    if (pipe(pipefd) == -1){
        perror("Pipe Failed!");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0){
        printf("Fork failed!");
        return 1;
    }

    if (pid == 0){

        // The child only needs the read end. 

        close(pipefd[1]);

        char buffer[100];

        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);

        if (bytes_read == -1){
            perror("Read failed!");
            close(pipefd[0]);
            _exit(1);
        }

        buffer[bytes_read] = '\0';

        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
        _exit(0);
    }

    // Parent only needs the write end. 

    close(pipefd[0]);

    const char *message = "Hello from parent!";

    if (write(pipefd[1], message, strlen(message)) == -1){
        perror("Write failed!");
        close(pipefd[1]);
        waitpid(pid, NULL, 0);
        return 1;
    }

    close(pipefd[1]);
    waitpid(pid, NULL, 0);

    printf("Parent has finished!");
    return 0;
}