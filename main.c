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

        image result = generateBitmapFromTextDEBUG(alphabet, "Hejka tu tomek 123 elo @");

        printImage(result, "out");

        while (true)
        {
                createEmptyImage(output);
                printGettingStarted();
                scanExpression(expression);
                printQuestionForPath();
                scanPath(path);
                tree = convertExpressionToTree(expression);
                generateBitmap(alphabet, tree, &output);
                printImage(output, path);
                printDoneAndQuestionForAgain();
                if (!scanIfAgain())
                        break;
        }
        return 0;
}
