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
    printf(GREEN "Debug: Printed. Press y/Y to print another expression or n/N to exit..." RESET);
}

bool scanIfAgain()
{
    char character = getchar();
    while (true)
    {
        getchar();
        if (character == 'y' || character == 'Y')
            return true;
        else if (character == 'n' || character == 'N')
            return false;
        else
            printf("I do not understand. Type y/Y or n/N\n");
    }
}

bool doesFileExist(char *filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

image *scanAlphabet()
{
    image *alphabet = (image *)malloc(sizeof(image) * 256);
    char filename[STRING_SIZE];
    const char *EXTENSION = ".ppm";
    const char *SLASH_FILE_NAME = "SLASH";
    const char *ALPHABET_DIRECTORY = "./alphabet/";
    for (int i = 1; i < 256; i++)
    {
        if (i == '/')
            sprintf(filename, "%s%s%s", ALPHABET_DIRECTORY, SLASH_FILE_NAME, EXTENSION);
        else
            sprintf(filename, "%s%c%s", ALPHABET_DIRECTORY, i, EXTENSION);
        if (doesFileExist(filename))
        {
            printf("%s loaded to alphabet.\n", filename); //DEBUG
            FILE *file = fopen(filename, "rb");
            alphabet[i].magic_number[0] = fgetc(file);
            alphabet[i].magic_number[1] = fgetc(file);

            fgetc(file);
            int character = getc(file);
            while (character == '#')
            {
                while (getc(file) != '\n')
                    ;
                character = getc(file);
            }
            ungetc(character, file);
            fscanf(file, "%d %d", &alphabet[i].width, &alphabet[i].height);
            fscanf(file, "%d", &alphabet[i].maxval);
            while (fgetc(file) != '\n')
                ;
            alphabet[i].map = (pixel *)malloc(alphabet[i].width * alphabet[i].height * sizeof(pixel));
            fread(alphabet[i].map, 3 * alphabet[i].width, alphabet[i].height, file);
            fclose(file);
        }
    }

    return alphabet;
}

void printImage(image output, const char *original_path)
{
    char path[STRING_SIZE];
    strcpy(path, original_path);
    const char *EXTENSTION = ".ppm";
    sprintf(path, "%s%s", path, EXTENSTION);
    if (doesFileExist(path))
    {
        printf(RED "This file already exist. For your safety please type the path again or type another one:\n" RESET);
        scanf("%s", path);
        sprintf(path, "%s%s", path, EXTENSTION);
        getchar();
    }

    FILE *file = fopen(path, "wb");
    fprintf(file, "%c%c\n", output.magic_number[0], output.magic_number[1]);
    fprintf(file, "# Created by Tomasz Stachowski's program\n");
    fprintf(file, "%d %d\n", output.width, output.height);
    fprintf(file, "%d\n", output.maxval);
    fwrite(output.map, 3 * output.width, output.height, file);
    fclose(file);
}

void generateTextDEBUG(node *current_node)
{
    if (current_node == NULL)
        return;

    if (current_node->value.type == OPERATOR)
    {
        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            printf("(");

        generateTextDEBUG(current_node->left);

        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            printf(")");
        printf("%c", current_node->value.expression[0]);

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            printf("(");

        generateTextDEBUG(current_node->right);
        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            printf(")");
    }
    else
        printf("%s", current_node->value.expression);
}