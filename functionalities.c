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

//8!=40320 man no of anagrams with 8 letters

typedef struct DynBuffer{
    char *data;
    int len;
    int cap;
}DynBuffer;

static int db_init(DynBuffer *db, int initial_cap){
    db->data = (char *)malloc(initial_cap * sizeof(char));
    if(db->data == NULL){
        perror("mem insuf");
        exit(-1);
    }
    db->len = 0;
    db->cap = initial_cap;
    db->data[0] = '\0';
    return 1;
}

static int db_verify_space(DynBuffer *db, int extra_len){
    if(db->len + extra_len + 1 <= db->cap) return 1;
    int new_cap = db->cap;
    while(new_cap<=db->len + extra_len + 1){
        new_cap *= 2;
    }   
    char *new_data = (char *)realloc(db->data, new_cap * sizeof(char));
    if(new_data == NULL){
        perror("mem insuf");
        exit(-1);
    }
    db->data = new_data;
    db->cap = new_cap;
    return 1;
}

static int db_append(DynBuffer *db, const char *str, int n){
    if(!db_verify_space(db, n)) return 0;
    memcpy(db->data + db->len, str, n);
    db->len += n;
    db->data[db->len] = '\0';
    return 1;
}

static int generate_perm(const char *sorted, int n, int *used, char *current, int depth, DynBuffer *db, int *count){
    if(depth == n){
        if(db_append(db, current, n)==0) return 0;
        if(db_append(db, "\n", 1)==0) return 0;
        (*count)++;
        return 1;
    }

    for(int i=0; i<n; i++){
        if(used[i]) continue;
        if(i>0 && sorted[i]==sorted[i-1] && !used[i-1]) continue;

        used[i] = 1;
        current[depth] = sorted[i];

        if(!generate_perm(sorted, n, used, current, depth+1, db, count)) return 0;
        used[i] = 0;
    }
    return 1;
}

static int char_cmp(const void *a, const void *b){
    char ca = *(const char *)a;
    char cb = *(const char *)b;
    return (ca > cb) - (ca < cb);
}

int anagrams(const char *name, size_t *length, char **result, int *count){
    if(name==NULL || length==NULL || result==NULL || count==NULL) return 0;

    size_t n = strlen(name);
    if(n > 8) return 0;
    if(n==0){
        *result=(char*)malloc(1);
        if(*result==NULL){
            perror("mem insuf");
            exit(-1);
        }
        (*result)[0]='\0';
        *count=0;
        *length=0;
        return 1;
    }

    char *sorted = (char *)malloc(n * sizeof(char));
    int *used = (int *)calloc(n, sizeof(int));
    char *current = (char *)malloc(n*sizeof(char)); // we dont need '\0'

    if(sorted==NULL || used==NULL || current==NULL){
        free(sorted);
        free(used);
        free(current);
        perror("mem insuf");
        exit(-1);
    }

    memcpy(sorted, name, n);
    qsort(sorted, n, sizeof(char), char_cmp);

    DynBuffer db;
    if(db_init(&db, 128)==0){
        free(sorted);
        free(used);
        free(current);
        return 0;
    }

    int counter=0;
    int ok = generate_perm(sorted, n, used, current, 0, &db, &counter);

    free(sorted);
    free(used);
    free(current);

    if(!ok){
        free(db.data);
        return 0;
    }

    *result=db.data;
    *length=db.len;
    *count=counter;

    return 1;
}