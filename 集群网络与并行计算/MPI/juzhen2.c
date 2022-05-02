#include <stdio.h>
#include "mpi.h"
#include"string.h"
#define MAX_ORDER 100

typedef float LOCAL_MATRIX_T[MAX_ORDER][MAX_ORDER];

int main(int argc, char* argv[])
{
    int             my_rank;
    int             p=4;
   LOCAL_MATRIX_T  local_A,local_B,local_C; 
    float           global_x[MAX_ORDER];
    float           local_x[MAX_ORDER];
    float           local_y[MAX_ORDER];
    int             m, n;
    int             local_m, local_n;
	//another matrix
	int				s,t;
	int				local_s,local_t;
	int				i,j,k;
	float			sum;
	//FILE			* ldw;
	void Read_matrix(char* prompt, LOCAL_MATRIX_T local_A, int local_m,int n, int my_rank, int p);
    void Print_matrix(char* title, LOCAL_MATRIX_T local_A, int local_m,
             int n, int my_rank, int p);
	void Parallel_matrix_matrix_prod(LOCAL_MATRIX_T local_A, int m,int  n,LOCAL_MATRIX_T local_B,int s, 
							int local_t,LOCAL_MATRIX_T  local_C);


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   if (my_rank == 0) {
        printf("输入矩阵A行和列数\n");
        scanf("%d %d", &m, &n);
		//读入另一个矩阵
		printf("输入矩阵B行和列数\n");
        scanf("%d %d", &s, &t);

    }

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);


	local_m = m;
    local_n = n;//m为行数，n为列数，p为进程总数

	local_s=s;
	local_t=t;
    Read_matrix("输入矩阵A", local_A, local_m, n, my_rank, p);//按行分块读入
    Print_matrix("矩阵A=", local_A, local_m, n, my_rank, p);

	 Read_matrix("输入矩阵B", local_B, local_s, t, my_rank, p);//按行分块读入
     Print_matrix("矩阵B=", local_B, local_s, t, my_rank, p);
	 Parallel_matrix_matrix_prod(local_A, m, n, local_B, s, 
							local_t, local_C);

	Print_matrix("A*B=", local_C, m, t, my_rank, p);

	/*******为了显示输出***********/
	int l;
	scanf("%d",&l);

    MPI_Finalize();
	return 0;

}  /* main */
void Read_matrix(char* prompt   /* in  */, 
         LOCAL_MATRIX_T  local_A  /* out */, 
         int             local_m  /* in  */, 
         int             n        /* in  */,
         int             my_rank  /* in  */, 
         int             p        /* in  */)
{

    int             i, j;
    LOCAL_MATRIX_T  temp;
    /* Fill dummy entries in temp with zeroes */
    for (i = 0; i < p*local_m; i++)
        for (j = n; j < MAX_ORDER; j++)
            temp[i][j] = 0.0;//矩阵的其余元素被0占满

    if (my_rank == 0)
	{
        printf("%s\n", prompt);
        for (i = 0; i < p*local_m; i++) 
            for (j = 0; j < n; j++)
                scanf("%f",&temp[i][j]);//读入要输入的矩阵的元素 
    }
    MPI_Scatter(temp, local_m*MAX_ORDER, MPI_FLOAT, local_A,
        local_m*MAX_ORDER, MPI_FLOAT, 0, MPI_COMM_WORLD);//若根进程，则读入矩阵元素；否则中将根进程存储了p个消息,第i个消息将传给第i个进程.
}  /* Read_matrix */


void Parallel_matrix_matrix_prod(
	LOCAL_MATRIX_T local_A,
	int m,
	int n,
	LOCAL_MATRIX_T local_B,
	int s,
	int local_t,
	LOCAL_MATRIX_T local_C)
{
	int i, j;
	int k, p, q, lie;
	float sum;
	float local_x[100];
	float global_x[100];
	//将第二个矩阵的每列globa_x
	for (i = 0; i < local_t; i++)
	{
		for (j = 0; j < s; j++)
		{
			local_x[j] = local_B[j][i];//为第二个矩阵每一列的值

			for (q = 0; q < m; q++)
			{
				local_C[q][i] = 0;
				for (p = 0; p < n; p++)
				{
					local_C[q][i] += local_A[q][p] * local_x[p];
				}
			}
		}

	}
}
	void Print_matrix(
		char* title      		/* in */,
		LOCAL_MATRIX_T  local_A    	/* in */,
		int             local_m    		/* in */,
		int             n          		/* in */,
		int             my_rank    		/* in */,
		int             p          		/* in */) {

		int   i, j;
		float temp[MAX_ORDER][MAX_ORDER];

		MPI_Gather(local_A, local_m * MAX_ORDER, MPI_FLOAT, temp,
			local_m * MAX_ORDER, MPI_FLOAT, 0, MPI_COMM_WORLD);
		if (my_rank == 0) {
			printf("%s\n", title);
			for (i = 0; i < p * local_m; i++) {
				for (j = 0; j < n; j++)
					printf("%4.1f ", temp[i][j]);
				printf("\n");
			}
		}
	}
