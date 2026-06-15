#ifndef REDIRECTION_H
#define REDIRECTION_H

void parse_redirection(
    char **args,
    char **input_file,
    char **output_file,
    int *append_mode,
    int *background);

void apply_redirection(
    char *input_file,
    char *output_file,
    int append_mode);

#endif