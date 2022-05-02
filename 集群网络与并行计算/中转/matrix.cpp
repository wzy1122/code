#include <stdio.h>
#include <math.h>
#include "mpi.h"
int main(int argc, char *argv[])
{
    int m, n;
    float s;
    float a[20][20], b[20][20], c[20][20];
    int i, j, k;
    int rank, size, namelen;
    char pro_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(pro_name, &namelen);
    if (0 == rank)
    {

        printf("请输入矩阵行数：");
        printf("%d\n", rank);
        scanf("%d", &m);

        printf("请输入矩阵列数：");
        scanf("%d", &n);

        printf("请输入第一个矩阵：");
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                scanf("%f", &a[i][j]);

        printf("请输入第二个矩阵：");
        for (i = 0; i < n; i++)
            for (j = 0; j < m; j++)
                scanf("%4f", &b[i][j]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(a, m * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            printf("%f ", a[i][j]);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            printf("%4f ", b[i][j]);
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < m; j++)
        {
            s = 0;
            for (k = 0; k < n; k++)
            {
                s = s + a[i][k] * b[k][j];
                c[i][j] = s;
                printf("%d%d", c[i][j], rank);
            }
        }
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%4f rank= %d", c[i][j], rank);
        }
        printf("\n");
    }

    // printf("%d",size);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
