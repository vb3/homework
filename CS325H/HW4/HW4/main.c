//
//  main.c
//  HW4 - Tiling Game
//
//  Created by Vinay Bikkina on 2/6/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int** board;
int size;
int counter=1;

void print_board(int** board);
void rec_spilt_matrix(int x1, int y1, int x2, int y2, int vx, int vy);

int main(int argc, char const *argv[])
{
    
    int i,j;
    size = atoi(argv[1]);
    srand(time(NULL));
    int vx = rand() % size;
    int vy = rand() % size;

    board = (int**) malloc(size*sizeof(int*));
    
    for (i = 0; i < size; i++)
        board[i] = (int*) malloc(size*sizeof(int));
    
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            board[i][j]=0;
        }
    }
    
    board[vx][vy] = 99;
    //print_board(board);
    rec_spilt_matrix(0,0,size-1,size-1,vx,vy);
    board[vx][vy] = 0;
    print_board(board);
    
    return 0;
}

void print_board(int** board){
    int i,j;
    
    for(j = 0; j <= size*3; j++){
        printf("_");
    }
    printf("\n");
    
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("|%02d", board[i][j]);
        }
        printf("|\n");
    }
    
    for(j = 0; j <= size*3; j++){
        printf("¯");
    }
    printf("\n");
    
}

void rec_spilt_matrix(int x1, int y1, int x2, int y2, int vx, int vy)
{    
    if ((x2-x1)+1 == 2) {
        if (board[x1][y1] == 0) board[x1][y1] = counter;
        if (board[x1][y2] == 0) board[x1][y2] = counter;
        if (board[x2][y1] == 0) board[x2][y1] = counter;
        if (board[x2][y2] == 0) board[x2][y2] = counter;
        counter++;
        return;
    }
    
    int x_mid = (x1+x2)/2;
    int y_mid = (y1+y2)/2;
    
    //determine Q (the quadrant that void is in)
    int Q=0;
    
    if (vx <= x_mid && vy <= y_mid) {
        Q=1;
        rec_spilt_matrix(x1, y1, x_mid, y_mid, vx, vy);             //Q1
    } else if (vx > x_mid && vy <= y_mid) {
        Q=2;
        rec_spilt_matrix((x_mid)+1, y1, x2, y_mid, vx, vy);         //Q2
    } else if (vx <= x_mid && vy > y_mid) {
        Q=3;
        rec_spilt_matrix(x1, (y_mid)+1, x_mid, y2, vx, vy);         //Q3
    } else if (vx > x_mid && vy > y_mid) {
        Q=4;
        rec_spilt_matrix((x_mid)+1, (y_mid)+1, x2, y2, vx, vy);     //Q4
    }
    
    int temp_counter = counter++;
    
    if (Q != 1) {
        board[x_mid][y_mid] = temp_counter;
        rec_spilt_matrix(x1, y1, x_mid, y_mid, x_mid, y_mid);                   //Q1
    }
    if (Q != 2) {
        board[x_mid+1][y_mid] = temp_counter;
        rec_spilt_matrix((x_mid)+1, y1, x2, y_mid, (x_mid)+1, y_mid);           //Q2
    }
    if (Q != 3) {
        board[x_mid][y_mid+1] = temp_counter;
        rec_spilt_matrix(x1, (y_mid)+1, x_mid, y2, x_mid, (y_mid)+1);           //Q3
    }
    if (Q != 4) {
        board[x_mid+1][y_mid+1] = temp_counter;
        rec_spilt_matrix((x_mid)+1, (y_mid)+1, x2, y2, (x_mid)+1, (y_mid)+1);   //Q4
    }
}
