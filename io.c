#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "io.h"

void printGettingStarted()
{
    printf("Debug: Type the expression:\n");
}

void scanExpression(char *expression)
{
    char character;
    unsigned int expression_iterator = 0;
    character = getchar();
    while (character != '\n' && expression_iterator < STRING_SIZE)
    {
        expression[expression_iterator] = character;
        expression_iterator++;
        character = getchar();
    }
    if (expression_iterator == 0)
    {
        printf("Empty expression, nothing to print.\nPress any key to exit...");
        getchar();
        exit(0);
    }
    else if (expression_iterator == STRING_SIZE)
    {
        printf("The expression is too long.\nPress any key to exit...");
        getchar();
        exit(0);
    }
    else
        expression[expression_iterator] = '\0';
}

void printQuestionForPath()
{
    printf("Debug: Type the path:\n");
}

void scanPath(char *path)
{
    char character;
    unsigned int path_iterator = 0;
    character = getchar();
    while (character != '\n' && path_iterator < STRING_SIZE)
    {
        path[path_iterator] = character;
        path_iterator++;
        character = getchar();
    }
    if (path_iterator == 0)
    {
        printf("Empty expression, nothing to print.\nPress any key to exit...");
        getchar();
        exit(0);
    }
    else if (path_iterator == STRING_SIZE)
    {
        printf("The expression is too long.\nPress any key to exit...");
        getchar();
        exit(0);
    }
    else
        path[path_iterator] = '\0';
}

void printDoneAndQuestionForAgain()
{
    printf("Debug: Printed. Press y/Y to print another expression or n/N to exit...");
}

bool scanIfAgain()
{
    char character = getchar();
    while (true)
    {
        if (character == 'y' || character == 'Y')
            return true;
        else if (character == 'n' || character == 'N')
            return false;
        else
            printf("I do not understand. Type y/Y or n/N\n");
    }
}