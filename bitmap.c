#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitmap.h"

image generateBitmapFromTree(image *alphabet, node *current_node, double scale)
{
    if (current_node == NULL)
        return createEmptyImage();
    double new_scale = scale;
    image result = createEmptyImage();
    if (current_node->value.type == OPERATOR)
    {
        if (current_node->value.expression[0] == '^' || current_node->value.expression[0] == '_')
            new_scale = 0.5 * scale;
        else if (current_node->value.expression[0] == '/')
        {
            new_scale = 0.85 * scale;
            scale = new_scale;
        }
        if (current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0]) && current_node->value.expression[0] != '/')
            result = mergeBitmapAndFreeMemory(result, alphabet['('], scale, '\0');
        image left_child = generateBitmapFromTree(alphabet, current_node->left, scale);

        result = mergeBitmapAndFreeMemory(result, left_child, 1.0, '\0');

        deleteBitmap(&left_child);

        if (current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0]) && current_node->value.expression[0] != '/')
            result = mergeBitmapAndFreeMemory(result, alphabet[')'], scale, '\0');

        if (current_node->value.expression[0] != '^' && current_node->value.expression[0] != FUNCTION && current_node->value.expression[0] != '_' && current_node->value.expression[0] != '/')
            result = mergeBitmapAndFreeMemory(result, alphabet[(int)current_node->value.expression[0]], scale, '\0');

        if (current_node->value.expression[0] == FUNCTION || (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0]) && current_node->value.expression[0] != '^' && current_node->value.expression[0] != '_' && current_node->value.expression[0] != '/') ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0]) && associativity(current_node->right->value.expression[0]) == LEFT_ASSOCIATIVITY))
            result = mergeBitmapAndFreeMemory(result, alphabet['('], new_scale, '\0');

        image right_child = generateBitmapFromTree(alphabet, current_node->right, new_scale);

        result = mergeBitmapAndFreeMemory(result, right_child, 1.0, current_node->value.expression[0]);

        deleteBitmap(&right_child);

        if (current_node->value.expression[0] == FUNCTION || (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0]) && current_node->value.expression[0] != '^' && current_node->value.expression[0] != '_' && current_node->value.expression[0] != '/') ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0]) && associativity(current_node->right->value.expression[0]) == LEFT_ASSOCIATIVITY))
            result = mergeBitmapAndFreeMemory(result, alphabet[')'], new_scale, '\0');
    }
    else
        for (unsigned int iter = 0; current_node->value.expression[iter] != '\0'; iter++)
            result = mergeBitmapAndFreeMemory(result, alphabet[(int)current_node->value.expression[iter]], new_scale, '\0');

    return result;
}

#if 0
image mergeDownscaledBitmap(image *alphabet, image left, image right, double scale)
{
    //image downscaled = createDownscaledImage(right, scale);
    //
}
#endif

image createDownscaledImage(image original, double scale)
{
    image downscaled;
    downscaled.height = (int)((double)original.height * scale);
    downscaled.width = (int)((double)original.width * scale);
    downscaled.maxval = original.maxval;
    setTypeP6(downscaled.magic_number);
    downscaled.map = (pixel *)malloc(downscaled.width * downscaled.height * sizeof(pixel));
    for (unsigned int i = 0; i < original.height; i++)
        for (unsigned int j = 0; j < original.width; j++)
            downscaled.map[minInt(downscaled.height - 1, (int)(i * scale)) * downscaled.width + minInt(downscaled.width - 1, (int)(j * scale))] = original.map[i * original.width + j];
    return downscaled;
}

image generateBitmapFromTextDEBUG(image *alphabet, const char *expression)
{
    image sequence[STRING_SIZE];
    unsigned int length = strlen(expression);
    unsigned int i;
    for (i = 1; i < STRING_SIZE && i <= length; i++)
    {
        sequence[i] = mergeBitmapAndFreeMemory(sequence[i - 1], alphabet[(int)expression[i - 1]], 1.0, '\0');
    }
    return sequence[i - 1];
}

image mergeBitmapHorizontal(image left, image right, int spot)
{
    if (isImageEmpty(left))
        return copyImage(right);
    if (isImageEmpty(right))
        return copyImage(left);
    image result;
    setTypeP6(result.magic_number);
    result.height = maxInt(left.height, right.height);
    result.width = left.width + right.width;
    result.maxval = maxInt(left.maxval, right.maxval);
    result.map = (pixel *)malloc(result.width * result.height * sizeof(pixel));
    for (unsigned int i = 0; i < result.width * result.height; i++)
    {
        result.map[i].red = 255;
        result.map[i].green = 255;
        result.map[i].blue = 255;
    }
    for (unsigned int i = 0; i < left.height; i++)
        for (unsigned int j = 0; j < left.width; j++)
            result.map[i * result.width + j] = left.map[i * left.width + j];

    if (spot == TOP)
        for (unsigned int i = 0; i < right.height; i++)
            for (unsigned int j = 0; j < right.width; j++)
                result.map[i * result.width + j + left.width] = right.map[i * right.width + j];
    else
        for (unsigned int i = 0; i < right.height; i++)
            for (unsigned int j = 0; j < right.width; j++)
                result.map[(result.width * (result.height - right.height)) + i * result.width + j + left.width] = right.map[i * right.width + j];
    return result;
}

image mergeBitmapVertical(image top, image bottom, int spot)
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
    for (unsigned int i = 0; i < result.width * result.height; i++)
    {
        result.map[i].red = 255;
        result.map[i].green = 255;
        result.map[i].blue = 255;
    }
    if (spot != MIDDLE)
        for (unsigned int i = 0; i < top.height; i++)
            for (unsigned int j = 0; j < top.width; j++)
                result.map[i * result.width + j] = top.map[i * top.width + j];

    if (spot == LEFT)
        for (unsigned int i = 0; i < bottom.height; i++)
            for (unsigned int j = 0; j < bottom.width; j++)
                result.map[i * result.width + j + top.height * result.width] = bottom.map[i * bottom.width + j];
    else if (spot == RIGHT)
        for (unsigned int i = 0; i < bottom.height; i++)
            for (unsigned int j = 0; j < bottom.width; j++)
                result.map[i * result.width + j + top.height * result.width + result.width - bottom.width] = bottom.map[i * bottom.width + j];
    else
    {
        if (top.width >= bottom.width)
        {
            for (unsigned int i = 0; i < top.height; i++)
                for (unsigned int j = 0; j < top.width; j++)
                    result.map[i * result.width + j] = top.map[i * top.width + j];

            unsigned int half_delta = result.width - (top.width + bottom.width) / 2;
            for (unsigned int i = 0; i < bottom.height; i++)
                for (unsigned int j = 0; j < bottom.width; j++)
                    result.map[i * result.width + j + top.height * result.width + half_delta] = bottom.map[i * bottom.width + j];
        }
        else
        {
            unsigned int half_delta = result.width - (top.width + bottom.width) / 2;

            for (unsigned int i = 0; i < top.height; i++)
                for (unsigned int j = 0; j < top.width; j++)
                    result.map[i * result.width + j + half_delta] = top.map[i * top.width + j];

            for (unsigned int i = 0; i < bottom.height; i++)
                for (unsigned int j = 0; j < bottom.width; j++)
                    result.map[i * result.width + j + top.height * result.width] = bottom.map[i * bottom.width + j];
        }
    }
    return result;
}

image mergeBitmapAndFreeMemory(image replaced, image added, double scale, char operation)
{
    image downscaled = createDownscaledImage(added, scale);
    image result;
    if (operation != '_' && operation != '/')
        result = mergeBitmapHorizontal(copyImage(replaced), copyImage(downscaled), TOP);
    else if (operation == '_')
        result = mergeBitmapHorizontal(copyImage(replaced), copyImage(downscaled), BOTTOM);
    else
    {
        if (replaced.width > downscaled.width)
            for (unsigned int i = 0; i < replaced.width; i++)
            {
                replaced.map[(replaced.height - 1) * replaced.width + i].red = 0;
                replaced.map[(replaced.height - 1) * replaced.width + i].green = 0;
                replaced.map[(replaced.height - 1) * replaced.width + i].blue = 0;
            }
        else
            for (unsigned int i = 0; i < downscaled.width; i++)
            {
                downscaled.map[i].red = 0;
                downscaled.map[i].green = 0;
                downscaled.map[i].blue = 0;
            }
        result = mergeBitmapVertical(copyImage(replaced), downscaled, MIDDLE);
    }

    deleteBitmap(&replaced);
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
    for (unsigned int i = 0; i < clone.width * clone.height; i++)
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