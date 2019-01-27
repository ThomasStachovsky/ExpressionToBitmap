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

        /*image one = generateBitmapFromTextDEBUG(alphabet, "H");
        image two = generateBitmapFromTextDEBUG(alphabet, "P");
        image merged = mergeBitmapHorizontal(one, two, BASED, 155, 11);
        printImage(merged, "test");*/

        while (true)
        {
                output = createEmptyImage();
                printGettingStarted();
                scanExpression(expression);
                printQuestionForPath();
                scanPath(path);
                tree = convertExpressionToTree(expression);
                output = generateBitmapFromTree(alphabet, tree);
                printImage(output, path);
                printf("Debug tree: ");  // DEBUG
                generateTextDEBUG(tree); //DEBUG
                printf("\n");            //DEBUG
                deleteTree(tree);
                deleteBitmap(&output);
                printDoneAndQuestionForAgain();
                scanIfAgain();
        }
        return 0;
}
