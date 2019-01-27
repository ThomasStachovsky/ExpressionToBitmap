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
                if (!scanExpression(expression))
                        break;
                printQuestionForPath();
                if (!scanPath(path))
                        break;
                tree = convertExpressionToTree(expression);
                output = generateBitmapFromTree(alphabet, tree);
                printImage(output, path);
                printf("Debug tree: ");  // DEBUG
                generateTextDEBUG(tree); //DEBUG
                printf("\n");            //DEBUG
                deleteTree(tree);
                deleteBitmap(&output);
                printDoneAndQuestionForAgain();
                if (!scanIfAgain())
                        break;
        }

        for (int i = 0; i < 256; i++)
                if (!isImageEmpty(alphabet[i]))
                        deleteBitmap(&alphabet[i]);
        free(alphabet);

        return 0;
}
