#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "history.h"

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
    static int history_index = -1;

    while (1)
    {
        read(STDIN_FILENO, &c, 1);

        if (c == 27)
        {
            char seq[2];

            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[' && seq[1] == 'A')
            {
                int count = get_history_count();

                if (count > 0)
                {
                    if (history_index == -1)
                    {
                        history_index = count - 1;
                    }
                    else if (history_index > 0)
                    {
                        history_index--;
                    }

                    strcpy(input, get_history(history_index));

                    pos = strlen(input);

                    printf("\33[2K\r");
                    printf("shivam-shell> %s", input);
                    fflush(stdout);
                }
            }
            else if (seq[0] == '[' && seq[1] == 'B')
            {
                int count = get_history_count();

                if (count > 0 && history_index != -1)
                {
                    if (history_index < count - 1)
                    {
                        history_index++;

                        strcpy(input, get_history(history_index));

                        pos = strlen(input);
                    }
                    else
                    {
                        history_index = -1;

                        input[0] = '\0';

                        pos = 0;
                    }

                    printf("\33[2K\r");
                    printf("shivam-shell> %s", input);
                    fflush(stdout);
                }
            }

            continue;
        }

        if (c == '\n')
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (c == '\t')
        {
            int start = pos - 1;

            while (start >= 0 && input[start] != ' ')
            {
                start--;
            }

            start++;

            char prefix[100];
            int k = 0;

            for (int i = start; i < pos; i++)
            {
                prefix[k++] = input[i];
            }

            prefix[k] = '\0';

            DIR *dir = opendir(".");

            struct dirent *entry;
            int matches = 0;

            char first_match[256];

            char all_matches[100][256];

            while ((entry = readdir(dir)) != NULL)
            {
                if (strncmp(entry->d_name,
                            prefix,
                            strlen(prefix)) == 0)
                {
                    strcpy(all_matches[matches], entry->d_name);

                    if (matches == 0)
                    {
                        strcpy(first_match, entry->d_name);
                    }

                    matches++;
                }
            }

            closedir(dir);

            if (matches == 1)
            {
                struct stat st;

                stat(first_match, &st);

                if (S_ISDIR(st.st_mode))
                {
                    sprintf(&input[start], "%s/", first_match);

                    pos = start + strlen(first_match) + 1;
                }
                else
                {
                    strcpy(&input[start], first_match);

                    pos = start + strlen(first_match);
                }

                printf("\r");
                printf("shivam-shell> %s", input);
                fflush(stdout);
            }
            else if (matches > 1)
            {
                printf("\n");

                for (int i = 0; i < matches; i++)
                {
                    struct stat st;

                    stat(all_matches[i], &st);

                    if (S_ISDIR(st.st_mode))
                    {
                        printf("%s/\n", all_matches[i]);
                    }
                    else
                    {
                        printf("%s\n", all_matches[i]);
                    }
                }

                printf("\nshivam-shell> %s", input);

                fflush(stdout);
            }
            continue;
        }

        if (c == 127)
        {
            if (pos > 0)
            {
                pos--;

                write(STDOUT_FILENO, "\b \b", 3);
            }

            continue;
        }

        input[pos++] = c;

        write(STDOUT_FILENO, &c, 1);
    }

    input[pos] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
