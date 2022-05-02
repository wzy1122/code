#include<iostream>                                                                                                       
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
 
using namespace std;
 
const int N = 10;
const int D = 8;
const int MAX = 10;
 
__global__ void cal_dis(int *train_data, int *test_data, int *dis,int pitch)
{
	int tid = blockIdx.x;
	if(tid<N)
	{
		int temp = 0;
		int sum = 0;
		for(int i=0;i<D;i++)
		{
			temp = *((int*)((char*)train_data + tid * pitch) + i) - test_data[i];
			sum += temp * temp;
		}
		dis[tid] = sum;
	}
}
 
void print(int data[][D])
{
	cout<<"training data:"<<endl;
 	for(int i=0;i<N;i++)
	{
		for(int j=0;j<D;j++)
		{
			cout<<*(*(data+i)+j)<<" ";		
		}
		cout<<endl;
	}
}
 
void print(int *data,int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<data[i]<<" ";
	}
	cout<<endl;
}
 
int main()
{
	int h_train_data[N][D] , h_test_data[D] , distance[N];
 
	int *d_train_data , *d_test_data , *d_dis;
 
	size_t pitch_d;
	size_t pitch_h = D * sizeof(int) ;
 
	//allocate memory on GPU 
	cudaMallocPitch( &d_train_data , &pitch_d , D * sizeof(float) , N ); 
	cudaMalloc( (void**)&d_test_data ,  D*sizeof(int) );
	cudaMalloc( (void**)&d_dis , 		  N*sizeof(int) );
 
	//initialize training data
	srand( (unsigned)time(NULL) );
	for( int i=0;i<N;i++ )
	{
		for( int j=0;j<D;j++)
		{
			h_train_data[i][j] = rand()%MAX;
		}
	}
	print(h_train_data);
 
	//initialize testing data
	for( int j=0;j<D;j++ )
	{
	  	h_test_data[j] = rand() % MAX;
	}
	cout<<"testing data:"<<endl;
	print(h_test_data,D);
 
	//copy training and testing data from host to device
	cudaMemcpy2D( d_train_data , pitch_d , h_train_data , pitch_h , D * sizeof(int) , N , cudaMemcpyHostToDevice );
	cudaMemcpy( d_test_data,  h_test_data ,  D*sizeof(int), cudaMemcpyHostToDevice);
 
	//calculate the distance
	cal_dis<<<N,1>>>( d_train_data,d_test_data,d_dis,pitch_d );
 
	//copy distance data from device to host
	cudaMemcpy( distance , d_dis  , N*sizeof(int) , cudaMemcpyDeviceToHost);
 
	cout<<"distance:"<<endl;;
	print(distance , N);
 
	cudaFree(d_train_data);
	cudaFree(d_test_data);
	cudaFree(d_dis);
 
	return 0;
}               