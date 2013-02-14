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

int *array_op(int *A, int *B, int len, int add);
int *poly_multi_divcon(int *P, int *Q, int n);

int main(int argc, const char * argv[])
{
    int order = atoi(argv[1]) + 1;
    int *a = calloc(order, sizeof(int));
    int *b = calloc(order, sizeof(int));
    int *c = NULL;
    int i, j;
    
    //User Input
    printf("Enter the coefficients of A with powers in decending order:\n");
    for (i = 0; i < order; i++)
        scanf("%d", &a[i]);
    printf("Enter the coefficients of B with powers in decending order:\n");
    for (i = 0; i < order; i++)
        scanf("%d", &b[i]);
    
    //Compute Iterative
	int *result = calloc((order-1)*2, sizeof(int));
	for(i=0; i<order; i++) {
		for(j=0; j<order; j++)
			result[i+j] +=a[i] * b[j];
    }
    //Print results Iterative
    printf("Iterative Results:\n");
    for (i = 0; i < (order*2)-1; i++) {
        if (i == (order-1)*2 && result[i] != 0)
            printf("%c %d ", '+', result[i]);
        else if(result[i] != 0)
            printf("%c %dx^%d ", '+', result[i], ((order-1)*2)-i);
    }
    printf("\n");
    
    //Compute DivCon
    c = poly_multi_divcon(a, b, order);
    //Print results DivCon
    printf("DivCon Results:\n");
    for (i = 0; i < (order*2)-1; i++) {
        if (i == (order-1)*2 && result[i] != 0)
            printf("%c %d ", '+', result[i]);
        else if(result[i] != 0)
            printf("%c %dx^%d ", '+', result[i], ((order-1)*2)-i);
    }
    printf("\n");
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
    
    //split p and q
    int p2[d], q2[d], p1[d-n%2], q1[d-n%2];
    for (int i = 0; i < d; i++) {
        p2[i] = P[i];
        q2[i] = Q[i];
        p1[i] = P[i+d];
        q1[i] = Q[i+d];
    }
    
    int *PP = array_op(p1, p2, d-n%2, d);      //add P1 and P2
    int *QQ = array_op(q1, q2, d-n%2, d);      //add Q1 and Q2
    
    int *R = poly_multi_divcon(p2, q2, d);     //Mult P2*Q2
    int *S = poly_multi_divcon(PP, QQ, d);     //Mult (P1+P2)(Q1+Q2)
    int *T = poly_multi_divcon(p1, q1, d);     //Mult P1*Q1
    
    int *outt = calloc(((n+1)*2), sizeof(int));
    
    //x^2d(R) + x^d(S-R-T) + T
    for (i = 0; i<d; i++) {
        //printf("n= %d i=%d T[i]=%d\t\t S-R-T=%d\t\t R[i]=%d\n", n, i, T[i], S[i] - R[i] - T[i], R[i]);
        outt[i] += R[i];
        outt[i+d] += S[i] - R[i] - T[i];
        outt[i+(d*2)] += T[i];
    }
    
    free(R); free(S); free(T); free(PP); free(QQ);
    return outt;
}

int *array_op(int *A, int *B, int lenA, int lenB)
{
    int i;
    int len_min = (lenA<=lenB?lenA:lenB);
    int *out = calloc((lenA>=lenB?lenA:lenB), sizeof(int));
    
    for (i=0; i<len_min; i++)
        out[i] = A[i] + B[i];
    
    if (lenA>lenB){
        for (i=lenB; i<lenA; i++)
            out[i] = A[i];
    } else {
        for (i=lenA; i<lenB; i++)
            out[i] = B[i];
    }
    return out;
}