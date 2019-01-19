#include <string.h>

#include "utilities.h"

void appendString(char *s,char c)
{
        int len=strlen(s);
        s[len]=c;
        s[len+1]='\0';
}
