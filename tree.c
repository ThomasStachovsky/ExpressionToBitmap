#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "bitmap.h"
#include "io.h"
#include "tree.h"
#include "utilities.h"

node *convertExpressionToTree(char *expression)
{
        unit *rpn=convertAlgebraicToRPN(expression);
        node *root_of_tree=convertRPNToTree(rpn);
        return root_of_tree;
}


node *createNode(char *expression)
{
        node *first_node=(node *)malloc(sizeof(node));
        first_node->left=NULL;
        first_node->right=NULL;
        strcpy(first_node->value.expression,expression);
        first_node->value.type=EXPRESSION;
        return first_node;
}

node *operateOnNode(node *first, node *second, char operator)
{
        node *result=malloc(sizeof(node));
        result->value.type=OPERATOR;
        result->value.expression[0]=operator;
        result->value.expression[1]='\0';
        result->left=first;
        result->right=second;
        return result;
}

bool isOperator(char character)
{
        if(character=='(' || character==')' || character=='/'
           || character=='*' || character=='-' || character =='+'
           || character=='^' || character=='_')
                return true;
        else return false;
}

int weight(char operator)
{
        //not for ( and )
        if(operator=='^' || operator=='_')
                return 3;
        if(operator=='*' || operator=='/')
                return 2;
        if(operator=='+' || operator == '-')
                return 1;
        else return 0;
}

unit *convertAlgebraicToRPN(char *expression)
{
        unsigned int size_of_expression=strlen(expression);
        unsigned int expression_iterator=0;

        unit *rpn=(unit*)malloc(sizeof(unit)*RPN_ARRAY_SIZE);
        unsigned int rpn_iterator=0;

        char operator_stack[RPN_ARRAY_SIZE];
        unsigned int operator_stack_iterator=0;



        char character;


        char current_expression[STRING_SIZE];
        current_expression[0]='\0';

        do
        {

                character=expression[expression_iterator++];
                if(isspace(character))
                        continue;

                if(isOperator(character))
                {
//cos

                        if(current_expression[0]!='\0')
                        {
                                strcpy(rpn[rpn_iterator].expression,current_expression);
                                rpn[rpn_iterator].type=EXPRESSION;
                                rpn_iterator++;
                                current_expression[0]='\0';
                        }

                        if(character=='(')
                        {
                                operator_stack[operator_stack_iterator++]=character;
                                continue;
                        }

                        if(character==')')
                        {
                                while(operator_stack[operator_stack_iterator-1]!='(')
                                {
                                        operator_stack_iterator--;
                                        char operator_as_string[2];
                                        operator_as_string[0]=operator_stack[operator_stack_iterator];
                                        operator_as_string[1]='\0';
                                        strcpy(rpn[rpn_iterator].expression,operator_as_string);
                                        rpn[rpn_iterator].type=OPERATOR;
                                        rpn_iterator++;
                                }
                                operator_stack_iterator--;
                                continue;
                        }

                        if(operator_stack_iterator>0)
                        {
                                if(weight(character)>weight(operator_stack[operator_stack_iterator-1]))
                                        operator_stack[operator_stack_iterator++]=character;
                                else
                                {
                                        while(operator_stack_iterator>0 && weight(operator_stack[operator_stack_iterator-1])>=weight(character))
                                        {
                                                operator_stack_iterator--;
                                                char operator_as_string[2];
                                                operator_as_string[0]=operator_stack[operator_stack_iterator];
                                                operator_as_string[1]='\0';
                                                strcpy(rpn[rpn_iterator].expression,operator_as_string);
                                                rpn[rpn_iterator].type=OPERATOR;
                                                rpn_iterator++;
                                        }
                                        operator_stack[operator_stack_iterator++]=character;
                                }
                        }
                        else
                                operator_stack[operator_stack_iterator++]=character;

                }
                else
                        appendString(current_expression,character);


        } while(expression_iterator<size_of_expression);


}
