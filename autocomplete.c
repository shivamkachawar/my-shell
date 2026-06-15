#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

void autocomplete(char *input,
                  int *pos,
                  int *cursor_pos)
{
    int start = *pos - 1;

    while (start >= 0 && input[start] != ' ')
    {
        start--;
    }

    start++;

    char prefix[100];
    int k = 0;

    for (int i = start; i < *pos; i++)
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
            strcpy(all_matches[matches],
                   entry->d_name);

            if (matches == 0)
            {
                strcpy(first_match,
                       entry->d_name);
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
            sprintf(&input[start],
                    "%s/",
                    first_match);

            *pos = start +
                   strlen(first_match) +
                   1;
        }
        else
        {
            strcpy(&input[start],
                   first_match);

            *pos = start +
                   strlen(first_match);
        }

        *cursor_pos = *pos;

        printf("\r");
        printf("shivam-shell> %s",
               input);

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
                printf("%s/\n",
                       all_matches[i]);
            }
            else
            {
                printf("%s\n",
                       all_matches[i]);
            }
        }

        printf("\nshivam-shell> %s",
               input);

        fflush(stdout);
    }
}