#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#include "history.h"
#include "input.h"
#include "autocomplete.h"
#include "line_editor.h"
#include "history_navigation.h"

void read_input(char *input)
{
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int pos = 0;
    int cursor_pos = 0;
    input[0] = '\0';

    char c;

    static int history_index = -1;

    while (1)
    {
        read(STDIN_FILENO, &c, 1);

        // Ctrl+A
        if (c == 1)
        {
            while (cursor_pos > 0)
            {
                write(STDOUT_FILENO, "\033[D", 3);
                cursor_pos--;
            }

            continue;
        }

        // Ctrl+E
        if (c == 5)
        {
            while (cursor_pos < pos)
            {
                write(STDOUT_FILENO, "\033[C", 3);
                cursor_pos++;
            }

            continue;
        }

        // Ctrl+L
        if (c == 12)
        {
            printf("\033[2J\033[H");
            printf("shivam-shell> %s", input);

            fflush(stdout);

            continue;
        }

        // Ctrl+U
        if (c == 21)
        {
            input[0] = '\0';

            pos = 0;
            cursor_pos = 0;

            printf("\33[2K\r");
            printf("shivam-shell> ");

            fflush(stdout);

            continue;
        }

        // Arrow keys
        if (c == 27)
        {
            char seq[2];

            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            // Up Arrow
            if (seq[0] == '[' && seq[1] == 'A')
            {
                history_up(
                    input,
                    &pos,
                    &cursor_pos,
                    &history_index);
            }

            // Down Arrow
            else if (seq[0] == '[' && seq[1] == 'B')
            {
                history_down(
                    input,
                    &pos,
                    &cursor_pos,
                    &history_index);
            }

            // Left Arrow
            else if (seq[0] == '[' &&
                     seq[1] == 'D')
            {
                if (cursor_pos > 0)
                {
                    write(STDOUT_FILENO,
                          "\033[D",
                          3);

                    cursor_pos--;
                }
            }

            // Right Arrow
            else if (seq[0] == '[' &&
                     seq[1] == 'C')
            {
                if (cursor_pos < pos)
                {
                    write(STDOUT_FILENO,
                          "\033[C",
                          3);

                    cursor_pos++;
                }
            }

            continue;
        }

        // Enter
        if (c == '\n')
        {
            history_index = -1;

            write(STDOUT_FILENO,
                  "\n",
                  1);

            break;
        }

        // TAB Completion
        if (c == '\t')
        {
            autocomplete(
                input,
                &pos,
                &cursor_pos);

            continue;
        }

        // Backspace
        if (c == 127)
        {
            if (cursor_pos > 0)
            {
                for (int i = cursor_pos - 1;
                     i < pos;
                     i++)
                {
                    input[i] = input[i + 1];
                }

                pos--;
                cursor_pos--;

                redraw_line(
                    input,
                    pos,
                    cursor_pos);
            }

            continue;
        }

        // Character insertion
        for (int i = pos;
             i >= cursor_pos;
             i--)
        {
            input[i + 1] = input[i];
        }

        input[cursor_pos] = c;

        pos++;
        cursor_pos++;

        redraw_line(
            input,
            pos,
            cursor_pos);
    }

    input[pos] = '\0';

    tcsetattr(
        STDIN_FILENO,
        TCSANOW,
        &oldt);
}