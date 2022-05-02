#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <offload.h>

#define N 5
#define M 5

	__attribute__((target (mic)))
void MatAdd(float** A,float** B,float** C,int n,int m)
{
	#pragma omp parallel
	{
		#pragma omp for 
		for (int i=0;i<n;i++)
			{
				for (int j=0;j<m;j++)
				C[i][j]=A[i][j]+B[i][j];
			}
	}
}

	__attribute__((target (mic)))
void MatMulti(float** A,float** B,float** C,int n,int m)
{
	#pragma omp parallel
	{
		#pragma omp for 
		for (int i=0;i<n;i++)
		{
			for (int j=0;j<n;j++){
				C[i][j]=0.0;
				for (int k=0;k<m;k++)
				{
					C[i][j] += A[i][k] * B[k][j];    
				}
			}
		}
	}
}
int main( int argc,char** argv)
{
	int i,j;
	float *in1;
	float **A,**B,**C,**D,**E; 
	A=(float**)malloc(sizeof(float*)*N);  
	for(int i=0;i<N;i++)  
	A[i]=(float*)malloc(sizeof(float)*M); 
	B=(float**)malloc(sizeof(float*)*M);  
	for(int i=0;i<M;i++)  
	B[i]=(float*)malloc(sizeof(float)*N); 
	C=(float**)malloc(sizeof(float*)*N);  
	for(int i=0;i<N;i++)  
	C[i]=(float*)malloc(sizeof(float)*N); 
	D=(float**)malloc(sizeof(float*)*N);  
	for(int i=0;i<N;i++)  
	D[i]=(float*)malloc(sizeof(float)*N); 
	E=(float**)malloc(sizeof(float*)*N);  
	for(int i=0;i<N;i++)  
	E[i]=(float*)malloc(sizeof(float)*N); 
	
	for (int i=0;i<N;i++)
	{
		for (int j=0;j<M;j++)
		{
			A[i][j]=i+1;
			B[j][i]=i+2;
		}
	}  
	for (int i=0;i<N;i++)
		{
		for (int j=0;j<N;j++)
		{
			C[i][j]=i+3;
		}
	} 
	
	#pragma offload target(mic) in(A:length(N)) in(B:length(M)) out(E:length(N))
	{
	MatMulti(A,B,E,N,M);
	}
	
	#pragma offload target(mic) in(E,C:length(N)) out(D:length(N))
	{
	MatAdd(E,C,D,N,N);
	}

	for (int i=0;i<N;i++)
	{
		for (int j=0;j<N;j++)
		printf("%d,%d,%f\n",i,j,D[i][j]);
	}

	free(A);
	free(B);
	free(C);
	free(D);
	free(E);
}
