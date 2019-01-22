#include <stdio.h>
#include <stdbool.h>

#include "io.h"

int main()
{
        image *alphabet = scanAlphabet();
        node *tree;
        image output;
        char expression[STRING_SIZE];
        char path[STRING_SIZE];

        while (true)
        {
                output = createEmptyImage();
                printGettingStarted();
                scanExpression(expression);
                printQuestionForPath();
                scanPath(path);
                tree = convertExpressionToTree(expression);
                generateBitmapFromTree(alphabet, tree, &output, 1.0);
                printImage(output, path);
                printf("Debug tree: ");  // DEBUG
                generateTextDEBUG(tree); //DEBUG
                printf("\n");            //DEBUG
                printDoneAndQuestionForAgain();
                scanIfAgain();
        }
        return 0;
}
