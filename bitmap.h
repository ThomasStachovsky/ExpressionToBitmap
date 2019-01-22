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

void generateBitmap(image *alphabet, node *current_node, image *output);
image generateBitmapFromTextDEBUG(image *alphabet, const char *expression); //DEBUG
image mergeBitmap(image left, image right);
void getTypeP6(char *magic_number);
void createEmptyImage(image empty);
bool isImageEmpty(image candidate);
image copyImage(image original);
