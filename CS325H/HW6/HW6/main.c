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

void compute();
int check(int x, int y);

int main(int argc, const char * argv[])
{
    printf("N Queens Solutions\n");
    scanf("Enter board size: %d", &n);
    
    sol = calloc(n, sizeof(int));
    
    
    return 0;
}

void compute() {
    int x=0, y=0;
    
    if (check(x, y)) {
        
    }
}

int check(int x, int y) {
    int i;
    
    for (i=0; i<n; i++) {
        if (sol[i] == x)
            return 0;
        if (sol[i]-(y+i) == x)
            return 0;
        if (sol[i]-(y-i) == x)
            return 0;
    }
    
    return 1;
}