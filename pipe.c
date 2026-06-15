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
    int count_pipes(char **args);
    int num_pipes = count_pipes(args);
    int num_commands = num_pipes + 1;

    char *commands[10][100];

    int cmd_idx = 0;
    int arg_idx = 0;

    // Split commands
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            commands[cmd_idx][arg_idx] = NULL;
            cmd_idx++;
            arg_idx = 0;
        }
        else
        {
            commands[cmd_idx][arg_idx++] = args[i];
        }
    }

    commands[cmd_idx][arg_idx] = NULL;

    // Create pipes
    int pipes[num_pipes][2];

    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            return;
        }
    }

    pid_t pids[num_commands];

    // Create all processes
    for (int i = 0; i < num_commands; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0)
        {
            // First command
            if (i == 0)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Last command
            else if (i == num_commands - 1)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            // Middle command
            else
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe FDs
            for (int j = 0; j < num_pipes; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(commands[i][0], commands[i]);

            perror("execvp");
            exit(1);
        }
    }

    // Parent closes all pipes
    for (int i = 0; i < num_pipes; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
}
int count_pipes(char **args)
{
    int count = 0;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            count++;
        }
    }

    return count;
}