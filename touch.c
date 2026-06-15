#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main()
{
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char c;

    while (1)
    {
        read(STDIN_FILENO, &c, 1);

        printf("ASCII = %d\n", c);

        if (c == 'q')
            break;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}