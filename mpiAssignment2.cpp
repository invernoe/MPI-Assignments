#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define mcw MPI_COMM_WORLD

void fillVector(int* arr, int size) {
	for (int i = 0; i < size; i++)
		*(arr++) = rand();
}

void printVector(int* arr, int size) {
	printf("current array: {");
	for (int i = 0; i < size - 1; i++)
	{
		printf("%d, ", arr[i]);
	}
	printf("%d}\n", arr[size - 1]);
}

int main(int argc, char** argv) {
	const int CHUNK = 5, ARR_SIZE = 15;
	int size, myRank;
	MPI_Status status;

	MPI_Init(NULL, NULL);

	//define size and the current machine's rank
	MPI_Comm_size(mcw, &size);
	MPI_Comm_rank(mcw, &myRank);

	if (myRank == 0) {
		int data[ARR_SIZE];
		int sum = 0, sum1, sum2;

		fillVector(data, ARR_SIZE);
		printVector(data, ARR_SIZE);

		MPI_Send(&data[CHUNK], CHUNK, MPI_INT, 1, 0, mcw);
		MPI_Send(&data[CHUNK * 2], CHUNK, MPI_INT, 2, 0, mcw);

		for (int i = 0; i < 5; i++)
		{
			sum += data[i];
		}
		printf("rank %d sum: %d\n", myRank, sum);

		MPI_Recv(&sum1, 1, MPI_INT, 1, 0, mcw, &status);
		MPI_Recv(&sum2, 1, MPI_INT, 2, 0, mcw, &status);

		printf("collective sum: %d\n", sum + sum1 + sum2);
	}
	else {
		int data[CHUNK];
		int sum = 0;

		MPI_Recv(&data, CHUNK, MPI_INT, 0, 0, mcw, &status);

		for (int i = 0; i < 5; i++)
		{
			sum += data[i];
		}
		printf("rank %d sum: %d\n", myRank, sum);

		MPI_Send(&sum, 1, MPI_INT, 0, 0, mcw);
	}

	MPI_Finalize();
	return 0;
}