#include <stdio.h>
#include <stdbool.h>

#include "bitmap.h"
#include "io.h"
#include "tree.h"
#include "utilities.h"

int main()
{
        bool running = true;
        while (running)
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
                running = scanIfExit();
        }
        return 0;
}
