#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stack.h"
#include "ls2.h"

/**
 * MODE 1
 *
 * This function reads the current directory and prints
 * every file or directory it encounters.
 *
 * For some reason, this reads the last file first. It has
 * to be a recursion issue but I can't understand why.
 */
void printDir(char *path, int depth)
{
    DIR *dir = opendir(path);
    struct dirent *returnVal; // to store readdir()'s output
    struct stat fstat;        // store file attributes

    if (!dir) // make sure the directory exists / is valid
    {
        fprintf(stderr, "Error opening directory, %s\n", path);
        return;
    }

    while ((returnVal = readdir(dir)) != NULL)
    {
        // if the current return value is equal to '.' or '..', skip it
        if (strcmp(returnVal->d_name, ".") == 0 || strcmp(returnVal->d_name, "..") == 0)
        {
            continue;
        }

        // have to keep track of depth
        for (int i = 0; i < depth; i++)
        {
            printf(INDENT);
        }

        char dirPath[MAX];                                                    // create a character array to hold the path
        snprintf(dirPath, sizeof(dirPath), "%s/%s", path, returnVal->d_name); // print readdir()'s outputs
        stat(dirPath, &fstat);

        // if its a directory
        if (S_ISDIR(fstat.st_mode))
        {
            printf("%s/ (directory)\n", returnVal->d_name);
            printDir(dirPath, depth + 1); // recurse
        }
        else
        { // its a file
            printf("%s (%ld bytes)\n", returnVal->d_name, fstat.st_size);
        }
    }
    closedir(dir); // close the directory
}

// Helper functions for mode 2:

char *pTs(void *ptr)
{
    return strdup((char *)ptr);
}

void output(stack_t *s)
{
    void *popped;
    while ((popped = pop(s)) != NULL)
    {
        char *poppedString = pTs(popped);
        int counter = 1;
        for (int i = 2; poppedString[i] != '\0'; i++)
        {
            printf("%c", poppedString[i]);
            if (poppedString[i] == '/')
            {
                printf(" (directory)");
                printf("\n");
                for (int i = 0; i < counter; i++)
                {
                    printf(INDENT);
                }
                counter++;
            }
        }
        printf("\n");
        free(poppedString);
    }
}

/**
 * MODE 2
 *
 * This function reads the current directory and prints
 * every file or directory it encounters that matches
 * the pattern given.
 */
void printModDir(char *path, char *match, int depth)
{
    DIR *dir = opendir(path);
    struct dirent *returnVal; // to store readdir()'s output
    struct stat fstat;        // store file attributes

    if (!dir) // make sure the directory exists/is valid
    {
        fprintf(stderr, "Error opening directory, %s\n", path);
        return;
    }

    stack_t *s = initstack(); // store the files to print

    while ((returnVal = readdir(dir)) != NULL)
    {
        char dirPath[MAX];                                                    // create a character array to hold the path
        snprintf(dirPath, sizeof(dirPath), "%s/%s", path, returnVal->d_name); // print readdir()'s outputs

        // if the current return value is equal to '.' or '..', skip it
        if (strcmp(returnVal->d_name, ".") == 0 || strcmp(returnVal->d_name, "..") == 0)
            continue;

        if (stat(dirPath, &fstat) < 0)
        {
            fprintf(stderr, "Error stating file %s\n", dirPath);
            continue;
        }

        if (S_ISDIR(fstat.st_mode))
        {
            printModDir(dirPath, match, depth + 1); // recurse
        }
        else if (S_ISREG(fstat.st_mode))
        {
            if (strcmp(returnVal->d_name, match) == 0)
            {
                push(s, strdup(dirPath)); // should not be pushing the entire path onto stack
            }
        }
    }
    closedir(dir); // close the directory

    output(s);
    freestack(s);
}
