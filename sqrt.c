//MPI_Reduce��
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
    int myid, N, source, numprocs;
    double d, TotalSum, SqrtSum = 0.0;
    MPI_Status status;
    N = atoi(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    double data[10000];
    for (int i = 0; i < N; ++i)
        data[i] = i * (i+1);
    for (int i = myid; i < N; i = i + numprocs)
    {
        //MPI_Recv(&d, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        //printf("%f\n", data[i]);
        SqrtSum += sqrt(data[i]);
    }
    MPI_Reduce(&SqrtSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);//���з�0���̵ļ�������Լ��0����
    if (myid == 0)
    {
        printf("I am process 0. TotalSqrtSum = %f.\n", TotalSum);
    }
    printf("I am process %d. I calculated total %f SqrtSum.\n", myid, SqrtSum);
    MPI_Finalize();
}
