#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "tree.h"

typedef struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

typedef struct image
{
    char magic_number[2];
    int width;
    int height;
    int maxval;
    pixel *map;
} image;

void generateBitmapFromTree(image *alphabet, node *current_node, image *output, double scale);
image generateBitmapFromTextDEBUG(image *alphabet, const char *expression); //DEBUG
image mergeBitmapHorizontal(image left, image right);
image mergeBitmapAndFreeMemory(image left, image right);
image mergeDownscaledBitmap(image *alphabet, image left, image right, double scale);
void deleteBitmap(image *bitmap);
void setTypeP6(char *magic_number);
image createEmptyImage();
image createDownscaledImage(image original, double scale);
bool isImageEmpty(image candidate);
image copyImage(image original);
