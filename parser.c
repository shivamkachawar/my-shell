#include <string.h>
#include "parser.h"

void parse_input(char *input, char **args)
{
    int i = 0;

    char *token = strtok(input, " \n");

    while(token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }

    args[i] = NULL;
}