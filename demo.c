#include"functionalities.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int N=30;

    printf("Number of primes up to %d: %d\n", N, count_primes(N));

    long long M=452876;

    printf("Number of prime divisors of %lld: %d\n", M, prime_divisors(M));

    return 0;
}