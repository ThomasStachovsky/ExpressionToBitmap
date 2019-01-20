#include <stdio.h>
#include <stdbool.h>

#include "io.h"

#define OPERATOR 1
#define EXPRESSION 0
#define RPN_ARRAY_SIZE 512
#define RIGHT_ASSOCIATIVITY 1
#define LEFT_ASSOCIATIVITY 0
#define FUNCTION '@'

typedef struct unit
{
        char expression[STRING_SIZE];
        bool type;
} unit;

typedef struct node
{
        unit value;
        struct node *left, *right;
} node;

node *createNode(unit object);
node *operateOnNode(node *left, node *right, unit object);

bool isOperator(char character);
int weight(char operator);
int associativity(char operator);
node *convertExpressionToTree(char *expression);
unit *convertAlgebraicToRPN(char *expression, unsigned int *rpn_size);
node *convertRPNToTree(unit *rpn, unsigned int rpn_size);
