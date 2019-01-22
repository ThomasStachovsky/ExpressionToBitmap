#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitmap.h"

void generateBitmapFromTree(image *alphabet, node *current_node, image *output, double scale)
{
    if (current_node == NULL)
        return;
    if (current_node->value.type == OPERATOR)
    {
        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            *output = mergeBitmapAndFreeMemory(*output, alphabet['(']);

        generateBitmapFromTree(alphabet, current_node->left, output, scale);

        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
            *output = mergeBitmapAndFreeMemory(*output, alphabet[')']);
        *output = mergeBitmapAndFreeMemory(*output, alphabet[current_node->value.expression[0]]);

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            *output = mergeBitmapAndFreeMemory(*output, alphabet['(']);

        generateBitmapFromTree(alphabet, current_node->right, output, scale);

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
            *output = mergeBitmapAndFreeMemory(*output, alphabet[')']);
    }
    else
    {
        unsigned int iter = 0;
        while (current_node->value.expression[iter] != '\0')
        {
            *output = mergeBitmapAndFreeMemory(*output, alphabet[current_node->value.expression[iter]]);
            iter++;
        }
    }
}

image mergeDownscaledBitmap(image *alphabet, image left, image right, double scale)
{
    //image downscaled = createDownscaledImage(right, scale);
    //
}

image generateBitmapFromTextDEBUG(image *alphabet, const char *expression)
{
    image sequence[STRING_SIZE];
    unsigned int length = strlen(expression);
    int i;
    for (i = 1; i < STRING_SIZE && i <= length; i++)
    {
        sequence[i] = mergeBitmapAndFreeMemory(sequence[i - 1], alphabet[expression[i - 1]]);
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
    setTypeP6(result.magic_number);
    //printf("\nLEFT %d RIGHT %d MAXINT %d\n", left.height, right.height, maxInt(left.height, right.height));
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
            result.map[i * result.width + j + left.width] = right.map[i * right.width + j];
    return result;
}

image mergeBitmapAndFreeMemory(image left, image right)
{
    image result = mergeBitmap(left, right);
    deleteBitmap(&left);
    return result;
}

void deleteBitmap(image *bitmap)
{
    free(bitmap->map);
    *bitmap = createEmptyImage();
}

void setTypeP6(char *magic_number)
{
    magic_number[0] = 'P';
    magic_number[1] = '6';
}

image createEmptyImage()
{
    image result;
    result.magic_number[0] = '\0';
    result.height = 0;
    result.width = 0;
    result.maxval = 255;
    result.map = NULL;
    return result;
}

image copyImage(image original)
{
    image clone;
    setTypeP6(clone.magic_number);
    clone.width = original.width;
    clone.height = original.height;
    clone.maxval = original.maxval;
    clone.map = (pixel *)malloc(clone.width * clone.height * sizeof(pixel));
    for (int i = 0; i < clone.width * clone.height; i++)
        clone.map[i] = original.map[i];
    return clone;
}
bool isImageEmpty(image candidate)
{
    if (candidate.map == NULL)
        return true;
    else
        return false;
}