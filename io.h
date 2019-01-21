#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bitmap.h"

image *scanAlphabet();
void printGettingStarted();
void scanExpression(char *expression);
void printQuestionForPath();
void scanPath(char *path);
void printDoneAndQuestionForAgain();
bool scanIfAgain();
bool doesFileExist(char *filename);
image *scanAlphabet();
void printImage(image output, const char *path);
