#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define N 8
#define SIZE N / 4
#define T 2
void print_myRows(int, float[][N]);
int main(int argc, char *argv[])
{
    
    float myRows[SIZE + 2][N], myRows2[SIZE + 2][N];
    int myid;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    int i, j;
    /*初始化*/
    for (i = 0; i < SIZE + 2; i++)
    {
        for (j = 0; j < N; j++)
        {
            myRows[i][j] = myRows2[i][j] = 0;
        }
    }
    if (0 == myid)
    {
        for (j = 0; j < N; j++)
            myRows[1][j] = 4.0;
    }
    if (3 == myid)
    {
        for (j = 0; j < N; j++)
            myRows[SIZE][j] = 4.0;
    }
    for (i = 1; i < SIZE + 1; i++)
    {
        myRows[i][0] = 4.0;
        myRows[i][N - 1] = 4.0;
    }
    /*Jacobi Iteration部分*/
    int step;
    for (step = 0; step < T; step++)
    {
        // 传递数据
        if (myid < 3)
        {
            // 从 下方 进程接收数据
            MPI_Recv(&myRows[SIZE + 1][0], N, MPI_FLOAT, myid + 1, 0, MPI_COMM_WORLD, &status);
        }
        if (myid > 0)
        {
            // 向 上方 进程发送数据
            MPI_Send(&myRows[1][0], N, MPI_FLOAT, myid - 1, 0, MPI_COMM_WORLD);
        }
        if (myid < 3)
        {
            // 向 下方 进程发送数据
            MPI_Send(&myRows[SIZE][0], N, MPI_FLOAT, myid + 1, 0, MPI_COMM_WORLD);
        }
        if (myid > 0)
        {
            // 从 上方 进程接收数据
            MPI_Recv(&myRows[0][0], N, MPI_FLOAT, myid - 1, 0, MPI_COMM_WORLD, &status);
        }
        // 计算
        int r_begin, r_end;
        r_begin = (0 == myid) ? 2 : 1;
        r_end = (3 == myid) ? SIZE - 1 : SIZE;
        for (i = r_begin; i <= r_end; i++)
        {
            for (j = 1; j < N - 1; j++)
                myRows2[i][j] = 0.25 * (myRows[i][j - 1] + myRows[i][j + 1] + myRows[i - 1][j] + myRows[i + 1][j]);
        }
        // 更新
        for (i = r_begin; i <= r_end; i++)
        {
            for (j = 1; j < N - 1; j++)
                myRows[i][j] = myRows2[i][j];
        }
    }
    // MPI_Barrier(MPI_COMM_WORLD);
    print_myRows(myid, myRows);
    MPI_Finalize();
}
void print_myRows(int myid, float myRows[][N])
{
    int i, j;
    int buf[1];
    MPI_Status status;
    buf[0] = 1;
    if (myid > 0)
    {
        MPI_Recv(buf, 1, MPI_INT, myid - 1, 0, MPI_COMM_WORLD, &status);
    }
    printf("Result in process %d:\n", myid);
    for (i = 0; i < SIZE + 2; i++)
    {
        for (j = 0; j < N; j++)
            printf("%1.3f\t", myRows[i][j]);
        printf("\n");
    }
    if (myid < 3)
    {
        MPI_Send(buf, 1, MPI_INT, myid + 1, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
