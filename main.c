#include <mpi.h>
#include <stdio.h>

#include "master_worker.h"
#include "mpi_protocol.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = -1, comm_sz = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (rank == MASTER) {
        if (argc != 2) {
            fprintf(stderr, "Usage: mpirun -np <P> %s <command_file>\n", argv[0]);
        } else {
            if (comm_sz == 1) {
                run_serial(argv[1]);          
            } else {
                master(argv[1], comm_sz);     
            }
        }
    } else {
        worker();
    }

    MPI_Finalize();
    return 0;
}