/* CS261- Assignment 1 - Q.5*/
/* Name: Vinay Bikkina
 * Date: 1/13/2012
 * Solution description:
 */
 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*converts ch to upper case, assuming it is in lower case currently*/
char toUpperCase(char ch){
     return ch-'a'+'A';
}

/*converts ch to lower case, assuming it is in upper case currently*/
char toLowerCase(char ch){
     return ch-'A'+'a';
}

void sticky(char* word){
     /*Convert to sticky caps*/
    int x=0;
    
    do{
        if(x%2 == 0){
            if(islower(word[x]))
                word[x] = toUpperCase(word[x]);
        }else{
            if(isupper(word[x]))
                word[x] = toLowerCase(word[x]);
        }
    }while(word[x++]!='\0');
    
}

int main(){
    char input[20];
    
    /*Read word from the keyboard using scanf*/
    printf("Enter a word: ");
    scanf("%s", &input);
    
    /*Call sticky*/
    sticky(input);
    
    /*Print the new word*/
    printf("Word after sticky: %s\n",input);
    
    return 0;
}
