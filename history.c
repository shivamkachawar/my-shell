#include <stdio.h>
#include <string.h>
#include "history.h"

static char history[1000][1024];
static int history_count = 0;

void add_history(char *cmd)
{
    strcpy(history[history_count++], cmd);
}

void print_history()
{
    for(int i = 0; i < history_count; i++)
    {
        printf("%d %s", i + 1, history[i]);
    }
}