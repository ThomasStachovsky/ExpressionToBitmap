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
    double new_scale = scale;
    if (current_node->value.type == OPERATOR)
    {
        if (current_node->value.expression[0] == '^')
            new_scale = 0.7;
        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
        {

            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet['('], scale);
            *output = copyImage(temp);
        }
        generateBitmapFromTree(alphabet, current_node->left, output, scale);

        if ((current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0])))
        {
            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet[')'], scale);
            *output = copyImage(temp);
        }
        if (current_node->value.expression[0] != '^')
        {
            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet[current_node->value.expression[0]], scale);
            *output = copyImage(temp);
        }

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
        {
            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet['('], new_scale);
            *output = copyImage(temp);
        }
        generateBitmapFromTree(alphabet, current_node->right, output, new_scale);

        if ((current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0])) ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0])))
        {
            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet[')'], new_scale);
            *output = copyImage(temp);
        }
    }
    else
    {
        unsigned int iter = 0;
        while (current_node->value.expression[iter] != '\0')
        {
            image temp = mergeBitmapAndFreeMemory(copyImage(*output), alphabet[current_node->value.expression[iter]], new_scale);
            *output = copyImage(temp);

            iter++;
        }
    }
}

image mergeDownscaledBitmap(image *alphabet, image left, image right, double scale)
{
    //image downscaled = createDownscaledImage(right, scale);
    //
}

image createDownscaledImage(image original, double scale)
{
    image downscaled;
    downscaled.height = (int)((double)original.height * scale);
    downscaled.width = (int)((double)original.width * scale);
    downscaled.maxval = original.maxval;
    setTypeP6(downscaled.magic_number);
    downscaled.map = (pixel *)malloc(downscaled.width * downscaled.height * sizeof(pixel));
    double inverted_scale = 1.0 / scale;
    //for (int i = 0; i < downscaled.height; i++)
    //   for (int j = 0; j < downscaled.width; j++)
    //        downscaled.map[i * downscaled.width + j] = original.map[(int)((double)i * original.width * inverted_scale + (double)j * inverted_scale)];
    for (int i = 0; i < downscaled.height * downscaled.width; i++)
        downscaled.map[i] = original.map[(int)((double)i * inverted_scale * inverted_scale)];
    return downscaled;
}

image generateBitmapFromTextDEBUG(image *alphabet, const char *expression)
{
    image sequence[STRING_SIZE];
    unsigned int length = strlen(expression);
    int i;
    for (i = 1; i < STRING_SIZE && i <= length; i++)
    {
        sequence[i] = mergeBitmapAndFreeMemory(sequence[i - 1], alphabet[expression[i - 1]], 1.0);
    }
    return sequence[i - 1];
}

image mergeBitmapHorizontal(image left, image right)
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

image mergeBitmapVertical(image top, image bottom)
{
    if (isImageEmpty(top))
        return copyImage(bottom);
    if (isImageEmpty(bottom))
        return copyImage(top);
    image result;
    setTypeP6(result.magic_number);
    result.height = top.height + bottom.height;
    result.width = maxInt(top.width, bottom.width);
    result.maxval = maxInt(top.maxval, bottom.maxval);
    result.map = (pixel *)malloc(result.width * result.height * sizeof(pixel));
    for (int i = 0; i < result.width * result.height; i++)
    {
        result.map[i].red = 255;
        result.map[i].green = 255;
        result.map[i].blue = 255;
    }
    for (int i = 0; i < top.height; i++)
        for (int j = 0; j < top.width; j++)
            result.map[i * result.width + j] = top.map[i * top.width + j];

    for (int i = 0; i < bottom.height; i++)
        for (int j = 0; j < bottom.width; j++)
            result.map[i * result.width + j + top.height * result.width] = bottom.map[i * bottom.width + j];
    return result;
}

image mergeBitmapAndFreeMemory(image left, image right, double scale)
{
    image downscaled = createDownscaledImage(right, scale);
    image result = mergeBitmapHorizontal(copyImage(left), copyImage(downscaled));
    deleteBitmap(&left);
    deleteBitmap(&downscaled);
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