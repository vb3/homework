//
//  main.c
//  HW7
//
//  Created by Vinay Bikkina on 2/27/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

int get_degree(int x, int y);
int **init2DintArray(int x, int y);
void free2DintArray(int **iArray, int x);
void print_board(int **pboard);
int next_move(int x, int y);

//holds all the degrees
int **board, cx, cy, n;

int main(int argc, const char * argv[])
{
    int i, j;
    
    //get board size
    n = atoi(argv[1]);
    
    //starting square
    cx = atoi(argv[2]);
    cy = atoi(argv[3]);
    
    //init board
    board = init2DintArray(n, n);
    
    //fill with degrees
    for (i=0; i<n; i++){
        for (j=0; j<n; j++) {
            board[i][j] = get_degree(i, j);
        }
    }
    
    //start at x,y
    printf("%d ", cy*n + (cx+1));
    board[cx][cy] = -1;
    fflush(stdout);
    while (next_move(cx, cy));
    
    printf("\n");
    print_board(board);
    
    //free board
    free2DintArray(board, n);
    
    return 0;
}

int next_move(int x, int y)
{
    int i;
    int lx=-1, ly=-1, low_degree = 9;
    int move_x[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
    int move_y[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
    int rx, ry;
    int result_deg;
    
    //find least degree square
    for (i=0; i<8; i++){
        rx = x + move_x[i];
        ry = y + move_y[i];
        if ((rx >= 0) && (rx < n) && (ry >= 0) && (ry < n)) {
            result_deg = get_degree(rx, ry);
            if (result_deg < low_degree && board[rx][ry] != -1) {
                lx = rx;
                ly = ry;
                low_degree = result_deg;
            }
        }
    }
    
    //if least degree exists, move, else end
    if (lx == -1 && ly == -1) {
        return 0;
    } else {
        printf("%d ", ly*n + (lx+1));
        fflush(stdout);
        board[lx][ly] = -1;
        cx = lx;
        cy = ly;
        return 1;
    }
    
}

//returns the degree of the square where knight can move to
int get_degree(int x, int y)
{
    int i, count = 0;
    int move_x[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
    int move_y[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
    int rx, ry;
    
    for (i=0; i<8; i++) {
        rx = x + move_x[i];
        ry = y + move_y[i];
        if ((rx >= 0) && (rx < n) && (ry >= 0) && (ry < n) && board[rx][ry] != -1)
            count++;
    }
    
    return count;
}

//print 2-d board
void print_board(int** pboard){
    int i,j;
    
    for(j = 0; j <= n*3; j++){
        printf("_");
    }
    printf("\n");
    
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("|%02d", pboard[i][j]);
        }
        printf("|\n");
    }
    
    for(j = 0; j <= n*3; j++){
        printf("¯");
    }
    printf("\n");
    
}

/* mallocs 2D int array at passed in iArray */
int **init2DintArray(int x, int y)
{
    int **iArray;
    int i;
    
    iArray = (int**)malloc(x * sizeof(int*));
    for (i = 0; i < x; i++) {
        iArray[i] = (int*)calloc(y, sizeof(int));
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