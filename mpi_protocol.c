#include"mpi_protocol.h"

void send_job(const JobMsg *job, int dest_rank, int tag)
{
    MPI_Send((void*)&job->job_id, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);

    if(tag==TAG_STOP) return;

    MPI_Send((void*)&job->command, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
    MPI_Send((void*)&jpb->n, 1, MPI_LONG_LONG, dest_rank, tag, MPI_COMM_WORLD);
    MPI_Send((void*)job->id, 32, MPI_CHAR, dest_rank, tag, MPI_COMM_WORLD);
    MPI_Send((void*)job->name, 9, MPI_CHAR, dest_rank, tag, MPI_COMM_WORLD);
}

int recv_job(JobMsg* j, int source_rank, MPI_Status* out_status) {
    MPI_Status st;

    MPI_Recv((void*)&j->job_id, 1, MPI_INT, source_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &st);

    if (out_status) *out_status = st;

    if (st.MPI_TAG == TAG_STOP) return 0;

    int src = st.MPI_SOURCE;
    int tag = st.MPI_TAG;

    MPI_Recv((void*)&j->command, 1, MPI_INT, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv((void*)&j->n, 1, MPI_LONG_LONG, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv((void*)j->id, 32, MPI_CHAR, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv((void*)j->name, 9, MPI_CHAR, src, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    j->id[31] = '\0';
    j->name[8] = '\0';

    return 1; 
}