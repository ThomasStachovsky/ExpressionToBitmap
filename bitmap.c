#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitmap.h"

/*
image generateBitmap(image *alphabet, node *tree)
{


}*/

image generateBitmapFromTextDEBUG(image *alphabet, const char *expression)
{
    image sequence[STRING_SIZE];
    unsigned int length = strlen(expression);
    int i;
    for (i = 1; i < STRING_SIZE && i <= length; i++)
    {
        sequence[i] = mergeBitmap(sequence[i - 1], alphabet[expression[i - 1]]);
    }
    return sequence[i - 1];
}

image mergeBitmap(image left, image right)
{
    if (left.map == NULL)
        return right;
    if (right.map == NULL)
        return left;
    image result;
    getTypeP6(result.magic_number);
    printf("\nLEFT %d RIGHT %d MAXINT %d\n", left.height, right.height, maxInt(left.height, right.height));
    result.height = maxInt(left.height, right.height);
    result.width = left.width + right.width;
    result.maxval = maxInt(left.maxval, right.maxval);
    result.map = (pixel *)malloc(result.width * result.height * sizeof(pixel));
    for (int i = 0; i < result.width * result.height; i++)
    {
        result.map[i].red = 255;
        result.map[i].green = 255;
        result.map[i].blue = 255;
    }

    for (int i = 0; i < left.height; i++)
        for (int j = 0; j < left.width; j++)
            result.map[i * result.width + j] = left.map[i * left.width + j];

    for (int i = 0; i < right.height; i++)
        for (int j = 0; j < right.width; j++)
            result.map[i * result.width + j + left.width] = right.map[i * right.width + j + left.width];
    return result;
}

void getTypeP6(char *magic_number)
{
    magic_number[0] = 'P';
    magic_number[1] = '6';
}

void createImage(image empty)
{
    empty.map = NULL;
}
bool isImageEmpty(image candidate)
{
    if (candidate.map = NULL)
        return true;
    else
        return false;
}