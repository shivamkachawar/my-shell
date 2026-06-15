#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#include "process.h"
#include "redirection.h"

void execute_simple_command(
    char **args,
    char *input_file,
    char *output_file,
    int append_mode,
    int background)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        apply_redirection(
            input_file,
            output_file,
            append_mode);

        execvp(args[0], args);

        perror("Command failed");
        exit(1);
    }
    else if (pid > 0)
    {
        if (background)
        {
            printf("[Background] PID : %d\n", pid);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }
    else
    {
        perror("fork failed");
    }
}