#include <string.h>

#include "history.h"
#include "line_editor.h"

void history_up(
    char *input,
    int *pos,
    int *cursor_pos,
    int *history_index)
{
    int count = get_history_count();

    if (count <= 0)
    {
        return;
    }

    if (*history_index == -1)
    {
        *history_index = count - 1;
    }
    else if (*history_index > 0)
    {
        (*history_index)--;
    }

    strcpy(
        input,
        get_history(*history_index));

    *pos = strlen(input);
    *cursor_pos = *pos;

    redraw_line(
        input,
        *pos,
        *cursor_pos);
}

void history_down(
    char *input,
    int *pos,
    int *cursor_pos,
    int *history_index)
{
    int count = get_history_count();

    if (count <= 0 ||
        *history_index == -1)
    {
        return;
    }

    if (*history_index < count - 1)
    {
        (*history_index)++;

        strcpy(
            input,
            get_history(*history_index));

        *pos = strlen(input);
        *cursor_pos = *pos;
    }
    else
    {
        *history_index = -1;

        input[0] = '\0';

        *pos = 0;
        *cursor_pos = 0;
    }

    redraw_line(
        input,
        *pos,
        *cursor_pos);
}