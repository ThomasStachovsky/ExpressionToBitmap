#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bitmap.h"

image *scanAlphabet();
void printTypeExpression();
void printGettingStarted();
bool scanExpression(char *expression);
void printQuestionForPath();
bool scanPath(char *path);
void printDoneAndQuestionForAgain();
bool scanIfAgain();
void printImage(image bitmap, const char *path);
