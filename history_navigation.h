#ifndef HISTORY_NAVIGATION_H
#define HISTORY_NAVIGATION_H

void history_up(
    char *input,
    int *pos,
    int *cursor_pos,
    int *history_index);

void history_down(
    char *input,
    int *pos,
    int *cursor_pos,
    int *history_index);

#endif