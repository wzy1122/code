#include<stdio.h>
#include<cuda_runtime.h>
#include<cuda.h> 
__global__ void VecAdd(float *A, float *B, float *C)
{
     int i = threadIdx.x;
      C[i] = A[i] + B[i];
}
 
int main(void)
{
    int host_A[30], host_B[30], host_C[30];      //申请主机内存，并进行初始化
    
    for(int i=0; i<30;i++)
    {
        host_A[i] = i;
        host_B[i]=i+i-1;
    }

     cudaError_t err = cudaSuccess;        //定义cudaError,默认为cudaSuccess(0)
 
    float *A, *B, *C;
    err = cudaMalloc( (void**)&A, sizeof(float)*30);    //在GPU上申请内存
    err = cudaMalloc( (void**)&B, sizeof(float)*30);
    err = cudaMalloc( (void**)&C, sizeof(float)*30);
 
   cudaMemcpy(A, host_A, sizeof(host_A), cudaMemcpyHostToDevice);
   cudaMemcpy(B, host_B, sizeof(host_B), cudaMemcpyHostToDevice);
 
   VecAdd<<<1,30>>>(A, B, C);
  
   cudaMemcpy(&host_C, C, sizeof(host_C), cudaMemcpyDeviceToHost);
 
   for(int i=0; i<30; i++)
      printf("A[%d] + B[%d] = %d + %d = %d\n", i, i, host_A[i], host_B[i], host_C[i]);
 
   cudaFree(A);
   cudaFree(B);
   cudaFree(C);
  
  return 0;
}
