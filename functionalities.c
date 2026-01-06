#include"functionalities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_primes(int N){
    if(N < 2) return 0;

    unsigned char *is_prime = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    if(is_prime == NULL){
        perror("mem insuf");
        exit(-1);
    }

    memset(is_prime, 1, (N + 1) * sizeof(unsigned char));
    is_prime[0] = 0;
    is_prime[1] = 0;

    for(int i = 2; i * i <= N; i++){
        if(is_prime[i]){
            for(int j = i * i; j <= N; j += i){
                is_prime[j] = 0;
            }
        }
    }

    int count=0;
    for(int i = 2; i <= N; i++){
        if(is_prime[i]) count++;
    }
    free(is_prime);
    return count;
}

int prime_divisors(long long N){
    if(N <= 1) return 0;
    int count=0;

    if(N%2==0){
        count++;
        while(N%2==0) N/=2;
    }

    for(long long i=3; i*i<=N; i+=2){
        if(N%i==0){
            count++;
            while(N%i==0) N/=i;
        }
    }

    if(N>1) count++;
    return count;
}