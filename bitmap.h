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

image generateBitmap(image *alphabet, node *tree);
image generateBitmapFromTextDEBUG(image *alphabet, const char *expression);
image mergeBitmap(image left, image right);
void getTypeP6(char *magic_number);
void createImage(image empty);
bool isImageEmpty(image candidate);
