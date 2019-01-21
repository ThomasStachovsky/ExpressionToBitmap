#include <stdio.h>
#include <stdbool.h>

#include "io.h"

int main()
{
        image *alphabet = scanAlphabet();

        while (true)
        {
                printGettingStarted();
                char expression[STRING_SIZE];
                scanExpression(expression);
                printQuestionForPath();
                char path[STRING_SIZE];
                scanPath(path);
                convertExpressionToTree(expression);
                //generateBitmap();
                //printBitmap();
                printDoneAndQuestionForAgain();
                if (!scanIfAgain())
                        break;
        }
        return 0;
}
