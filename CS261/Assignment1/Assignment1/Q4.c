/* CS261- Assignment 1 - Q.4*/
/* Name: Vinay Bikkina
 * Date: 1/13/2012
 * Solution description:
 */
 
#include <stdio.h>
#include <stdlib.h>

struct student{
	int id;
	int score;
};

void sort(struct student* students, int n){
     /*Sort the n students based on their score*/     
    
    //Insertion Sort
    for (int i = 1; i < n; i++) {
	    int p = i;
	    struct student temp = students[i];
	    while ((p > 0) && (students[p-1].score > temp.score)) {
	        students[p] = students[p-1];
	        p--;
	    }
	    students[p] = temp;
    }
}

int main(){
    /*Declare an integer n and assign it a value.*/
    int n=10;
    
    /*Allocate memory for n students using malloc.*/
    struct student* stud = (struct student*) malloc(sizeof(struct student) * n);
    
    /*Generate random IDs and scores for the n students, using rand().*/
    srand(time(NULL));
    unsigned char is_used[n+1]; //for unique IDs
    memset(&is_used, 0, (n+1)*sizeof(unsigned char));
    for(int i=0;i<n;i++){
        do{
            stud[i].id = (rand() % (n) + 1);
        }while(is_used[stud[i].id]);
        is_used[stud[i].id] = 1;
        
        stud[i].score = (rand() % (101));
    }
    
    /*Print the contents of the array of n students.*/
    printf("Before sort:\n");
    for(int i=0;i<n;i++){
        printf("ID: %d\tScore: %d\n", stud[i].id, stud[i].score);
    }

    /*Pass this array along with n to the sort() function*/
    sort(stud, n);
    
    /*Print the contents of the array of n students.*/
    printf("\nAfter sort:\n");
    for(int i=0;i<n;i++){
        printf("ID: %d\tScore: %d\n", stud[i].id, stud[i].score);
    }
    
    free(stud);
    return 0;
}
