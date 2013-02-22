//
//  main.c
//  HW6 - N-Queens Problem
//
//  Created by Vinay Bikkina on 2/20/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int n;
int *sol;
int sol_num=1;

void compute();
int check(int x, int y);
void print_sol();
int if_iso();

int main(int argc, const char * argv[])
{
    int i;
    printf("N Queens Solutions\nEnter board size: ");
    scanf("%d", &n);
    
    sol = calloc(n, sizeof(int));
    
    for (i=0; i<n; i++) {
        sol[i]=n*2;
    }
    
    compute();
    printf("\nDone. %d Solutions.\n", sol_num-1);
    
    return 0;
}

void compute() {
    int x=0, y=0;
    
    do {
        if (check(x, y)) {
            sol[y++] = x;
            if (y==n) {
                print_sol();
                sol_num++;
            } else {
                x=0;
                continue;
            }
        }
        if(x==n-1 && y>0) { //if at end of row and not end of column then backtrack
            do {
                x = sol[y-1] + 1; //change to prev row and incr column.
                sol[--y] = n*2; //reset that location
            } while (x>=n && y>0);
        } else {
            x++;            
        }
    } while (x<n);
    

    
}

int check(int x, int y) {
    int i;
    
    for (i=0; i<y; i++) {
        if (sol[i] == x) return 0;          //check same column
        if ((sol[i]-(y-i)) == x) return 0;  //check diagonal
        if ((sol[i]+(y-i)) == x) return 0;  //check diagonal
    }
    
    return 1;
}

void print_sol() {
    int x;
    
    if (if_iso()) return;
    
    printf("Solution %d: ", sol_num);
    for (x=0; x<n; x++) {
        printf("%d", sol[x]);
    }
    printf("\n");
}

int if_iso() {
    
}