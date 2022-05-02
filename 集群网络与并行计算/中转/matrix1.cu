#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cuda_runtime.h>

__global__ void matrixMulKernel(float *D, float *A, float *B, int width, int height){
  int tx = blockIdx.x * blockDim.x + threadIdx.x;
  int ty = blockIdx.y * blockDim.y + threadIdx.y;
  if(tx >= width || ty >= height)
    return;

  float sum1 = 0;
  for(int i=0; i<width; ++i){
    sum1 += A[ty * width + i] * B[i * width + tx];
  }
  D[ty * width + tx] = sum1;
}

__global__ void sumMatrixOnDevice(float *E, float *D, float *C, int width, int height)
{
	int tx = threadIdx.x + blockDim.x*blockIdx.x;
	int ty = threadIdx.y + blockDim.y*blockIdx.y;
	unsigned int idx = iy * width + ix;
	//unsigned int t_n = gridDim.x*blockDim.x + gridDim.y*blockDim.y;
	if (tx < width && ty < height)
	{
		E[idx] = D[idx] + C[idx];
	
	}
}

void constantInit(float *data, int size, float val){
    for (int i = 0; i < size; ++i){
        data[i] = val;
    }
}



void matrixMul(){
  unsigned int width = 128;
  unsigned int height = 128;
  unsigned int size = width * height * sizeof(float);
  float *h_A = (float*)malloc(size);
  float *h_B = (float*)malloc(size);
  float *h_C = (float*)malloc(size);
  float *h_D = (float*)malloc(size);
  float *h_E = (float*)malloc(size);

  // Initialize host memory
  const float valB = 0.01f;
  constantInit(h_A, width*height, 1.0f);
  constantInit(h_B, width*height, valB);
  constantInit(h_c, width*height, valB);


  float *d_A, *d_B, *d_C,*d_D ,*d_E;
  cudaMalloc((void**)&d_A, size);
  cudaMalloc((void**)&d_B, size);
  cudaMalloc((void**)&d_C, size);
  cudaMalloc((void**)&d_D, size);
  cudaMalloc((void**)&d_E, size);

  //copy host memory to device
  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_C, h_C, size, cudaMemcpyHostToDevice);

  //config dims
  dim3 block(16, 16);
  dim3 grid(width / block.x, height / block.y);

  // Excute the kernel
  matrixMulKernel<<<grid, block>>>(d_D, d_A, d_B,width, height);
  sumMatrixOnDevice<<<grid, block>>>(d_E, d_D, d_C,width, height);
  // Copy the memory from device to host
  cudaMemcpy(h_D, d_D, size, cudaMemcpyDeviceToHost);
  cudaMemcpy(h_E, d_E, size, cudaMemcpyDeviceToHost);

  printf("Checking computed result for correctness: ");
  bool correct = true;
  // test relative error by the formula
  //     |<x, y>_cpu - <x,y>_gpu|/<|x|, |y|>  < eps
  double eps = 1.e-6 ; // machine zero

  for (int i = 0; i < width*height; i++){
      double abs_err = fabs(h_E[i] - (width * valB));
      double dot_length = width;
      double abs_val = fabs(h_E[i]);
      double rel_err = abs_err/abs_val/dot_length ;
      if (rel_err > eps)
      {
          printf("Error! Matrix[%05d]=%.8f, ref=%.8f error term is > %E\n", i, h_E[i], (float)(width*height), eps);
          correct = false;
      }
  }
  printf("%s\n", correct ? "Result = PASS" : "Result = FAIL");


  // Free
  free(h_A);
  free(h_B);
  free(h_C);
  free(h_D);
  free(h_E);
  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);
  cudaFree(d_D);
  cudaFree(d_E);
  

}

int main(){
  matrixMul();
}
