#include <stdio.h>
#include <mpi.h>
#define mcw MPI_COMM_WORLD

int main(int argc, char** argv) {
	int size, myRank, data = 0;
	MPI_Status status;

	MPI_Init(NULL, NULL);

	//define size and the current machine's rank
	MPI_Comm_size(mcw, &size);
	MPI_Comm_rank(mcw, &myRank);

	switch (myRank) {
		case 0:
			MPI_Send(&data, 1, MPI_INT, 1, 0, mcw);
			MPI_Recv(&data, 1, MPI_INT, 3, 0, mcw, &status);
			printf("rank %d received data %d from rank %d", myRank, data, status.MPI_SOURCE);
			break;
		case 1:
			MPI_Send(&data, 1, MPI_INT, 2, 0, mcw);
			MPI_Recv(&data, 1, MPI_INT, 0, 0, mcw, &status);
			printf("rank %d received data %d from rank %d", myRank, data, status.MPI_SOURCE);
			break;
		case 2:
			MPI_Send(&data, 1, MPI_INT, 3, 0, mcw);
			MPI_Recv(&data, 1, MPI_INT, 1, 0, mcw, &status);
			printf("rank %d received data %d from rank %d", myRank, data, status.MPI_SOURCE);
			break;
		case 3:
			MPI_Send(&data, 1, MPI_INT, 0, 0, mcw);
			MPI_Recv(&data, 1, MPI_INT, 2, 0, mcw, &status);
			printf("rank %d received data %d from rank %d", myRank, data, status.MPI_SOURCE);
			break;
		default:
			printf("error");
			break;
	}

	MPI_Finalize();
	return 0;
}