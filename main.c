#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

#include "parser.h"
#include "builtin.h"
#include "executor.h"
#include "history.h"
#include "input.h"

int main()
{
    char input[1024];
    signal(SIGINT, SIG_IGN);

    while (1)
    {
        while (waitpid(-1, NULL, WNOHANG) > 0)
        {
        }
        char *args[100];

        printf("shivam-shell> ");
        fflush(stdout);

        read_input(input);

        add_history(input);

        parse_input(input, args);

        if (execute_builtin(args))
            continue;

        execute_command(args);
    }

    return 0;
}