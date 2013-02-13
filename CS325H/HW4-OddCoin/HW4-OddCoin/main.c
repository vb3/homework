//
//  main.c
//  HW4-OddCoin
//
//  Created by Vinay Bikkina on 2/7/13.
//  Copyright (c) 2013 Vinay Bikkina. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int *coins;
int count=0;
void print_coins(int size);
int compare(int srange1, int srange2, int size);
void rec_split(int srange, int size);

int main(int argc, const char * argv[])
{
    int size = pow(3, atoi(argv[1]));
    coins = (int*) malloc(size * sizeof(int));
    
    for (int i=0; i<size; i++) {
        coins[i] = 1;
    }
    
    srand((int)time(NULL));
    int temp_rand = rand() % size;
    while(coins[temp_rand] == 1) coins[temp_rand] = rand() % 3;
    
    //print_coins(size);
    rec_split(0, size);
    printf("\n# of Calls: %d\n", count);
    return 0;
}

void print_coins(int size)
{
    for (int i=0; i<size/3; i++) {
        printf("%d %d %d ", coins[i*3], coins[i*3+1], coins[i*3+2]);
    }
    printf("\n");
}

int compare(int srange1, int srange2, int size)
{
    count++;
    int srange1_sum=0;
    int srange2_sum=0;
    
    for (int i = 0; i<size; i++){
        srange1_sum += coins[i+srange1];
        srange2_sum += coins[i+srange2];
    }
    
    if (srange2_sum == srange1_sum) {
        return 0;
    } else if (srange1_sum < srange2_sum) {
        return -1;
    } else {
        return 1;
    }
}

void rec_split(int srange, int size)
{
    if (size == 1) {
        printf("Index of ODD coin: %d\n", srange);
        return;
    }
    int new_size = size/3;
    int s1 = srange;
    int s2 = srange+new_size;
    int s3 = srange+(2*new_size);
    
    if (compare(s1, s2, new_size) == 0) { //compare 1 & 2
        rec_split(s3, new_size);
    } else if (compare(s2, s3, new_size) == 0) { //compare 2 & 3
        rec_split(s1, new_size);
    } else {
        rec_split(s2, new_size);
    }
}
