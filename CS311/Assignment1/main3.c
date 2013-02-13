#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int *nPrimes(int n, int* count)
{
    int *allNums = malloc(n * sizeof(int));
    int k; //used for identifing primes and marking multiples of itself
    int i;
    int primeCount;
    
    //assign numbers to n in array
    //for (i=1;i<n;i++)
    //    allNums[i]=i;
    
    allNums[0]=1;
    allNums[1]=1; //Special case
    k=2;
    while(k<=sqrt(n)){
        for (i=2;i*k<n;i++)
            allNums[i*k]=1;
        
        while(allNums[++k]==1); //find next # not identified as composite
    }
    
    primeCount=0;
    //count the number of primes
    for (i=0;i<n;i++)
        if (allNums[i]!=1)
            primeCount++;
    
    //store all the primes in seprate list
    int *primes = malloc(primeCount * sizeof(int));
    *count = primeCount;
    primeCount=0;
    primes[primeCount++]=2;
    for (i=3;i<n;i=i+2)
        if (allNums[i]==0)
            primes[primeCount++]=i;
    
    free(allNums);
    return primes;
}

int main(int argc, char *argv[])
{
    int pCount;
    int* primes = NULL;
    int n;
    
    printf("Enter n: ");
    scanf("%d", &n);
    
    primes = nPrimes(n, &pCount);
    
    //for(int i=0;i<pCount;i++)
    //    printf("%d\n", primes[i]);
    
    printf("count: %d\n", pCount);

    free(primes);
    
    return 0;
}
