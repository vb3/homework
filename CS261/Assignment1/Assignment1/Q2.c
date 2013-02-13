/* CS261- Assignment 1 - Q.2*/
/* Name: Vinay Bikkina
 * Date: 1/13/2012
 * Solution description:
 */
 
#include <stdio.h>
#include <stdlib.h>

int foo(int* a, int* b, int c){
    /*Set a to double its original value*/
    *a *= 2;
    
    /*Set b to half its original value*/
    *b /= 2;
    
    /*Assign a+b to c*/
    c = *a+*b;
    
    /*Return c*/
    return c;
}

int main(){
    /*Declare three integers x,y and z and initialize them to 5, 6, 7 respectively*/
    int x=5, y=6, z=7;
    int fooVal;
    
    /*Print the values of x, y and z*/
    printf("x= %d\ty= %d\tz= %d\n",x,y,z);
    
    /*Call foo() appropriately, passing x,y,z as parameters*/
    /*Print the value returned by foo*/
    printf("Value returned by foo: %d\n",foo(&x,&y,z));
    
    /*Print the values of x, y and z again*/
    printf("x= %d\ty= %d\tz= %d\n",x,y,z);
 
    /*Is the return value different than the value of z?  Why?*/
    //YES, Because the value of z was never changed in the funtion "foo". the value of Z as pass to the function "foo" as a ByVal not ByRef.
    return 0;
}
    
    
