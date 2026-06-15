#include <stdio.h>
#include <unistd.h>

void redraw_line(char *input, int pos, int cursor_pos)
{
    printf("\33[2K\r");
    printf("shivam-shell> %s", input);

    fflush(stdout);

    int move_left = pos - cursor_pos;

    for (int i = 0; i < move_left; i++)
    {
        write(STDOUT_FILENO, "\033[D", 3);
    }
}