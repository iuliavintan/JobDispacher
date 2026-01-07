#include"master_worker.h"
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include"mpi_protocol.h"
#include"command_parser.h"
#include"functionalities.h"

static int find_free_worker(int comm_sz, int* worker_free) {
    for (int r = 1; r < comm_sz; r++) {
        if (worker_free[r]) return r;
    }
    return -1;
}

static void write_to_client_file(const char* client_id, const char* text) {
    char fname[64];
    snprintf(fname, sizeof(fname), "%s.out", client_id);

    FILE* f = fopen(fname, "a");
    if (!f) { perror("fopen client out"); return; }
    fputs(text, f);
    fclose(f);
}

static void receive_res_write(int *worker_free){
    ResultHeader rh;
        int src = -1;
    recv_result_hdr(&rh, &src);
    
    worker_free[src] = 1;

    if (rh.cmd == ANAGRAMS && rh.str_len > 0) {
        char* buf = (char*)malloc((size_t)rh.str_len + 1);
        if (!buf) return;

        MPI_Recv(buf, rh.str_len, MPI_CHAR, src, TAG_RESULT_STRING, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        buf[rh.str_len] = '\0';

        char header_line[256];
        snprintf(header_line, sizeof(header_line),"JOB %d ANAGRAMS => %d anagrams\n", rh.job_id, rh.anagram_count);

        write_to_client_file(rh.client_id, header_line);
        write_to_client_file(rh.client_id, buf);
        write_to_client_file(rh.client_id, "\n");

        free(buf);
    } else {
        char line[256];
        if (rh.cmd == PRIMES) {
            snprintf(line, sizeof(line), "JOB %d PRIMES => %lld\n", rh.job_id, rh.num_result);
        } else {
            snprintf(line, sizeof(line), "JOB %d PRIMEDIVISORS => %lld\n", rh.job_id, rh.num_result);
        }
        write_to_client_file(rh.client_id, line);
    }
}


void master(const char *cmd_file, int comm_size){
    if(comm_size<2){
        fprintf(stderr, "At least 2 processes are required (1 master + 1 worker)\n");
        return;
    }

    int *worker_free = (int*)malloc((comm_size)*sizeof(int));
    if(worker_free==NULL){
        perror("mem induf");
        return;
    }
    worker_free[0]=0;
    for(int i=1;i<comm_size;i++){
        worker_free[i]=1; 
    }

    FILE *fp=fopen(cmd_file, "r");
    if(fp==NULL){
        perror("fopen failed");
        free(worker_free);
        exit(EXIT_FAILURE);
    }

    char line[256];
    int active_jobs=0;
    int next_job_id=1;

    while(fgets(line, sizeof(line), fp)!=NULL){
        ParsedLine pl;
        LineType lt=parse_line(line, &pl);
        if(lt==EMPTY) continue;
        else if(lt==INVALID){
            fprintf(stderr, "Invalid command: %s", line);
            continue;
        }else if(lt==WAIT){
            sleep(pl.seconds);
            continue;
        }

        //JOB

        int worker = find_free_worker(comm_size, worker_free);
        while(worker==-1){
            receive_res_write(worker_free);
            active_jobs--;
            worker= find_free_worker(comm_size, worker_free);
        }

        JobMsg job;
        memset(&job, 0, sizeof(JobMsg));
        job.job_id=next_job_id++;
        snprintf(job.client_id, sizeof(job.client_id), "%s", pl.job.id);

        job.cmd=pl.job.command;
        job.n=pl.job.n;

        if(pl.job.command==ANAGRAMS){
            snprintf(job.name, sizeof(job.name), "%s", pl.job.name);
        }else{
            job.name[0]='\0';
        }

        send_job(&job, worker, TAG_WORK);
        worker_free[worker]=0;
        active_jobs++;
    }

    fclose(fp);

    while(active_jobs>0){
        receive_res_write(worker_free);
        active_jobs--;
    }

   for (int w = 1; w < comm_size; w++) {
        JobMsg dummy;
        memset(&dummy, 0, sizeof(dummy));
        send_job(&dummy, w, TAG_STOP);
    }
     free(worker_free);
}

void worker(void) {
    while (1) {
        JobMsg job;
        MPI_Status st;
        int ok = recv_job(&job, MASTER, &st);
        if (!ok) break; //stop

        ResultHeader hdr;
        memset(&hdr, 0, sizeof(hdr));
        hdr.job_id = job.job_id;
        hdr.cmd = job.cmd;
        snprintf(hdr.client_id, sizeof(hdr.client_id), "%s", job.client_id);

        if (job.cmd == PRIMES) {
            long long ans = (long long)count_primes((int)job.n);
            hdr.num_result = ans;
            hdr.anagram_count = 0;
            hdr.str_len = 0;
            send_result_hdr(&hdr, MASTER);

        } else if (job.cmd == PRIMEDIVISORS) {
            long long ans = (long long)prime_divisors(job.n);
            hdr.num_result = ans;
            hdr.anagram_count = 0;
            hdr.str_len = 0;
            send_result_hdr(&hdr, MASTER);

        } else if (job.cmd == ANAGRAMS) {
            char* out = NULL;
            size_t len = 0;
            int count = 0;

            if (!anagrams(job.name, &len, &out, &count) || !out) {
                hdr.num_result = 0;
                hdr.anagram_count = 0;
                hdr.str_len = 0;
                send_result_hdr(&hdr, MASTER);
            } else {
                hdr.num_result = 0;
                hdr.anagram_count = count;
                hdr.str_len = (int)len;

                send_result_hdr(&hdr, MASTER);
                if (hdr.str_len > 0) {
                    MPI_Send(out, hdr.str_len, MPI_CHAR, MASTER, TAG_RESULT_STRING, MPI_COMM_WORLD);
                }
                free(out);
            }
        }
    }
}
