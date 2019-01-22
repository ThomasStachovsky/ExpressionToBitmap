#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "tree.h"

node *convertExpressionToTree(char *expression)
{
        unsigned int rpn_size;
        unit *rpn = convertAlgebraicToRPN(expression, &rpn_size);
        node *root_of_tree = convertRPNToTree(rpn, rpn_size);
        return root_of_tree;
}

node *createNode(unit object)
{
        node *first_node = (node *)malloc(sizeof(node));
        first_node->left = NULL;
        first_node->right = NULL;
        first_node->value = object;
        return first_node;
}

node *operateOnNode(node *left, node *right, unit object)
{
        node *result = malloc(sizeof(node));
        result->value = object;
        result->left = left;
        result->right = right;
        return result;
}

bool isOperator(char character)
{
        if (character == '(' || character == ')' || character == '/' || character == '*' || character == '-' || character == '+' || character == '^' || character == FUNCTION || character == '_' || character == ',' || character == '=' || character == '<' || character == '>')
                return true;
        else
                return false;
}

bool commutative(char operator)
{
        if (operator== '-' || operator== '/' || operator== FUNCTION || operator== '^' || operator== '_' || operator== '<' || operator== '>')
                return false;
        else
                return true;
}

int weight(char operator)
{
        if (operator== FUNCTION)
                return 4;
        if (operator== '^' || operator== '_')
                return 3;
        if (operator== '*' || operator== '/')
                return 2;
        if (operator== '+' || operator== '-')
                return 1;
        if (operator== ',' || operator== '=' || operator== '<' || operator== '>')
                return 0;
        else
                return -1;
}

int associativity(char operator)
{
        if (operator== '^' || operator== FUNCTION)
                return RIGHT_ASSOCIATIVITY;
        else
                return LEFT_ASSOCIATIVITY;
}

unit *convertAlgebraicToRPN(char *expression, unsigned int *rpn_size)
{
        unsigned int size_of_expression = strlen(expression);
        unsigned int expression_iterator = 0;

        unit *rpn = (unit *)malloc(sizeof(unit) * RPN_ARRAY_SIZE);
        unsigned int rpn_iterator = 0;

        char operator_stack[RPN_ARRAY_SIZE];
        unsigned int operator_stack_iterator = 0;

        char character;

        char current_expression[STRING_SIZE];
        clearString(current_expression);

        do
        {

                character = expression[expression_iterator];

                if (isOperator(character))
                {
                        bool nonempty_previous_expression = false;

                        if (!isStringEmpty(current_expression))
                        {
                                strcpy(rpn[rpn_iterator].expression, current_expression);
                                rpn[rpn_iterator].type = EXPRESSION;
                                rpn_iterator++;
                                clearString(current_expression);
                                nonempty_previous_expression = true;
                        }

                        if (character == '(')
                        {
                                if (expression_iterator != 0 && nonempty_previous_expression)
                                        operator_stack[operator_stack_iterator++] = FUNCTION;

                                operator_stack[operator_stack_iterator++] = character;
                        }

                        else if (character == ')')
                        {
                                while (operator_stack[operator_stack_iterator - 1] != '(')
                                {
                                        operator_stack_iterator--;
                                        rpn[rpn_iterator].expression[0] = operator_stack[operator_stack_iterator];
                                        rpn[rpn_iterator].expression[1] = '\0';
                                        rpn[rpn_iterator].type = OPERATOR;
                                        rpn_iterator++;
                                }
                                operator_stack_iterator--;
                        }

                        else
                        {
                                while (operator_stack_iterator > 0 && operator_stack[operator_stack_iterator - 1] != '(' && (weight(operator_stack[operator_stack_iterator - 1]) > weight(character) || (weight(operator_stack[operator_stack_iterator - 1]) == weight(character) && associativity(operator_stack[operator_stack_iterator - 1]) == LEFT_ASSOCIATIVITY)))
                                {
                                        operator_stack_iterator--;
                                        rpn[rpn_iterator].expression[0] = operator_stack[operator_stack_iterator];
                                        rpn[rpn_iterator].expression[1] = '\0';
                                        rpn[rpn_iterator].type = OPERATOR;
                                        rpn_iterator++;
                                }
                                operator_stack[operator_stack_iterator++] = character;
                        }
                }
                else if (!isspace(character))
                        appendString(current_expression, character);

                expression_iterator++;

        } while (expression_iterator < size_of_expression);

        if (!isStringEmpty(current_expression))
        {
                strcpy(rpn[rpn_iterator].expression, current_expression);
                rpn[rpn_iterator].type = EXPRESSION;
                rpn_iterator++;
                clearString(current_expression);
        }

        while (operator_stack_iterator > 0)
        {
                operator_stack_iterator--;
                rpn[rpn_iterator].expression[0] = operator_stack[operator_stack_iterator];
                rpn[rpn_iterator].expression[1] = '\0';
                rpn[rpn_iterator].type = OPERATOR;
                rpn_iterator++;
        }

        *rpn_size = rpn_iterator;

        return rpn;
}

node *convertRPNToTree(unit *rpn, unsigned int rpn_size)
{

        unsigned int stack_iterator = 0;
        node *stack[RPN_ARRAY_SIZE];

        for (unsigned int rpn_iterator = 0; rpn_iterator < rpn_size; rpn_iterator++)
        {
                if (rpn[rpn_iterator].type == OPERATOR)
                {
                        stack[stack_iterator - 2] = operateOnNode(stack[stack_iterator - 2], stack[stack_iterator - 1], rpn[rpn_iterator]);
                        stack_iterator--;
                }
                else
                {
                        stack[stack_iterator] = createNode(rpn[rpn_iterator]);
                        stack_iterator++;
                }
        }
        return stack[stack_iterator - 1];
}