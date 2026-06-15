#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "pipe.h"

int has_pipe(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            return i;
        }
    }

    return -1;
}

void execute_pipe(char **args)
{
    int pipe_index = has_pipe(args);

    char *left_args[100];
    char *right_args[100];

    int left_idx = 0;
    int right_idx = 0;

    for (int i = 0; i < pipe_index; i++)
    {
        left_args[left_idx++] = args[i];
    }
    left_args[left_idx] = NULL;

    for (int i = pipe_index + 1; args[i] != NULL; i++)
    {
        right_args[right_idx++] = args[i];
    }
    right_args[right_idx] = NULL;

    int fd[2];

    if (pipe(fd) < 0)
    {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();

    if (pid1 == 0)
    {
        close(fd[0]);

        dup2(fd[1], STDOUT_FILENO);

        close(fd[1]);

        execvp(left_args[0], left_args);

        perror("execvp");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 == 0)
    {
        close(fd[1]);

        dup2(fd[0], STDIN_FILENO);

        close(fd[0]);

        execvp(right_args[0], right_args);

        perror("execvp");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}