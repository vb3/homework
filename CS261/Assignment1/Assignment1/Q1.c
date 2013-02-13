/* CS261- Assignment 1 - Q.1*/
/* Name: Vinay Bikkina
 * Date: 1/13/2012
 * Solution description:
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct student{
	int id;
	int score;
};

struct student* allocate(){
    /*Allocate memory for ten students*/
    struct student *s = (struct student *) malloc(sizeof(struct student) * 10);
    
    /*return the pointer*/
    return s;
}

void generate(struct student* students){
    /*Generate random ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
    //(rand() % (max - min + 1) + min);
    srand(time(NULL));
    
    unsigned char is_used[11] = {0}; //for unique IDs
    
    for(int i=0;i<10;i++){
        do{
            students[i].id = (rand() % (10) + 1);
        }while(is_used[students[i].id]);
        is_used[students[i].id] = 1;
        
        students[i].score = (rand() % (101));
    }
     
}

void output(struct student* students){
     /*Output information about the ten students in the format:
              ID1 Score1
              ID2 score2
              ID3 score3
              ...
              ID10 score10*/
    
    for(int i=0;i<10;i++){
        printf("ID: %d\tScore: %d\n", students[i].id, students[i].score);
    }
}

void summary(struct student* students){
    /*Compute and print the minimum, maximum and average scores of the ten students*/
    int min=100, max=0, average=0;
    
    for(int i=0;i<10;i++){
        average += students[i].score;
        min = (students[i].score < min) ? (students[i].score) : (min);
        max = (students[i].score > max) ? (students[i].score) : (max);
    }
    
    average /= 10;
    
    printf("min: %d\tmax: %d\taverage: %d\n",min,max,average);
}

void deallocate(struct student* stud){
    /*Deallocate memory from stud*/
    if(stud != NULL)
        free(stud);
}

int main(){
    struct student* stud = NULL;
    
    /*call allocate*/
    stud = allocate();
    
    /*call generate*/
    generate(stud);
    
    /*call output*/
    output(stud);
    
    /*call summary*/
    summary(stud);
    
    /*call deallocate*/
    deallocate(stud);

    return 0;
}
