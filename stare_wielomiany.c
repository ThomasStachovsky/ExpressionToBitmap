#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef unsigned long long rational;

rational add(rational a, rational b);
rational subtract(rational a, rational b);
rational multiply(rational a, rational b);
rational divide(rational a, rational b);
rational power(rational a, int exponent);
void scanr(rational *number);
void printr(rational number);
void reduce(rational *number);
int den(rational number);
int num(rational number);
rational construct(int numerator, int denumerator);
int gcd(int a, int b);

#define POLYNOMIAL_SIZE 256


typedef rational polynomial[POLYNOMIAL_SIZE];

void addp(polynomial a, polynomial b, polynomial sum);
void subtractp(polynomial a, polynomial b, polynomial difference);
void multiplyp(polynomial a, polynomial b, polynomial product);
void scanp(polynomial p, char *original_line);
void printp(polynomial p);
rational valueOf(polynomial p, rational x);
void zerop(polynomial p);
/*typedef struct node{
   bool is_operator;
   char operator;
   polynomial expression;
   }node;*/
int den(rational number)
{
        return (number << 32) >> 32;
}

int num(rational number)
{
        return number >>= 32;
}


int gcd(int a,int b)
{
        unsigned int temp;
        while(b!=0)
        {
                temp=a%b;
                a=b;
                b=temp;
        }
        return a;
}



rational construct(int numerator, int denominator)
{
        rational number = numerator;
        number <<= 32;
        number+=denominator;
        return number;
}


rational add(rational a, rational b)
{
        rational result=0;
        int numerator1, denominator1, numerator2,denominator2,denominatorresult, numeratorresult;
        denominator1=den(a);
        denominator2=den(b);
        numerator1=num(a);
        numerator2=num(b);
        denominatorresult=denominator1*denominator2;
        numeratorresult=numerator1*denominator2 + numerator2*denominator1;
        result+=numeratorresult;
        result <<= 32;
        result+=denominatorresult;
        reduce(&result);
        return result;
}

rational subtract(rational a, rational b)
{
        rational result=0;
        int numerator1, denominator1, numerator2,denominator2,denominatorresult, numeratorresult;
        denominator1=den(a);
        denominator2=den(b);
        numerator1=num(a);
        numerator2=num(b);
        denominatorresult=denominator1*denominator2;
        numeratorresult=numerator1*denominator2 - numerator2*denominator1;
        result+=numeratorresult;
        result <<= 32;
        result+=denominatorresult;
        reduce(&result);
        return result;
}


rational multiply(rational a, rational b)
{
        rational result=0;
        int numerator1, denominator1, numerator2,denominator2,denominatorresult, numeratorresult;
        denominator1=den(a);
        denominator2=den(b);
        numerator1=num(a);
        numerator2=num(b);
        denominatorresult=denominator1*denominator2;
        numeratorresult=numerator1*numerator2;
        result+=numeratorresult;
        result <<= 32;
        result+=denominatorresult;
        reduce(&result);
        return result;
}

rational power(rational a, int exponent)
{
        if(num(a)==0 && exponent == 0) return construct (1,1);
        if(num(a)==0) return construct(0,1);
        rational result = construct(1,1);
        for(int i=0; i<exponent; i++)
                result = multiply(result, a);

        reduce(&result);
        return result;
}



rational divide(rational a, rational b)
{
        if(!b)
        {
                printf("Exception: Divided by zero");
                return 0;
        }
        rational result=0;
        int numerator1, denominator1, numerator2,denominator2,denominatorresult, numeratorresult;
        denominator1=den(a);
        denominator2=den(b);
        numerator1=num(a);
        numerator2=num(b);
        denominatorresult=denominator1*numerator2;
        numeratorresult=numerator1*denominator2;
        result+=numeratorresult;
        result <<= 32;
        result+=denominatorresult;
        reduce(&result);
        return result;
}

void reduce(rational *number)
{
        int denominator = den(*number);
        int numerator = num(*number);
        int temp;
        if(numerator>0)
        {
                temp = numerator;
                numerator/=gcd(numerator,denominator);
        }
        else
        {
                temp = -numerator;
                numerator/=gcd(-numerator,denominator);
        }
        denominator/=gcd(temp,denominator);

        (*number)=construct(numerator, denominator);
}


void scanr(rational *number)
{
        int numerator, denominator,integer;
        integer = scanf("%d/%d",&numerator,&denominator);
        if(integer==1) denominator=1;
        if(!denominator)
        {
                printf("Exception: The denominator is zero!");
                return;
        }
        (*number)=construct(numerator,denominator);
        reduce(&(*number));
}

void printr(rational number)
{
        int numerator = num(number), denominator=den(number);
        if(denominator==1)
        {
                printf("%d",numerator);
                return;
        }
        printf("%d/%d",numerator,denominator);
}

void zerop(polynomial p)
{
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
                p[i]=construct(0,1);
}

void addp(polynomial a, polynomial b, polynomial sum)
{
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
                sum[i]=add(a[i],b[i]);
}

void subtractp(polynomial a, polynomial b, polynomial difference)
{
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
                difference[i]=subtract(a[i],b[i]);
}

void multiplyp(polynomial a, polynomial b, polynomial product)
{
        zerop(product);
        for(int i=0; i*2<POLYNOMIAL_SIZE; i++)
                for(int j=0; j*2<POLYNOMIAL_SIZE; j++)
                        product[i+j] = add(product[i+j], multiply(a[i],b[j]));
}

rational valueOf(polynomial p, rational x)
{
        rational value = construct(0,1);
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
                if(num(p[i]))
                        value = add(value, multiply(p[i],power(x,i)));
        return value;
}


void printp(polynomial p)
{
        int empty=1;
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
                if(num(p[i])) empty=0;

        if(empty)
        {
                printf("0");
                return;
        }

        int first = 0;

        for(int i=POLYNOMIAL_SIZE-1; i>=0 && !first; i--)
                if(num(p[i]))
                        first = i;
        if(first>1)
        {
                if(num(p[first])>0)
                {
                        if(num(p[first])!=1)
                                printr(p[first]);
                        printf("x^%d ",first);
                }
                else
                {
                        printf("-");
                        if(-num(p[first])!=1)
                                printr(construct(-num(p[first]),den(p[first])));
                        printf("x^%d ",first);
                }
        }
        for(int i=first - 1; i>=2; i--)
        {
                if(num(p[i]))
                {
                        if(num(p[i])>0)
                        {
                                printf("+ ");
                                printr(p[i]);
                                printf("x^%d ",i);
                        }
                        else if(num(p[i])<0)
                        {

                                printf("- ");
                                printr(construct(-num(p[i]),den(p[i])));
                                printf("x^%d ",i);
                        }
                }
        }
        if(num(p[1])>0)
        {
                if(first>1)
                        printf("+ ");
                if(num(p[1])!=1)
                        printr(p[1]);
                printf("x ");
        }
        else if(num(p[1])<0)
        {
                if(first==1)
                        printf("-");
                else

                        printf("- ");
                printr(construct(-num(p[1]),den(p[1])));
                printf("x ");
        }
        if(num(p[0])>0)
        {
                if(first>0)
                        printf("+ ");
                printr(p[0]);
        }
        else if(num(p[0])<0)
        {
                if(first==0)
                        printf("-");
                else
                        printf("- ");
                printr(construct(-num(p[0]),den(p[0])));
        }
}


void scanp(polynomial p, char *original_line)
{

        zerop(p);
        const int LINE_SIZE = 101, EXPRESSION_SIZE = 32;
        const char EMPTY_CHAR=0;
        char line[LINE_SIZE];
        /*for(int i=0; i<LINE_SIZE; i++)
                line[i]=EMPTY_CHAR;
           char character;

           int char_pos=0;
           while(char_pos<LINE_SIZE)
           {
                character=getchar();
                if(character=='\n')
                        break;
                if(!isspace(character) && character!='*')
                {
                        line[char_pos]=character;
                        char_pos++;
                }
           }*/

        strcpy(line,original_line);

        char expression[EXPRESSION_SIZE];
        for(int i=0; i<EXPRESSION_SIZE; i++)
                expression[i]=EMPTY_CHAR;
        int it=0, sign_0=0, sign = 1;
        if(line[0]== '-' || line[0]=='+')
        {
                sign=44-line[0];
                sign_0=1;
        }
        for(int i=0 + sign_0; line[i]!= EMPTY_CHAR && i<LINE_SIZE; i++)
        {
                if(line[i]!='-' && line[i]!='+' && i+1<LINE_SIZE && line[i+1]!=EMPTY_CHAR)
                {
                        expression[it]=line[i];
                        it++;
                }
                else
                {
                        if(i+1>=LINE_SIZE || line[i+1]==EMPTY_CHAR)
                        {
                                expression[it]=line[i];
                                it++;
                        }

                        int exponent=0, coefficient_numerator=1, coefficient_denominator=1;
                        for(int j=0; j<it; j++)
                                if(expression[j]=='x')
                                {
                                        if(j+2<it && expression[j+1]=='^')
                                        {
                                                char exponent_array[it-j-2 + 1];
                                                exponent_array[it-j-2]='\0';
                                                for(int k=0; k<it-j-2; k++)
                                                        exponent_array[k]=expression[j+2+k];
                                                sscanf(exponent_array, "%d", &exponent);
                                        }
                                        else exponent=1;
                                        break;
                                }
                        if(expression[0]!='x')
                        {
                                int slash_position=0, x_position=0;

                                for(int j=0; j<it && !slash_position; j++)
                                        if(expression[j]=='/')
                                                slash_position=j;


                                for(int j=slash_position; j<it && !x_position; j++)
                                        if(expression[j]=='x')
                                                x_position=j;

                                if(x_position)
                                {
                                        if(slash_position)
                                        {
                                                char numerator_array[slash_position + 1];
                                                numerator_array[slash_position]='\0';
                                                char denominator_array[x_position-slash_position-1 + 1];
                                                denominator_array[x_position-slash_position-1]='\0';
                                                for(int j=0; j<slash_position; j++)
                                                        numerator_array[j]=expression[j];
                                                for(int j=slash_position+1, iter=0; j<x_position; j++,iter++)
                                                        denominator_array[iter]=expression[j];
                                                sscanf(numerator_array, "%d", &coefficient_numerator);
                                                sscanf(denominator_array, "%d", &coefficient_denominator);
                                        }
                                        else
                                        {
                                                char numerator_array[x_position + 1];
                                                numerator_array[x_position]='\0';
                                                for(int j=0; j<x_position; j++)
                                                        numerator_array[j]=expression[j];
                                                sscanf(numerator_array, "%d", &coefficient_numerator);
                                        }

                                }
                                else
                                {
                                        if(slash_position)
                                        {
                                                char numerator_array[slash_position + 1];
                                                numerator_array[slash_position]='\0';
                                                char denominator_array[it-1-slash_position + 1];
                                                denominator_array[it-1-slash_position]='\0';
                                                for(int j=0; j<slash_position; j++)
                                                        numerator_array[j]=expression[j];
                                                for(int j=slash_position+1, iterat=0; j<it; j++, iterat++)
                                                        denominator_array[iterat]=expression[j];
                                                sscanf(numerator_array, "%d", &coefficient_numerator);
                                                sscanf(denominator_array, "%d", &coefficient_denominator);
                                        }
                                        else
                                        {
                                                char numerator_array[it + 1];
                                                numerator_array[it]='\0';

                                                for(int j=0; j<it; j++)
                                                        numerator_array[j]=expression[j];
                                                sscanf(numerator_array, "%d", &coefficient_numerator);
                                        }
                                }
                        }
                        rational coefficient = construct(sign*coefficient_numerator,coefficient_denominator);
                        sign=44-line[i];
                        p[exponent]=coefficient;
                        for(int i=0; i<it; i++)
                                expression[i]=EMPTY_CHAR;
                        it=0;
                }
        }
}

void differentiate(polynomial a, polynomial result)
{
        zerop(result);

        for(int i=0; i<POLYNOMIAL_SIZE-1; i++)
        {
                //result[i]=a[i+1]*i
                result[i]=multiply(a[i+1],construct(i+1,1));
        }
}


void substitute(polynomial expression, polynomial substitution, polynomial result)
{

        zerop(result);
        for(int i=0; i<POLYNOMIAL_SIZE; i++)
        {
                polynomial coefficient;
                zerop(coefficient);
                coefficient[0]=expression[i];

                polynomial one;
                zerop(one);
                one[0]=construct(1,1);

                if(expression[i]==construct(0,1))
                        continue;

                for(int j=0; j<i; j++)
                {
                        polynomial product;
                        multiplyp(substitution,one,product);

                        for(int k=0; k<POLYNOMIAL_SIZE; k++)
                                one[k]=product[k];

                }

                polynomial partial;
                multiplyp(one,coefficient,partial);

                polynomial addon;
                addp(result,partial,addon);
                for(int k=0; k<POLYNOMIAL_SIZE; k++)
                        result[k]=addon[k];
        }


}

void appendString(char *s,char c)
{
        int len=strlen(s);
        s[len]=c;
        s[len+1]='\0';
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

int main()
{
        const int constant=1000;
        size_t h=100;
        polynomial first_stack[h];
        int first_stack_pointer=0;
        int coded_stack[h];
        int coded_stack_pointer=0;
        char operator_stack[h];
        int operator_stack_pointer=0;


        for(int i=0; i<h; i++)
                zerop(first_stack[i]);


        char *input;
        input = (char *)malloc(h+1);

        getline(&input,&h,stdin);



        int tempiter=0;

        char tempstr[h];

        for(int i=0; i<h; i++)
                tempstr[i]='\0';

        for(int i=0; i<h; i++)
        {
                if(input[i]!='[' && input[i]!=']')
                        tempstr[i+tempiter]=input[i];
                else if(input[i]=='[')
                {
                        tempstr[i+tempiter]=input[i];
                        appendString(tempstr,'(');
                        tempiter++;
                }
                else
                {
                        tempstr[i+tempiter]=input[i];
                        appendString(tempstr,')');
                        tempiter++;
                        appendString(tempstr,' ');
                        tempiter++;
                }
        }

        strcpy(input,tempstr);

        //printf("%s",input);

        char character;

        char current_polynomial[h];
        current_polynomial[0]='\0';

        int input_pointer=0;
        do
        {
                character=input[input_pointer++];
                if(isspace(character))
                        continue;

                if(isOperator(character))
                {
                  #if 0
                        /*if(character=='/')
                           {
                                input_pointer+=2;
                                if(current_polynomial[0]!='\0')
                                {
                                        coded_stack[coded_stack_pointer++]=constant+first_stack_pointer;
                                        scanp(first_stack[first_stack_pointer++],current_polynomial);
                                        current_polynomial[0]='\0';
                                        //printf("Dopisuje wielomian do coded_stack\n");
                                }
                                //printf("Pomijam 2 znaki bo / \n");
                                continue;
                           }

                           if(character=='-')
                           {
                                if(input_pointer-2<0 || !isspace(input[input_pointer-2]))
                                {
                                        appendString(current_polynomial,character);
                                        //  printf("Znak minus, ale to nie operator\n");
                                        //printf("                                 Appenduje wielomian tym minusem\n");
                                        continue;
                                }
                           }*/


                        //operator_stack[operator_stack_pointer++]=character;
#endif
                        if(current_polynomial[0]!='\0')
                        {
                                coded_stack[coded_stack_pointer++]=constant+first_stack_pointer;
                                scanp(first_stack[first_stack_pointer++],current_polynomial);
                                current_polynomial[0]='\0';
                                //printf("Dopisuje wielomian do coded_stack\n");
                        }

                        if(character=='(')
                        {
                                operator_stack[operator_stack_pointer++]=character;
                                //printf("Napotkalem (\n");
                                continue;
                        }

                        if(character==')')
                        {
                                //  printf("Napotkalem )...\n");
                                while(operator_stack[operator_stack_pointer-1]!='(')
                                {
                                        operator_stack_pointer--;
                                        coded_stack[coded_stack_pointer++]=operator_stack[operator_stack_pointer];
                                        //printf("   Wrzucam do coded_stack %c\n",operator_stack[operator_stack_pointer]);

                                }
                                operator_stack_pointer--;
                                //printf("usuwam (\n");
                                continue;
                        }

                        if(operator_stack_pointer>0)
                        {
                                if(weight(character)>weight(operator_stack[operator_stack_pointer-1]))
                                {
                                        operator_stack[operator_stack_pointer++]=character;
                                        //printf("Napotkalem %c i go dopisuje do operator_stack\n",character);
                                }
                                else
                                {
                                        while(operator_stack_pointer>0 && weight(operator_stack[operator_stack_pointer-1])>=weight(character))
                                        {
                                                operator_stack_pointer--;
                                                coded_stack[coded_stack_pointer++]=operator_stack[operator_stack_pointer];
                                                //printf("Popuje %c do coded_stack\n",operator_stack[operator_stack_pointer]);
                                        }
                                        operator_stack[operator_stack_pointer++]=character;
                                        //printf("Skonczylem popowac operatory a teraz biore %c i go dopisuje do operator_stack\n",character);
                                }

                        }
                        else
                        {
                                operator_stack[operator_stack_pointer++]=character;
                                //printf("Pierwszy operator  %c i go dopisuje do operator_stack\n",character);
                        }


                }
                else
                {
                        appendString(current_polynomial,character);
                        //  printf("                                 Appenduje wielomian\n");
                }



        } while(input_pointer<strlen(input));

        if(current_polynomial[0]!='\0')
        {
                coded_stack[coded_stack_pointer++]=constant+first_stack_pointer;
                scanp(first_stack[first_stack_pointer++],current_polynomial);
                current_polynomial[0]='\0';
                //printf("Dopisuje (koncowy) wielomian do coded_stack\n");
        }

        while(operator_stack_pointer>0)
        {
                operator_stack_pointer--;
                coded_stack[coded_stack_pointer++]=operator_stack[operator_stack_pointer];
                //printf("Napotkalem koniec i popuje %c do coded_stack\n",operator_stack[operator_stack_pointer]);
        }

        //debug
        /*printf("\n---------------------------------\n");
           for(int i=0; i<h; i++)
                if(coded_stack[i]<constant && coded_stack[i])
                        printf("%c\n",(char)coded_stack[i]);
                else if(coded_stack[i]>=constant)
                {
                        printp(first_stack[coded_stack[i]-constant]);
                        printf("\n");
                }*/

        polynomial usable_stack[h];

        for(int i=0; i<h; i++)
                zerop(usable_stack[i]);

        polynomial zero_polynomial;
        zerop(zero_polynomial);

        int usable_iterator=0;

        for(int i=0; i<h; i++)
        {


                /*for(int u=0; u<usable_iterator; u++)
                   {
                        printp(usable_stack[u]);
                        printf("\n");
                   }
                   printf("-----------\n");*/

                if(coded_stack[i]>=constant)
                        addp(zero_polynomial,first_stack[coded_stack[i]-constant],usable_stack[usable_iterator++]);
                else
                {
                        if(coded_stack[i]=='+')
                        {
                                //printf("Dodaje\n");
                                polynomial sum;
                                addp(usable_stack[usable_iterator-1],usable_stack[usable_iterator-2],sum);
                                usable_iterator-=2;
                                addp(zero_polynomial,sum,usable_stack[usable_iterator++]);
                        }


                        if(coded_stack[i]=='-')
                        {
                                //printf("Odejmuje\n");
                                polynomial difference;
                                subtractp(usable_stack[usable_iterator-2],usable_stack[usable_iterator-1],difference);
                                usable_iterator-=2;
                                addp(zero_polynomial,difference,usable_stack[usable_iterator++]);
                        }

                        if(coded_stack[i]=='*')
                        {
                                //printf("Mnoze\n");
                                polynomial product;
                                multiplyp(usable_stack[usable_iterator-1],usable_stack[usable_iterator-2],product);
                                usable_iterator-=2;
                                addp(zero_polynomial,product,usable_stack[usable_iterator++]);
                        }

                        if(coded_stack[i]=='[')
                        {
                                //printf("Podstawiam\n");
                                polynomial product;
                                substitute(usable_stack[usable_iterator-2],usable_stack[usable_iterator-1],product);
                                usable_iterator-=2;
                                addp(zero_polynomial,product,usable_stack[usable_iterator++]);
                        }

                        if(coded_stack[i]==39)
                        {
                                //printf("Rozniczkuje\n");
                                polynomial product;
                                differentiate(usable_stack[usable_iterator-1],product);
                                usable_iterator-=1;
                                addp(zero_polynomial,product,usable_stack[usable_iterator++]);
                        }


                }
        }

        //printf("\n\n\n\n");
        printp(usable_stack[0]);


        return 0;
}
