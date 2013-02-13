//
//  main.c
//  divide and conquer algorithm (using only three multiples)
//  for multiplying polynomials
//
//  Created by Vinay Bikkina on 2/11/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int *array_op(int *A, int *B, int len, int add);
int *poly_multi_divcon(int *P, int *Q, int n);

int main(int argc, const char * argv[])
{
    int *a, *b, *c = NULL;
    int i;
    int order = 3; //atoi(argv[1]) + 1;
    
    a = (int *) calloc(order, sizeof(int));
    b = (int *) calloc(order, sizeof(int));
    
    //User Input
    printf("Enter the coefficients of A with powers in ascending order:\n");
    for (i = 0; i < order+1; i++)
        scanf("%d", &a[i]);
    printf("Enter the coefficients of B with powers in ascending order:\n");
    for (i = 0; i < order+1; i++)
        scanf("%d", &b[i]);
    
    //Compute
    c = poly_multi_divcon(a, b, order+1);
    
    //Print results
    for (i = 2*(order+1); i >= 0; i--) {
        if (i == 0 && c[i] != 0)
            printf("%c %d ", '+', c[i]);
        else if(c[i] != 0)
            printf("%c %dx^%d ", '+', c[i], i);
    }
    
    return 0;
}

int *poly_multi_divcon(int *P, int *Q, int n)
{
    if (n == 1) {
        int *out = malloc(sizeof(int));
        *out = P[0] * Q[0];
        return out;
    }
    
    int i;
    int d = n/2 + n%2;
    int dd = d*2;

    int *PP = array_op(P, &P[d], d, 1);      //add P1 and P2
    int *QQ = array_op(Q, &Q[d], d, 1);      //add Q1 and Q2
    
    int *R = poly_multi_divcon(P, Q, d);        //Mult P1*Q1
    int *S = poly_multi_divcon(PP, QQ, d);      //Mult (P1+P2)(Q1+Q2)
    int *T = poly_multi_divcon(&P[d], &Q[d], d);//Mult P2*Q2
    
    int *outt = calloc((n*2)+1, sizeof(int));
    
    //x^2d(R) + x^d(S-R-T) + T
    for (i = 0; i<=d; i++) {
        outt[i] += T[i];
        outt[i+d] += S[i] - R[i] - T[i];
        outt[i+dd] += R[i];
    }
    
    free(R); free(S); free(T); free(PP); free(QQ);
    return outt;
}

/* Adds or Subs A and B. add=1 for add, add=0 for sub */
int *array_op(int *A, int *B, int len, int add)
{
    int i;
    int *out = calloc(len, sizeof(int));
    
    for (i=0; i<len; i++)
        if (add)    out[i] = A[i] + B[i];
        else        out[i] = A[i] - B[i];
    
    return out;
}