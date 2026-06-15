#ifndef HISTORY_H
#define HISTORY_H

void add_history(char *cmd);
void print_history();
char *get_history(int index);
int get_history_count();

#endif