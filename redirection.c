#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "redirection.h"

void parse_redirection(
    char **args,
    char **input_file,
    char **output_file,
    int *append_mode,
    int *background)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            *output_file = args[i + 1];
            args[i] = NULL;
            break;
        }

        if (strcmp(args[i], ">>") == 0)
        {
            *output_file = args[i + 1];
            *append_mode = 1;
            args[i] = NULL;
            break;
        }

        if (strcmp(args[i], "<") == 0)
        {
            *input_file = args[i + 1];
            args[i] = NULL;
            break;
        }

        if (strcmp(args[i], "&") == 0)
        {
            *background = 1;
            args[i] = NULL;
            break;
        }
    }
}
void apply_redirection(
    char *input_file,
    char *output_file,
    int append_mode)
{
    if (output_file != NULL)
    {
        int flags;

        if (append_mode)
        {
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        else
        {
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }

        int fd = open(output_file, flags, 0644);

        if (fd < 0)
        {
            perror("open");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);
    }

    if (input_file != NULL)
    {
        int fd = open(input_file, O_RDONLY);

        if (fd < 0)
        {
            perror("open");
            exit(1);
        }

        dup2(fd, STDIN_FILENO);

        close(fd);
    }
}