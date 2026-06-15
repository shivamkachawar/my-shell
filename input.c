#include <unistd.h>
#include <termios.h>
#include <stdio.h>

void read_input(char *input)
{
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int pos = 0;
    char c;

    while (1)
    {
        read(STDIN_FILENO, &c, 1);

        if (c == '\n')
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (c == '\t')
        {
            printf("\nTAB DETECTED\n");
            continue;
        }

        input[pos++] = c;

        write(STDOUT_FILENO, &c, 1);
    }

    input[pos] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}