#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "builtin.h"
#include "history.h"

int execute_builtin(char **args)
{
    if(args[0] == NULL)
        return 1;

    if(strcmp(args[0], "history") == 0)
    {
        print_history();
        return 1;
    }

    if(strcmp(args[0], "cd") == 0)
    {
        if(args[1] == NULL)
        {
            char *home = getenv("HOME");

            if(home)
                chdir(home);
        }
        else
        {
            if(chdir(args[1]) != 0)
                perror("cd");
        }

        return 1;
    }

    if(strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }

    return 0;
}