#include <stdio.h>
#include <stdbool.h>

#include "io.h"


#define OPERATOR 1
#define EXPRESSION 0
#define RPN_ARRAY_SIZE 512

typedef struct unit
{
        char expression[STRING_SIZE];
        bool type;
}unit;

typedef struct node
{
        unit value;
        struct node *left, *right;
} node;

node *createNode(char *expression);
node *operateOnNode(node *first,node *second, char operator);

bool isOperator(char character);
int weight(char operator);
node *convertExpressionToTree(char *expression);
unit *convertAlgebraicToRPN(char *expression);
node *convertRPNToTree(unit *rpn);
