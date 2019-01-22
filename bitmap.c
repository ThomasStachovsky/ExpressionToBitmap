#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitmap.h"

void generateBitmap(image *alphabet, node *current_node, image *output)
{
    if (current_node == NULL)
        return;

    if (current_node->value.type == OPERATOR)
    {
        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            (*output) = mergeBitmap(*output, alphabet['(']);

        generateBitmap(alphabet, current_node->left, output);

        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            (*output) = mergeBitmap(*output, alphabet[')']);
        (*output) = mergeBitmap(*output, alphabet[current_node->value.expression[0]]);

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            (*output) = mergeBitmap(*output, alphabet['(']);

        generateBitmap(alphabet, current_node->right, output);
        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            (*output) = mergeBitmap(*output, alphabet[')']);
    }
    else
    {
        unsigned int iter = 0;
        while (current_node->value.expression[iter] != '\0')
        {
            (*output) = mergeBitmap(*output, alphabet[current_node->value.expression[iter]]);
            iter++;
        }
    }
}

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
    if (isImageEmpty(left))
        return copyImage(right);
    if (isImageEmpty(right))
        return copyImage(left);
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

void createEmptyImage(image empty)
{
    empty.map = NULL;
}

image copyImage(image original)
{
    image clone;
    getTypeP6(clone.magic_number);
    clone.width = original.width;
    clone.height = original.height;
    clone.map = (pixel *)malloc(clone.width * clone.height * sizeof(pixel));
    for (int i = 0; i < clone.width * clone.height; i++)
        clone.map[i] = original.map[i];
    return clone;
}
bool isImageEmpty(image candidate)
{
    if (candidate.map = NULL)
        return true;
    else
        return false;
}