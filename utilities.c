#include <string.h>

#include "utilities.h"

void appendString(char *s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len + 1] = '\0';
}

bool isStringEmpty(char *s)
{
        if (s[0] == '\0')
                return true;
        else
                return false;
}

void clearString(char *s)
{
        s[0] = '\0';
}

int maxInt(int a, int b)
{
        if (a > b)
                return a;
        else
                return b;
}

int minInt(int a, int b)
{
        if (a < b)
                return a;
        else
                return b;
}
