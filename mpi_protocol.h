#ifndef MPI_PROTOCOL_H
#define MPI_PROTOCOL_H

#include <mpi.h>
#include<stddef.h>
#include"functionalities.h"

typedef struct JobMsg{
    int job_id;
    Command cmd;             // 1=PRIMES, 2=PRIMEDIVISORS, 3=ANAGRAMS
    long long n;          
    char client_id[32];   
    char name[9];        
} JobMsg;

enum{
    TAG_WORK=1,
    TAG_STOP=2,
    TAG_RESULT_HEADER=3
    TAG_RESULT_STRING=4
};

typedef struct ResultHeader{
    int job_id;
    int cmd;
    long long num_result;     
    int anagram_count;        
    int str_len;              
    char client_id[32];
} ResultHeader;

void send_job();
int recv_job();

#endif