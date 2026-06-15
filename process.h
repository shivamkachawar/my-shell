#ifndef PROCESS_H
#define PROCESS_H

void execute_simple_command(
    char **args,
    char *input_file,
    char *output_file,
    int append_mode,
    int background);

#endif