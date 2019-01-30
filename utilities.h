#include <string.h>
#include <stdbool.h>

#define STRING_SIZE 512

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[97m"
#define BLUE "\x1B[94m"
#define RESET "\x1B[0m"

void appendString(char *s, char c);
bool isStringEmpty(char *s);
void clearString(char *s);
int maxInt(int a, int b);
int minInt(int a, int b);
void clearStdin();
bool doesFileExist(char *filename);