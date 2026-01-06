#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include <stddef.h>

int count_primes(int N);
/*find out how many primes there are in the first N natural numbers*/


int prime_divisors(long long N);
/*find out how many prime divisors has the number N*/


int anagrams(const char *name, size_t *length, char **result, int *count);
/* generates all anagrams (permutations) of name.*/

#endif