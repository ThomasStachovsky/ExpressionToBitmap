#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "tree.h"

#define BASED 0
#define TOP -1
#define BOTTOM -2
#define LEFT 0
#define RIGHT 1
#define MIDDLE 2
#define OTHER 1
#define SYMBOL '\0'

typedef struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct image
{
    char magic_number[2];
    unsigned int width;
    unsigned int height;
    unsigned int maxval;
    pixel *map;
} image;
image generateBitmapFromTree(image *alphabet, node *head_of_tree);
image generateBitmapFromTreeUtil(image *alphabet, node *current_node, double scale, unsigned int *baseline_parent);
image mergeBitmapHorizontal(image left, image right, int spot, unsigned int baseline_left, unsigned int baseline_right);
image mergeBitmapVertical(image top, image bottom, int spot);
image mergeBitmapAndFreeMemory(image left, image right, double scale, char operation, unsigned int baseline_left, unsigned int baseline_right, unsigned int child_height);
void deleteBitmap(image *bitmap);
void setTypeP6(char *magic_number);
image createEmptyImage();
image createDownscaledImage(image original, double scale);
image createUpscaledImageVertically(image original, double scale);
bool isImageEmpty(image candidate);
image copyImage(image original);
