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
int *polyMult(int *p, int *q, int n);

int main(int argc, const char * argv[])
{
    int *a, *b, *c = NULL;
    int i;
    int order = atoi(argv[1]) + 1;
    
    a = (int *) calloc(order, sizeof(int));
    b = (int *) calloc(order, sizeof(int));
    
    //User Input
    printf("Enter the coefficients of A with powers in ascending order:\n");
    for (i = 0; i < order; i++)
        scanf("%d", &a[i]);
    printf("Enter the coefficients of B with powers in ascending order:\n");
    for (i = 0; i < order; i++)
        scanf("%d", &b[i]);
    
    //Compute
    //c = polyMult(a, b, order+1);
    c = poly_multi_divcon(a, b, order);
    
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

    int *PP = array_op(P, &P[d], d-n%2, d);      //add P1 and P2
    int *QQ = array_op(Q, &Q[d], d-n%2, d);      //add Q1 and Q2
    
    int *R = poly_multi_divcon(P, Q, d);        //Mult P1*Q1
    int *S = poly_multi_divcon(PP, QQ, d);      //Mult (P1+P2)(Q1+Q2)
    int *T = poly_multi_divcon(&P[d], &Q[d], d);//Mult P2*Q2
    
    int *outt = calloc(((n+1)*2), sizeof(int));
    
    //x^2d(R) + x^d(S-R-T) + T
    for (i = 0; i<=(d==1?0:d); i++) { //if d=1 then only need to do loop once, otherwise we get artifacts 
        outt[i] += T[i];
        outt[i+d] += S[i] - R[i] - T[i];
        outt[i+dd] += R[i];
    }
    
    free(R); free(S); free(T); free(PP); free(QQ);
    return outt;
}

int *array_op(int *A, int *B, int lenA, int lenB)
{
    int i;
    int lenC = (lenA<=lenB?lenA:lenB);
    int *out = calloc(lenC, sizeof(int));
    
    for (i=0; i<lenC; i++)
        out[i] = A[i] + B[i];
    
    if (lenA>lenB)
        for (i=lenB; i<lenA; i++)
            out[i] = A[i];
    
    if (lenA<lenB)
        for (i=lenA; i<lenB; i++)
            out[i] = B[i];
    
    return out;
}

// http://www.cs.vassar.edu/~cs241/teneyck/divideandconquer.pdf
int *polyMult(int *p, int *q, int n) {
    if (n == 1) {
        int *out = malloc(sizeof(int));
        *out = p[0] * q[0];
        return out;
    }
    
    int d = n/2 + n%2;
    //split p and q
    int p2[d];
    int q2[d];
    for (int i = 0; i < d; i++) {
        p2[i] = p[i];
        q2[i] = q[i];
    }
    int p1[d-n%2];
    int q1[d-n%2];
    for (int i = 0; i < d; i++) {
        p1[i] = p[i+d];
        q1[i] = q[i+d];
    }
    int *r = polyMult(p2,q2,d);
    int *s = polyMult(array_op(p2, p1, d, d-n%2), array_op(p2, p1, d, d-n%2), d);
    int *t = polyMult(p1,q1,d);
    int *prod = calloc(2*n, sizeof(int));

    //x^2d(R) + x^d(S-R-T) + T
    for (int i = 0; i < d; i++) {
        prod[i] = r[i];
        prod[i+d] += s[i] - r[i] - t[i];
        prod[i+2*d] += t[i];
    }
    return prod;
}