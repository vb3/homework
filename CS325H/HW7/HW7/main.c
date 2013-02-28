//
//  main.c
//  HW7
//
//  Created by Vinay Bikkina on 2/27/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

int get_degree(int x, int y, int n);
int **init2DintArray(int x, int y);
void free2DintArray(int **iArray, int x);

//holds all the degrees
int **board;

int main(int argc, const char * argv[])
{
    //get board size
    int n = atoi(argv(1));
    
    //starting square
    int x = atoi(argv(2));
    int y = atoi(argv(3));
    
    //init board
    board = init2DintArray(n, n);
    
    
    
    
    //free board
    free2DintArray(board);
    
    return 0;
}

//returns the degree of the square where knight can move to
int get_degree(int x, int y, int n)
{
    int i, count = 0;
    int move_x[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
    int move_y[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
    int rx, ry;
    
    for (i=0; i<8; i++) {
        rx = x + move_x[i];
        ry = y + move_y[i];
        if (rx <= 0 && rx > n && ry <= 0 && ry > n)
            count++;
    }
    
    return count;
}

/* mallocs 2D int array at passed in iArray */
int **init2DintArray(int x, int y)
{
    int **iArray;
    int i;
    
    iArray = (int**)malloc(x * sizeof(int*));
    for (i = 0; i < x; i++) {
        iArray[i] = (int*)malloc(y * sizeof(int));
    }
    
    return iArray;
}

/* frees the malloced int array passed in */
void free2DintArray(int **iArray, int x)
{
    int i;
    
    for (i = 0; i < x; i++) {
        free(iArray[i]);
    }
    free(iArray);
}