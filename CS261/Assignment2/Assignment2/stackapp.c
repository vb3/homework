//Vinay Bikkina
/*	stack.c: Stack application. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dynArray.h"

char nextChar(char* s);
int isBalanced(char* s);

/* ***************************************************************
Using stack to check for unbalanced parentheses.
***************************************************************** */

/* Returns the next character of the string, once reaches end return '0' (zero)
	param: 	s pointer to a string 	
	pre: s is not null		
*/
char nextChar(char* s)
{
	static int i = -1;	
	char c;
	++i;	
	c = *(s+i);			
	if ( c == '\0' )
		return '\0';	
	else 
		return c;
}

/* Checks whether the (), {}, and [] are balanced or not
	param: 	s pointer to a string 	
	pre: s is not null	
	post:	
*/
int isBalanced(char* s)
{
	char temp = nextChar(s);
    char lookForI[] = "[{("; //open braces
    char lookForO[] = ")}]"; //close braces
    
    DynArr* stack;
    stack = newDynArr(16);
    
    do{
        if(strchr(lookForI, temp)!=0) //if opening brace, add to stack
            pushDynArr(stack, temp);
        
        if(strchr(lookForO, temp)!=0){ //if closing brace, check for the mathcing closing brace in the stack
            if(temp == ')'){
                if(topDynArr(stack) != '(') return 0;
            }
            if(temp == '}' || temp == ']'){
                if(topDynArr(stack) != temp-2) return 0;
            }
            popDynArr(stack);
        }
        
        temp = nextChar(s);
    }while(temp!='\0');
    
    if(isEmptyDynArr(stack)) return 1;
	return 0;
}

int main(int argc, char* argv[]){

	char* s=argv[1];	
	int res;
	
	printf("Assignment 2\n");
    
	res = isBalanced(s);

	if (res)
		printf("The string %s is balanced\n",s);
	else 
		printf("The string %s is not balanced\n",s);
	
	return 0;	
}

