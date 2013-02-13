//
//  main.c
//  HW1
//
//  Created by Vinay Bikkina on 1/16/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdio.h>
#include "dynArray.h"
#include <math.h>
#include <limits.h>

void hanoi(DynArr* A, DynArr* B, DynArr* C, int n);
void printHanoi();
void moveAtoC(DynArr* A, DynArr* C);
void move(int from, int to);
char *int2bin(unsigned n, char *buf);
void itHanoi(DynArr* A, DynArr* B, DynArr* C, int n);
int tpHelper(int t, int p);

DynArr* iA;
DynArr* iB;
DynArr* iC;
int iN;
int stepCounter=0;

int main(int argc, const char * argv[])
{
    int n = 4; //(int)argv[1];
    
    iA = newDynArr(n);
    iB = newDynArr(n);
    iC = newDynArr(n);
    iN = n;
    
    printf("Running hanoi with n=%d\n\n", n);
    for (int i=n;i>0;i--){
        pushDynArr(iA, i);
    }
    
    //printHanoi(n);
    
    //recursive
    hanoi(iA, iB, iC, n);
    
    //iterative
    //itHanoi(iA, iB, iC, n);
    
    printf("Done in %d steps.\n\n", stepCounter);
    
    return 0;
}

//Page 21
void itHanoi(DynArr* A, DynArr* B, DynArr* C, int n)
{
    int t=0;
    stepCounter=0;
    char* binCount;
    
    int p=(n%2==0?1:-1);
    
    while (1) {
        move((t%3), abs(tpHelper(t, p)%3));
        t = tpHelper(t, p);
        if (stepCounter==(pow(2, n) - 1)) return;
        binCount = int2bin(stepCounter, NULL);
        
        int i;
        //find right most bit
        for (i=0; i<n; i++)   
            if (binCount[i] == '0') break;
        
        if ((i+1)%2==0)     //even
            move(tpHelper(t, 0-p)%3, tpHelper(t, p)%3);
        else                //odd
            move(tpHelper(t, p)%3, tpHelper(t, 0-p)%3);
    }
}


int tpHelper(int t, int p)
{
    if (t==0 && p == -1)
        return 2;
    return t += p;
}

void hanoi(DynArr* A, DynArr* B, DynArr* C, int n)
{
    printf("Call hanoi n=%d\t\t A=%x\t B=%x\t C=%x\n", n, A, B, C);
    if(n==1){
        moveAtoC(A, C);
    } else {
        hanoi(A, C, B, n-1);
        moveAtoC(A, C);
        
        hanoi(B, A, C, n-1);
    }
    printf("Return hanoi n=%d\t A=%x\t B=%x\t C=%x\n", n, A, B, C);
}

void move(int from, int to)
{
    DynArr* ato;
    DynArr* afrom;
    
    switch (from) {
        case 0:
            afrom = iA;
            break;
        case 1:
            afrom = iB;
            break;
        case 2:
            afrom = iC;
            break;
    }
    
    switch (to) {
        case 0:
            ato = iA;
            break;
        case 1:
            ato = iB;
            break;
        case 2:
            ato = iC;
            break;
    }
    
    moveAtoC(afrom, ato);
}

void moveAtoC(DynArr* A, DynArr* C)
{
    //Move top disk of A to C
    pushDynArr(C, topDynArr(A));
    popDynArr(A);
    //printHanoi();
    stepCounter++;
}

void printHanoi()
{
    char a, b, c;
    
    for (int i=iN-1;i>=0;i--){
        a = getDynArr(iA, i) + 48;
        b = getDynArr(iB, i) + 48;
        c = getDynArr(iC, i) + 48;
        a = (a == 48 ? 32 : a);
        b = (b == 48 ? 32 : b);
        c = (c == 48 ? 32 : c);
        
        printf("| %2c | %2c | %2c |\n", a, b, c);
    }
    printf("|  A |  B |  C |\n\n");
}

char *int2bin(unsigned n, char *buf)
{
#define BITS (sizeof(n) * CHAR_BIT)
    
    static char static_buf[BITS + 1];
    int i;
    
    if (buf == NULL)
        buf = static_buf;
    
    for (i = 0; i < BITS; i++) {
        buf[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
    
    buf[BITS] = '\0';
    return buf;
    
#undef BITS
}
