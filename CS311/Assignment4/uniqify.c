/*
 *  uniqify.c
 *
 *
 *  Created by Vinay Bikkina on 11/3/12.
 *
 */
#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_WORD_LEN 100

void makeSortProcesses(int numProcesses, int **fdin, int **fdout);
void startParser(int numProcesses, int **fdin);
void startMergerProcess(int numProcesses, int **fdout);
void mergerProcess(int numProcesses, int **fdout);
int mergerWords(int numProcesses, char **words);
int **init2DintArray(int x, int y);
void free2DintArray(int **iArray, int x);
void closeFDarray(int **iArray, int xStart, int xEnd, int y);

char pre_word[MAX_WORD_LEN] = "";
int word_frequency = 1;

int main(int argc, char *argv[])
{
        int iProcesses, i;
        
        /* error checking for number of argc */
        if (argc != 2){
                printf("Invalid number of arguments\n");
                exit(EXIT_FAILURE);
        }
        
        /* make sure input argv is a int */
        if ((iProcesses = atoi(argv[1])) > 0) {
                //printf("Entered number: %d\n", iProcesses);
        } else {
                printf("Invalid number of processes\n");
                exit(EXIT_FAILURE);
        }
        
        /* initilize the 2D array of FDs to sort processes */
        int **fdinput = init2DintArray(iProcesses, 2);
        int **fdoutput = init2DintArray(iProcesses, 2);
        
        /* make numProcesses number of sort processes with FDs */
        makeSortProcesses(iProcesses, fdinput, fdoutput);
        
        /* read STDIN, give lines to sort processes */
        startParser(iProcesses, fdinput);
        
        /* Close all fds in parent parsing process so sort with EOF */
        closeFDarray(fdinput, 0, iProcesses, 2);
        
        /* Make merger process */
        startMergerProcess(iProcesses, fdoutput);
        
        /* Free the FD arrays */
        free2DintArray(fdinput, iProcesses);
        free2DintArray(fdoutput, iProcesses);
        
        /* wait for sorts to die */
        for (i = 0; i < iProcesses; i++) {
                wait(NULL);
        }
        
}

/* make numProcesses number of sort processes */
void makeSortProcesses(int numProcesses, int **fdin, int **fdout)
{
        int oldstdin;
        int oldstdout;
        int i;
        int qStatus;
        
        
        /* Save current STDIN/STDOUT of parent process*/
        oldstdin = dup(STDIN_FILENO);
        oldstdout = dup(STDOUT_FILENO);
        
        for (i = 0; i < numProcesses; i++){
                if (pipe(fdin[i]) == -1){
                        perror("error with creating pipe");
                        exit(EXIT_FAILURE);
                }
                if (pipe(fdout[i]) == -1){
                        perror("error with creating pipe");
                        exit(EXIT_FAILURE);
                }
                
                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                
                /* Make the read end of in to be stdin */
                if(dup2(fdin[i][0], STDIN_FILENO) == -1) {
                        perror("dup2 error");
                        exit(EXIT_FAILURE);
                }
                /* Make the write end of out to be stdout */
                if(dup2(fdout[i][1], STDOUT_FILENO) == -1) {
                        perror("dup2 error");
                        exit(EXIT_FAILURE);
                }
                
                switch(fork()){
                        case -1:
                                perror("forking error");
                                exit(EXIT_FAILURE);
                        case 0: /* CHILD */
                                /* close FDs in child process */
                                /*close(fdin[i][0]);
                                close(fdin[i][1]);
                                close(fdout[i][0]);
                                close(fdout[i][1]);*/
                                
                                closeFDarray(fdin, 0, i+1, 2);
                                closeFDarray(fdout, 0, i+1, 2);
                                execlp("sort", "sort", (char *)NULL);
                        default:
                                break; /* fall through to parent */
                }
                
                /* these are used by CHILD */
                close(fdin[i][0]);
                close(fdout[i][1]);
        
        }
        
        /* restore STDIN/STDOUT for parent process */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(oldstdin, STDIN_FILENO);
        dup2(oldstdout, STDOUT_FILENO);
        
}

/* read STDIN, give lines to sort processes */
void startParser(int numProcesses, int **fdin)
{
        int i, readnum;
        char buf[1];
        
        for (i = 0; i < numProcesses; i++) {
                //printf("Process #%d: ", i);
                while ((readnum = read(STDIN_FILENO, buf, 1)) != 0) {
                        /* check to see if alpha */
                        if (isalpha(buf[0])) {
                                if (isupper(buf[0])) buf[0] = buf[0] + 32;
                                write(fdin[i][1], buf, 1);
                                //printf("%c", buf[0]);
                        } else {
                                buf[0] = '\n';
                                write(fdin[i][1], buf, 1);
                                break;
                        }
                }
                
                if (readnum == 0) return;
                if (i == (numProcesses - 1)) i = -1;
                //printf("\n");
        }
}

/* output process starter */
void startMergerProcess(int numProcesses, int **fdout)
{
        int child_PID;
        //mergerProcess(numProcesses, fdout);
        
        switch(child_PID = fork()){
                case -1:
                        perror("forking error");
                        exit(EXIT_FAILURE);
                case 0: /* CHILD */
                        mergerProcess(numProcesses, fdout);
                        _exit(EXIT_SUCCESS);
                        break;
                default:
                        /* parent waits until output process is done */
                        waitpid(child_PID, NULL, 0);
                        break; 
        }

}

void mergerProcess(int numProcesses, int **fdout)
{
        int i, wrote=0;
        FILE *output[numProcesses];
        char **words;
        
        /* malloc 2D string array */
        words = (char**)malloc(numProcesses * sizeof(char*));
        for (i = 0; i < numProcesses; i++) {
                words[i] = (char*)malloc(MAX_WORD_LEN * sizeof(char));
        }
        
        /* open File Pointers and get first word from each process */
        for (i = 0; i < numProcesses; i++) {
                output[i] = fdopen(fdout[i][0], "r");
                if(fgets(words[i], MAX_WORD_LEN, output[i]) == NULL)
                        words[i] = NULL;
                //printf("process %d: %s\n", i, words[i]);
        }
        
        /* compare words gotten from each sort and print most alpha */
        while((wrote = mergerWords(numProcesses, words)) + 1) {
                if (fgets(words[wrote], MAX_WORD_LEN, output[wrote]) == NULL)
                        words[wrote] = NULL;
        }
        
        /* print last word */
        printf("%d %s", word_frequency, pre_word);
        
        /* free 2D string array */
        for (i = 0; i < numProcesses; i++) {
                free(words[i]);
        }
        free(words);
        
        /* close all file pointers and descriptions */
        for (i = 0; i < numProcesses; i++) {
                fclose(output[i]);
        }
}

/* compare words and print first alpha word, return with process's word was used*/
int mergerWords(int numProcesses, char **words)
{
        int i, alpha=0;
        
        /* checks that words[alpha] is not NULL */
        while (words[alpha] == NULL) {
                if (alpha == (numProcesses - 1)) { /* if all the words are null */
                        return -1;
                }
                alpha++;
        }
        
        /* finds the most aplha word */
        for (i = alpha + 1; i < numProcesses; i++)
                if (words[i] != NULL)
                        if (strcmp(words[i], words[alpha]) < 0) alpha = i;
        
        if (strcmp(pre_word, words[alpha]) != 0) {
                if (strcmp(pre_word, "") != 0)
                        printf("%d %s", word_frequency, pre_word);
                strcpy(pre_word, words[alpha]);
                word_frequency = 1;
        } else {
                word_frequency++;
        }
        
        
        
        return alpha;
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

/* Closes all the FDs in the iArray passed in */
void closeFDarray(int **iArray, int xStart, int xEnd, int y)
{
        int i, j;
        
        for (i = xStart; i < xEnd; i++) {
                for ( j = 0; j < y; j++) {
                        close(iArray[i][j]);
                }
        }
}
