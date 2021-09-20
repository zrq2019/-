//MPI_Send,MPI_Recv版
#define N 100000         //定义小矩形个数
#define a 10              //定义积分区间
#define b 100
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
int main(int argc, char** argv) 
{
	int myid, numprocs, C = 0;
	double inte=0;
	double dx = (double)(b - a) / N;
	double x;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	--numprocs;
	//printf("%d", numprocs);
	if (myid == 0) 
	{
		double integral = 0.0;
		for (int source = 1; source <= numprocs; ++source) 
		{
			MPI_Recv(&inte, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status);
			integral += inte;
		}
		printf("I am process 0. The integral of x^2 on [%d,%d] is %f.\n", a,b,integral);
	}
	else 
	{
		for (int i = myid - 1; i < N; i = i + numprocs) 
		{
			x = a + i * dx + dx / 2;
			//printf("x:%f", x);
			inte += x * x * x * dx;
			C++;
		}
		MPI_Send(&inte, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
		printf("I am process %d. I calculated total %d rectangles, and sum=% f.\n", myid, C, inte);
	}
	MPI_Finalize();
}
