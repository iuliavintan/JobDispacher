#ifndef MASTER_WORKER_H
#define MASTER_WORKER_H

typedef struct LogEntry {
    int job_id;
    char client_id[32];
    int cmd;              
    long long n;          
    char name[9];         
    int worker_rank;

    double t_received;
    double t_dispatched;
    double t_finished;
} LogEntry;

void master(const char *cmd_file, int comm_size);
void worker();
void run_serial(const char* cmd_file);
#endif