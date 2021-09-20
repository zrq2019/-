#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
int main(int argc, char** argv) 
{
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int myid, N, numprocs, len, C = 0;
	double d, TotalSum, SqrtSum = 0.0;
	MPI_Status status;
	N = atoi(argv[1]);
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Get_processor_name(hostname, &len);
	//fflush(stdout);                             //解决输入在输出前面的问题
	double data[10000];
	for (int i = 0; i < N; ++i)
		data[i] = i * (i + 1);
	for (int i = myid; i < N; i = i + numprocs)
	{
		SqrtSum += sqrt(data[i]); 
		C++;
	}
	MPI_Reduce(&SqrtSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0)
    {
		printf("I am process 0 from %s. TotalSqrtSum=%f.\n", hostname, TotalSum);
    }
	printf("I am process %d from %s. I recv total %d numbers from process 0, and SqrtSum = %f.\n", myid, hostname, C, SqrtSum);
	MPI_Finalize();
}
