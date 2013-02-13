/*
 *  primes.c
 *
 *
 *  Created by Vinay Bikkina on 11/15/12.
 *
 */
#define _POSIX_SOURCE
#define _BSD_SOURCE

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_NUMS 128

void initial_errorchk(int argc, char *argv[]);
void start_calc(unsigned int n, unsigned int *count);
void process_primes();
void *process_primes_thread(void * vp);
unsigned int next_k(unsigned int k);
void init_bitmap(unsigned char *bitmap, unsigned int bitmap_size);
void *mount_shmem(char *path, unsigned int object_size);
void primes_k(int n, unsigned char *kbitmap);

int shmem_fd, num_proc;
char proc_type;
unsigned char *bitmap;
unsigned char *bitmap_multis;
unsigned int max_prime = MAX_NUMS;

int main(int argc, char *argv[])
{
    unsigned int bitmap_size;
    unsigned int bitmap_multis_size;
    void *shmaddr;
    unsigned int count;
    
    printf("Starting initial check\n");
    initial_errorchk(argc, argv);
    printf("Finished initial check\n");
    
    /* update sizes */
    bitmap_size = ((max_prime / 8) + 1);
    bitmap_multis_size = ((sqrt(max_prime) / 8) + 1);
    
    /* make shared memory */
    printf("Starting shared memory init\n");
    shmaddr = mount_shmem("primesbitmap", bitmap_size + bitmap_multis_size + 2);
    bitmap = shmaddr;
    bitmap_multis = (unsigned char *)shmaddr + bitmap_size;
    init_bitmap(bitmap, bitmap_size);
    init_bitmap(bitmap_multis, bitmap_multis_size);
    printf("Finished shared memory init\n");
    
    start_calc(max_prime, &count);
    
    printf("%d primes found.\n", count);
    
    close(shmem_fd);
}

void initial_errorchk(int argc, char *argv[])
{
    unsigned int temp;
    
    /* error checking for number of argc */
    if (argc != 3 && argc != 4){
        printf("Invalid number of arguments\n" \
               "Usage: primes [p|t] [#processes/threads]\n");
        exit(EXIT_FAILURE);
    }
    
    /* make sure input argv is a int */
    if ((num_proc = atoi(argv[2])) > 0) {
        //printf("Entered number: %d\n", num_proc);
    } else {
        printf("Invalid number of processes/threads\n");
        exit(EXIT_FAILURE);
    }
    
    /* get procType */
    proc_type = argv[1][0];
    if (proc_type != 'p' && proc_type != 't') {
        printf("Invalid command.\nUsage: primes [p|t] [#processes/threads]\n");
        exit(EXIT_FAILURE);
    }

    /* manual max_primes? */
    if (argc > 3)
        if ((temp = atoi(argv[3])) > 0)
            max_prime = temp;
            
}

void start_calc(unsigned int n, unsigned int *count)
{
    unsigned int i;
    unsigned int primes_count;
    
    bitmap[0] = 0b10101100; //Special case, set bits 0 and 1 off, bit 2 on
    bitmap_multis[0] &= ~(1 << 0 | 1 << 1 | 1 << 2); //bitmap init with odds only
    
    /* fill bitmap_multis */
    primes_k(sqrt(max_prime), bitmap_multis);
    
    /* thread or multi-process with this function */
    //process_primes();
    
    if (proc_type == 'p') {
        //initProcesses;
    } else if (proc_type == 't') {
        pthread_t *thread;		/* thread object */
        pthread_attr_t attr;			/* set of thread attributes for thread */
        
         /* Allocate the number of pthreads given by num_proc */
        thread = (pthread_t*)malloc(num_proc * sizeof(pthread_t));
        
        /* initialize thread attribute with defaults */
        pthread_attr_init(&attr);
    
        /* Create threads that will each execute mult_matrix */
        /* i is used as the thread id, which is passed as the thread's only argument */
        for (i = 0; i < num_proc; i++) {
            if (pthread_create(&thread[i], 			 	/* thread */
                               &attr, 				 	/* thread attribute */
                               process_primes_thread, 	/* function to execute */
                               NULL) != 0) {            /* thread argument */
                perror("Cannot create the thread.");
                exit(-1);
            }
        }
        
        for (i = 0; i < num_proc; i++) {
            pthread_join(thread[i], NULL);
        }
    }
    
    *count=0;
    //count the number of primes
    for (i=0;i<=n;i++)
        if (bitmap[i/8] & (1 << (i%8)))
            (*count)++;
            
}

void process_primes()
{
    sem_t *sem = sem_open("/primes", O_CREAT, 0777, 0);
    unsigned int i, k=0;
    
    printf("%d: About to wait on semaphore\n", getpid());
    sem_wait(sem);
    printf("%d: obtained semaphore, about to \"work\"\n", getpid());
    k = next_k(k);
    printf("%d: Released semaphore\n", getpid());
    sem_post(sem);
    do {
        for (i=2; i*k<=max_prime; i++) {
            bitmap[(i*k)/8] &= ~(1 << ((i*k)%8)); /* set bit off, not prime */
        }
        
        printf("%d: About to wait on semaphore\n", getpid());
        sem_wait(sem);
        printf("%d: obtained semaphore, about to \"work\"\n", getpid());
        k = next_k(k);
        printf("%d: Released semaphore\n", getpid());
        sem_post(sem);
    } while (k != 0);
    
    sem_close(sem);
}

void *process_primes_thread(void * vp)
{
    process_primes();
    
    pthread_exit(0); /* exit */
}

/* returns next k in bitmap_multis else return 0 to indicate end */
unsigned int next_k(unsigned int k)
{
    unsigned int n;
    for (n=k+1; n<=sqrt(max_prime); n++)
        if (bitmap_multis[n/8] & (1 << (n%8))){ /* if bit n is set */
            return n;
        }
    return 0;
}

/* initialize bitmap with odds */
void init_bitmap(unsigned char *initbitmap, unsigned int bitmap_size)
{
    unsigned int i;
    for(i = 0; i < bitmap_size; i++)
        initbitmap[i] = 170;
}

void *mount_shmem(char *path, unsigned int object_size)
{
	void *addr;
    
	/* create and resize it */
	shmem_fd = shm_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (shmem_fd == -1){
		fprintf(stdout, "failed to open shared memory object\n");
		exit(EXIT_FAILURE);
	}
	/* resize it to something reasonable */
	if (ftruncate(shmem_fd, object_size) == -1){
		fprintf(stdout, "failed to resize shared memory object\n");
		exit(EXIT_FAILURE);
	}
    
	addr = mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmem_fd, 0);
	if (addr == MAP_FAILED){
		fprintf(stdout, "failed to map shared memory object\n");
		exit(EXIT_FAILURE);
	}
    
	return addr;
}

void primes_k(int n, unsigned char *kbitmap)
{
    int k=2, i; //k is used for identifing primes and marking multiples of itself
    
    while (k <= sqrt(n)) {
        for (i = 2; i * k <= n; i++)
            kbitmap[(i*k) / 8] &= ~(1 << ((i*k) % 8)); /* set bit to off */
        
        do
            k++;
        while((bitmap_multis[(i*k)/8] & (1 << ((i*k)%8))) != 0);
    }
}
