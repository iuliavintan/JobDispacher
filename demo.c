#include"functionalities.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int N=30;

    printf("Number of primes up to %d: %d\n", N, count_primes(N));

    long long M=452876;

    printf("Number of prime divisors of %lld: %d\n", M, prime_divisors(M));

    const char *name="abba";
    char *out=NULL;
    size_t length=0;
    int count=0;

    if(anagrams(name, &length, &out, &count)){
        printf("Anagrams of %s (total %d):\n%s", name, count, out);
        free(out);
    } else {
        printf("Failed to generate anagrams.\n");
    }

    return 0;
}