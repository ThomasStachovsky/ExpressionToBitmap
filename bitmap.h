#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "tree.h"

#define TOP 0
#define BOTTOM 1
#define LEFT 0
#define RIGHT 1
#define MIDDLE 2
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

image generateBitmapFromTree(image *alphabet, node *current_node, double scale);
image generateBitmapFromTextDEBUG(image *alphabet, const char *expression); //DEBUG
image mergeBitmapHorizontal(image left, image right, int spot);
image mergeBitmapVertical(image top, image bottom, int spot);
image mergeBitmapAndFreeMemory(image left, image right, double scale, char operation);
image mergeDownscaledBitmap(image *alphabet, image left, image right, double scale);
void deleteBitmap(image *bitmap);
void setTypeP6(char *magic_number);
image createEmptyImage();
image createDownscaledImage(image original, double scale);
bool isImageEmpty(image candidate);
image copyImage(image original);
