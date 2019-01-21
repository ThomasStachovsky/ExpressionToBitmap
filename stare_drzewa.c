/*
Tomasz Stachowski
List 8, Exercise 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_SIZE 100

typedef struct node
{
    char operator;
    double argument;
    struct node *left, *right;
} node;

node *create(double number)
{
    node *first_node = malloc(sizeof(node));
    first_node->left = NULL;
    first_node->right = NULL;
    first_node->argument = number;
    first_node->operator= '\0';
    return first_node;
}

node *operate(node *first, node *second, char operator)
{
    node *result = malloc(sizeof(node));
    result->operator= operator;
    result->left = first;
    result->right = second;
    return result;
}

node *scan()
{
    node *stack[STACK_SIZE];
    unsigned top = 0;
    char operator;
    double argument;
    char character;
    while (character != '=')
    {
        character = getchar();
        if (isspace(character))
            continue;
        if (character == '+' || character == '-' || character == '*' || character == '/')
        {
            operator= character;
            stack[top - 2] = operate(stack[top - 2], stack[top - 1], operator);
            top--;
        }
        else if (character != '=')
        {
            ungetc(character, stdin);
            scanf("%lf", &argument);
            stack[top] = create(argument);
            top++;
        }
    }
    return stack[top - 1];
}

int priority(char operator)
{
    if (operator== '*' || operator== '/')
        return 1;
    else
        return 0;
}

int commutative(char operator)
{
    if (operator== '-' || operator== '/')
        return 0;
    else
        return 1;
}

void print(node *current)
{
    if (current == NULL)
        return;
    if (current->operator!= '\0')
    {
        if ((current->left->operator!= '\0' && priority(current->operator) > priority(current->left->operator)))
            printf("(");

        print(current->left);

        if ((current->left->operator!= '\0' && priority(current->operator) > priority(current->left->operator)))
            printf(")");
        printf("%c", current->operator);
        if ((current->right->operator!= '\0' && priority(current->operator) > priority(current->right->operator)) ||
            (current->right->operator!= '\0' && priority(current->operator) == priority(current->right->operator) && !commutative(current->operator)))

            printf("(");

        print(current->right);
        if ((current->right->operator!= '\0' && priority(current->operator) > priority(current->right->operator)) ||
            (current->right->operator!= '\0' && priority(current->operator) == priority(current->right->operator) && !commutative(current->operator)))
            printf(")");
    }
    else
        printf("%.1lf", current->argument);
}

int main()
{
    node *root;
    root = scan();
    print(root);
    return 0;
}
