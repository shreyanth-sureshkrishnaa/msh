
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    printf("stdin  = %d\n", STDIN_FILENO);
    printf("stdout = %d\n", STDOUT_FILENO);
    printf("stderr = %d\n", STDERR_FILENO);

    int fd = open("hello.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    printf("File descriptor = %d\n", fd);

    write(fd, "Hello from Linux!\n", 18);

    close(fd);

    return 0;
}
