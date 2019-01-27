#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "bitmap.h"

image generateBitmapFromTree(image *alphabet, node *head_of_tree)
{
    unsigned int temp;
    return generateBitmapFromTreeUtil(alphabet, head_of_tree, 1.0, &temp);
}

image generateBitmapFromTreeUtil(image *alphabet, node *current_node, double scale, unsigned int *baseline_parent)
{
    if (current_node == NULL)
        return createEmptyImage();
    unsigned int baseline_left, baseline_right;
    double new_scale = scale;
    image result = createEmptyImage();
    if (current_node->value.type == OPERATOR)
    {
        if (current_node->value.expression[0] == '^' || current_node->value.expression[0] == '_')
            new_scale = 0.5 * scale;
        else if (current_node->value.expression[0] == '/')
        {
            new_scale = 0.8 * scale;
            scale = new_scale;
        }

        image left_child = generateBitmapFromTreeUtil(alphabet, current_node->left, scale, &baseline_left);
        image right_child = generateBitmapFromTreeUtil(alphabet, current_node->right, new_scale, &baseline_right);

        image left_part = createEmptyImage();
        image right_part = createEmptyImage();

        image left_bracket = createEmptyImage();
        image right_bracket = createEmptyImage();

        unsigned int baseline_symbol = (unsigned int)(scale * alphabet['a'].height / 2.0);
        unsigned int baseline_left_bracket;
        unsigned int baseline_right_bracket;

        unsigned int higher_child_height = maxInt(left_child.height, right_child.height);

        if (current_node->left->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->left->value.expression[0]) && current_node->value.expression[0] != '/')
        {
            left_bracket = mergeBitmapAndFreeMemory(left_bracket, alphabet['('], scale, '(', baseline_left, baseline_symbol, higher_child_height);
            right_bracket = mergeBitmapAndFreeMemory(right_bracket, alphabet[')'], scale, ')', baseline_left, baseline_symbol, higher_child_height);
        }
        baseline_left_bracket = left_bracket.height / 2;
        baseline_right_bracket = right_bracket.height / 2;

        left_part = mergeBitmapAndFreeMemory(left_part, left_child, 1.0, OTHER, baseline_left, baseline_right, 0);

        left_part = mergeBitmapAndFreeMemory(left_part, right_bracket, 1.0, OTHER, baseline_left, baseline_right_bracket, 0);
        baseline_left = maxInt(baseline_left, baseline_right_bracket);
        image temp_to_deletion = left_part;
        left_part = mergeBitmapAndFreeMemory(left_bracket, left_part, 1.0, OTHER, baseline_left_bracket, baseline_left, 0);
        deleteBitmap(&temp_to_deletion);
        baseline_left = maxInt(baseline_left, baseline_left_bracket);

        if (current_node->value.expression[0] != '^' && current_node->value.expression[0] != FUNCTION && current_node->value.expression[0] != '_' && current_node->value.expression[0] != '/')
            left_part = mergeBitmapAndFreeMemory(left_part, alphabet[(int)current_node->value.expression[0]], scale, OTHER, baseline_left, baseline_symbol, 0);

        deleteBitmap(&right_bracket);
        left_bracket = createEmptyImage();

        if (current_node->value.expression[0] == FUNCTION || (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) > weight(current_node->right->value.expression[0]) && current_node->value.expression[0] != '^' && current_node->value.expression[0] != '_' && current_node->value.expression[0] != '/') ||
            (current_node->right->value.type == OPERATOR && weight(current_node->value.expression[0]) == weight(current_node->right->value.expression[0]) && !commutative(current_node->value.expression[0]) && associativity(current_node->right->value.expression[0]) == LEFT_ASSOCIATIVITY && current_node->value.expression[0] != '/'))
        {
            left_bracket = mergeBitmapAndFreeMemory(left_bracket, alphabet['('], new_scale, '(', baseline_left, baseline_symbol, higher_child_height);
            right_bracket = mergeBitmapAndFreeMemory(right_bracket, alphabet[')'], new_scale, ')', baseline_left, baseline_symbol, higher_child_height);
        }
        baseline_left_bracket = left_bracket.height / 2;
        baseline_right_bracket = right_bracket.height / 2;
        right_part = mergeBitmapAndFreeMemory(right_part, right_child, 1.0, OTHER, baseline_left, baseline_right, 0);

        right_part = mergeBitmapAndFreeMemory(right_part, right_bracket, 1.0, OTHER, baseline_right, baseline_right_bracket, 0);
        baseline_right = maxInt(baseline_right, baseline_right_bracket);
        temp_to_deletion = right_part;
        right_part = mergeBitmapAndFreeMemory(left_bracket, right_part, 1.0, OTHER, baseline_left_bracket, baseline_right, 0);
        deleteBitmap(&temp_to_deletion);
        baseline_right = maxInt(baseline_right, baseline_left_bracket);

        result = mergeBitmapAndFreeMemory(left_part, right_part, 1.0, current_node->value.expression[0], baseline_left, baseline_right, 0);

        if (current_node->value.expression[0] == '/')
            (*baseline_parent) = left_child.height;
        else
            (*baseline_parent) = maxInt(baseline_left, baseline_right);
        deleteBitmap(&right_bracket);
        deleteBitmap(&right_part);
        deleteBitmap(&left_child);
        deleteBitmap(&right_child);
    }
    else
    {
        for (unsigned int iter = 0; current_node->value.expression[iter] != '\0'; iter++)
            result = mergeBitmapAndFreeMemory(result, alphabet[(int)current_node->value.expression[iter]], new_scale, SYMBOL, baseline_left, baseline_right, 0);
        (*baseline_parent) = result.height / 2;
    }
    return result;
}

image createDownscaledImage(image original, double scale)
{
    image downscaled;
    downscaled.height = (unsigned int)((double)original.height * scale);
    downscaled.width = (unsigned int)((double)original.width * scale);
    downscaled.maxval = original.maxval;
    setTypeP6(downscaled.magic_number);
    downscaled.map = (pixel *)malloc(downscaled.width * downscaled.height * sizeof(pixel));
    for (unsigned int i = 0; i < original.height; i++)
        for (unsigned int j = 0; j < original.width; j++)
            downscaled.map[minInt(downscaled.height - 1, (unsigned int)(i * scale)) * downscaled.width + minInt(downscaled.width - 1, (unsigned int)(j * scale))] = original.map[i * original.width + j];
    return downscaled;
}

image generateBitmapFromTextDEBUG(image *alphabet, const char *expression)
{
    image sequence[STRING_SIZE];
    unsigned int length = strlen(expression);
    unsigned int i;
    for (i = 1; i < STRING_SIZE && i <= length; i++)
    {
        sequence[i] = mergeBitmapAndFreeMemory(sequence[i - 1], alphabet[(int)expression[i - 1]], 1.0, '\0', 0, 0, 0);
    }
    return sequence[i - 1];
}

image mergeBitmapHorizontal(image left, image right, int spot, unsigned int baseline_left, unsigned int baseline_right)
{
    if (isImageEmpty(left))
        return copyImage(right);
    if (isImageEmpty(right))
        return copyImage(left);
    image result;
    setTypeP6(result.magic_number);
    if (spot == BASED)
        result.height = left.height + right.height - minInt(baseline_left, baseline_right) - minInt(left.height - baseline_left, right.height - baseline_right);
    else
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
    if (spot != BASED)
    {
        for (unsigned int i = 0; i < left.height; i++)
            for (unsigned int j = 0; j < left.width; j++)
                result.map[i * result.width + j] = left.map[i * left.width + j];

        if (spot == TOP)
            for (unsigned int i = 0; i < right.height; i++)
                for (unsigned int j = 0; j < right.width; j++)
                    result.map[i * result.width + j + left.width] = right.map[i * right.width + j];
        else if (spot == BOTTOM)
            for (unsigned int i = 0; i < right.height; i++)
                for (unsigned int j = 0; j < right.width; j++)
                    result.map[(result.width * (result.height - right.height)) + i * result.width + j + left.width] = right.map[i * right.width + j];
    }
    else
    {
        unsigned int level = maxInt(baseline_left, baseline_right);
        for (unsigned int i = 0; i < left.height; i++)
            for (unsigned int j = 0; j < left.width; j++)
                result.map[i * result.width + j + (level - baseline_left) * result.width] = left.map[i * left.width + j];

        for (unsigned int i = 0; i < right.height; i++)
            for (unsigned int j = 0; j < right.width; j++)
                result.map[i * result.width + j + left.width + (level - baseline_right) * result.width] = right.map[i * right.width + j];
    }
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

image mergeBitmapAndFreeMemory(image replaced, image added, double scale, char operation, unsigned int baseline_left, unsigned int baseline_right, unsigned int child_height)
{
    image downscaled = createDownscaledImage(added, scale);
    image result;
    if (operation == '^' || operation == SYMBOL)
        result = mergeBitmapHorizontal(replaced, downscaled, TOP, baseline_left, baseline_right);
    else if (operation == '_')
        result = mergeBitmapHorizontal(replaced, downscaled, BOTTOM, baseline_left, baseline_right);
    else if (operation == '/')
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
        result = mergeBitmapVertical(replaced, downscaled, MIDDLE);
    }
    else if (operation == ')' || operation == '(')
    {
        image upscaled = createUpscaledImageVertically(downscaled, (child_height / scale) / (double)added.height);
        result = mergeBitmapHorizontal(replaced, upscaled, BASED, baseline_left, baseline_right);
        deleteBitmap(&upscaled);
    }
    else
        result = mergeBitmapHorizontal(replaced, downscaled, BASED, baseline_left, baseline_right);
    deleteBitmap(&replaced);
    deleteBitmap(&downscaled);
    return result;
}

image createUpscaledImageVertically(image original, double scale)
{
    image upscaled;
    upscaled.height = (unsigned int)((double)original.height * scale);
    upscaled.width = original.width;
    upscaled.maxval = original.maxval;
    setTypeP6(upscaled.magic_number);
    upscaled.map = (pixel *)malloc(upscaled.width * upscaled.height * sizeof(pixel));

    double iter_old = 0.0;
    unsigned int iter_new = 0;
    while ((unsigned int)iter_old < original.height && iter_new < upscaled.height)
    {
        for (unsigned int i = 0; i < original.width; i++)
            upscaled.map[iter_new * upscaled.width + i] = original.map[(unsigned int)iter_old * original.width + i];
        iter_new++;
        iter_old += 1.0 / scale;
    }
    return upscaled;
}

void deleteBitmap(image *bitmap)
{
    if (bitmap == NULL)
        return;
    if (bitmap->map != NULL)
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