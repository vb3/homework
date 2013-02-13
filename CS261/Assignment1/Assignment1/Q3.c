/* CS261- Assignment 1 - Q.3*/
/* Name: Vinay Bikkina
 * Date: 1/13/2012
 * Solution description:
 */
 
#include <stdio.h>

void sort(int* number, int n){
    /*Sort the given array number , of length n*/

    //Insertion Sort
    for (int i = 1; i < n; i++) {
	    int p = i;
	    int temp = number[i];
	    while ((p > 0) && (number[p-1] > temp)) {
	        number[p] = number[p-1];
	        p--;
	    }
	    number[p] = temp;
    }
}

int main(){
    /*Declare an integer n and assign it a value of 20.*/
    int n=20;
    
    /*Allocate memory for an array of n integers using malloc.*/
    int* arrInts = (int*)(malloc(sizeof(int)*n));
    
    /*Fill this array with random numbers, using rand().*/
    srand(time(NULL));
    for(int i=0;i<n;i++){
        arrInts[i] = (rand() % (999 - 100 + 1) + 100);
    }
    
    /*Print the contents of the array.*/
    printf("Before sort:\n");
    for(int i=0;i<n;i++){
        printf("%d\t",arrInts[i]);
        if(i%10==9) printf("\n");
    }
    
    /*Pass this array along with n to the sort() function of part a.*/
    sort(arrInts, n);
    
    /*Print the contents of the array.*/    
    printf("\nAfter sort:\n");
    for(int i=0;i<n;i++){
        printf("%d\t",arrInts[i]);
        if(i%10==9) printf("\n");
    }
    
    return 0;
}
