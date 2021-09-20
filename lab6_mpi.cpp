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
	int size, myRank;
	MPI_Status status;

	MPI_Init(NULL, NULL);

	//define size and the current machine's rank
	MPI_Comm_size(mcw, &size);
	MPI_Comm_rank(mcw, &myRank);

	//initialize array size and chunk size (assuming that the array can be distributed equally to n number of nodes)
	const int ARR_SIZE = 100, CHUNK = 25;

	//actual arrays initialized in rank 0 node
	int arr1[ARR_SIZE];
	int arr2[ARR_SIZE];

	//local arrays for every node that only take chunks of data
	int arr1Local[CHUNK];
	int arr2Local[CHUNK];

	if (myRank == 0) {
		fillVector(arr1, ARR_SIZE);
		fillVector(arr2, ARR_SIZE);
	}

	MPI_Scatter(&arr1, CHUNK, MPI_INT, &arr1Local, CHUNK, MPI_INT, 0, mcw);
	MPI_Scatter(&arr2, CHUNK, MPI_INT, &arr2Local, CHUNK, MPI_INT, 0, mcw);

	int localSum = 0;
	int finalSum = 0;
	for (int i = 0; i < CHUNK; i++)
	{
		localSum += arr1Local[i] * arr2Local[i];
	}

	MPI_Reduce(&localSum, &finalSum, 1, MPI_INT, MPI_SUM, 0, mcw);
	if(myRank == 0)
		printf("dot product = %d\n", finalSum);

	MPI_Finalize();
	return 0;
}