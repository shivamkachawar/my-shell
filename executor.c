#include <stddef.h>
#include "executor.h"
#include "pipe.h"
#include "process.h"
#include "redirection.h"

void execute_command(char **args)
{
    char *output_file = NULL;
    char *input_file = NULL;

    int append_mode = 0;
    int background = 0;

    parse_redirection(
        args,
        &input_file,
        &output_file,
        &append_mode,
        &background);

    if (has_pipe(args) != -1)
    {
        execute_pipe(args);
        return;
    }

    execute_simple_command(
        args,
        input_file,
        output_file,
        append_mode,
        background);
}