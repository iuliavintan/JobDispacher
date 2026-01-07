#ifndef MPI_PROTOCOL_H
#define MPI_PROTOCOL_H

#include <mpi.h>
#include<stddef.h>
#include"functionalities.h"
#include"command_parser.h"

#define MASTER 0

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
    TAG_RESULT_HEADER=3,
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

void send_job(const JobMsg *job, int dest_rank, int tag);
int recv_job(JobMsg* j, int source_rank, MPI_Status* out_status);
void send_result_hdr(const ResultHeader* r, int dest_rank);
int recv_result_hdr(ResultHeader* r, int* out_source_rank);

#endif

// {
//     "configurations": [
//         {
//             "name": "Linux",
//             "includePath": [
//                 "${workspaceFolder}/**"
//             ],
//             "defines": [],
//             "compilerPath": "/usr/bin/gcc",
//             "cStandard": "c17",
//             "cppStandard": "gnu++17",
//             "intelliSenseMode": "linux-gcc-x64"
//         }
//     ],
//     "version": 4
// }