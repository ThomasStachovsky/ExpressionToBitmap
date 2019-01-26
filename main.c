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
        //image big = generateBitmapFromTextDEBUG(alphabet, "4*sin(34)+23^43423");
        // image small = createDownscaledImage(big, 0.5);

        //printImage(big, "big");
        //printImage(small, "small");
        // image big2 = copyImage(big);
        //image hej;
        //image small = createDownscaledImage(big2, 0.4);
        //printImage(small, "path");
        //hej = mergeBitmapHorizontal(big, small);
        //image hej = mergeBitmapAndFreeMemory(big, big2, 0.8);
        //printImage(hej, "merged");
        while (true)
        {
                output = createEmptyImage();
                printGettingStarted();
                scanExpression(expression);
                printQuestionForPath();
                scanPath(path);
                tree = convertExpressionToTree(expression);
                output = generateBitmapFromTree(alphabet, tree, 1.0);
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
