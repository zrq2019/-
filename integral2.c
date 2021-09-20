#define N 1000000
#define a 10
#define b 100
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
int main(int argc, char** argv) 
{
	char hostname[MPI_MAX_PROCESSOR_NAME];
	int myid, numprocs, C = 0, len;
	int i;
	double inte=0;
	double dx = (double)(b - a) / N;
	double x;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	--numprocs;
	//printf("%d", numprocs);
	MPI_Get_processor_name(hostname, &len);
	if (myid == 0) 
	{
		clock_t start, end; //加入运行计时功能，方便后续分析进程数和节点数对运行时间的影响
		double integral = 0.0;
		start = clock();
		for (int source = 1; source <= numprocs; ++source) 
		{
			MPI_Recv(&inte, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
			integral += inte;
		}
		end = clock();
		printf("I am process 0 from %s. The integral of x^2 on [0,10] is %f. Running time is % f seconds.\n", hostname, integral, ((double)(end - start) / CLOCKS_PER_SEC));
	}
	else 
	{
		for (i = myid - 1; i < N; i = i + numprocs) 
		{
			x = a + i * dx + dx / 2;
			//printf("x:%f\n", x);
			inte += x * x * x * dx;
			C++;
		}
		MPI_Send(&inte, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
		printf("I am process %d from %s. I calculated total %d rectangles, and sum = % f.\n", myid, hostname, C, inte);
	}
	MPI_Finalize();
}
