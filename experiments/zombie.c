#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid = fork();

    if (pid == 0) {
        printf("Child exiting. PID = %d\n", getpid());
        _exit(0);
    }
    else if (pid > 0) {
        printf("Parent sleeping. PID = %d\n", getpid());
        sleep(30);
    }

    return 0;
}