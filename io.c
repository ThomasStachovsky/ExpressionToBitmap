#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "io.h"

void printGettingStarted()
{
    printf(CYAN "ExpToImg by Tomasz Stachowski\n\n" RESET);
    printf(YELLOW "Program converts a given mathematical expression to an image.\n");
    printf("While writing the formula you can use:\n");
    printf("+ - * / as arithmetic operators\n^ as superscript operator");
    printf("\n_ as subscript operator\nstrings of letters and digits as variables\n");
    printf("($2,$3,...,$n) as multivariable functions where $1...n are expressions as arguments and ,(comma) separates them\n= > < as comparators\n\n" RESET);
}

void printTypeExpression()
{
    printf("Type the expression you want to print:\n");
}

bool scanExpression(char *expression)
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
        printf(RED "Empty expression, nothing to print.\nPress any key to exit..." RESET);
        getchar();
        return false;
    }
    else if (expression_iterator == STRING_SIZE)
    {
        printf(RED "The expression is too long.\nPress any key to exit..." RESET);
        getchar();
        return false;
    }
    expression[expression_iterator] = '\0';
    return true;
}

void printQuestionForPath()
{
    printf("Type the name or path/name for the output file:\n");
}

bool scanPath(char *path)
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
        printf(RED "Empty path, nothing to print.\nPress any key to exit..." RESET);
        getchar();
        return false;
    }
    else if (path_iterator == STRING_SIZE)
    {
        printf(RED "The path is too long.\nPress any key to exit..." RESET);
        getchar();
        return false;
    }
    path[path_iterator] = '\0';
    return true;
}

void printDoneAndQuestionForAgain()
{
    printf(GREEN "Expression printed. Press y/Y to print another expression or n/N to exit...\n" RESET);
}

bool scanIfAgain()
{
    char character;
    while (true)
    {
        character = getchar();
        clearStdin();
        if (character == 'y' || character == 'Y')
            return true;
        else if (character == 'n' || character == 'N')
            return false;
        printf("I do not understand. Type y/Y or n/N\n");
    }
}

bool doesFileExist(char *filename)
{
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return true;
    }
    return false;
}

image *scanAlphabet()
{
    image *alphabet = (image *)malloc(sizeof(image) * 256);
    for (int i = 0; i < 256; i++)
        alphabet[i] = createEmptyImage();
    char filename[STRING_SIZE];
    const char *EXTENSION = ".ppm";
    const char *SLASH_FILENAME = "SLASH";
    const char *ALPHABET_DIRECTORY = "./alphabet/";
    for (int i = 1; i < 256; i++)
    {
        if (i == '/')
            sprintf(filename, "%s%s%s", ALPHABET_DIRECTORY, SLASH_FILENAME, EXTENSION);
        else
            sprintf(filename, "%s%c%s", ALPHABET_DIRECTORY, i, EXTENSION);
        if (doesFileExist(filename))
        {
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

void printImage(image bitmap, const char *original_path)
{
    char path[STRING_SIZE];
    strcpy(path, original_path);
    const char *EXTENSION = ".ppm";
    unsigned int path_length = strlen(path);
    unsigned int extension_length = strlen(EXTENSION);
    if (path_length <= extension_length || (path_length > extension_length && (path[path_length - 1] != 'm' || path[path_length - 2] != 'p' || path[path_length - 3] != 'p' || path[path_length - 4] != '.')))
        sprintf(path, "%s%s", path, EXTENSION);
    if (doesFileExist(path))
    {
        printf(RED "This file already exists. For your safety please type the path again or type another one:\n" RESET);
        scanf("%s", path);
        path_length = strlen(path);
        if (path_length <= extension_length || (path_length > extension_length && (path[path_length - 1] != 'm' || path[path_length - 2] != 'p' || path[path_length - 3] != 'p' || path[path_length - 4] != '.')))
            sprintf(path, "%s%s", path, EXTENSION);
        getchar();
    }

    FILE *file = fopen(path, "wb");
    fprintf(file, "%c%c\n", bitmap.magic_number[0], bitmap.magic_number[1]);
    fprintf(file, "# Created by Tomasz Stachowski's program\n");
    fprintf(file, "%d %d\n", bitmap.width, bitmap.height);
    fprintf(file, "%d\n", bitmap.maxval);
    fwrite(bitmap.map, 3 * bitmap.width, bitmap.height, file);
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