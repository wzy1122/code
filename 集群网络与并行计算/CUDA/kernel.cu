
#include "cuda_runtime.h"

#include <stdlib.h>
#include <stdio.h>

const int N = 6;
__global__ void MatMulAdd(float* A, float* B, float* C, float* D, int width) {
	int x = threadIdx.x;
	int y = threadIdx.y;
	D[y * width + x] = 0.0f;
	for (int k = 0; k < width; k++) {
		D[y * width + x] += A[y * width + k] + B[k * width + x];
	}
	D[y * width + x] += C[y * width + x];
}

__global__ void MatVecDistance(float* vec, float* mat, float* res, int N) {
	int x = threadIdx.x;
	float total = 0.0f;
	for (int k = 0; k < N; k++) {
		total += vec[k] * mat[x * N + k];
	}
	res[x] = sqrt(total);
}

void func2() {
	const int N = 6;
	float vec[N] = { 0 };
	float mat[N*N] = { 0 };
	float res[N] = { 0 };

	for (int i = 0; i < N; i++) {
		vec[i] = i;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mat[i * N + j] = i * N + j;
		}
	}
	
	int vecSize = sizeof(float) * N;
	int matSize = sizeof(float) * N * N;
	float* d_vec;
	float* d_mat;
	float* d_res;
	
	cudaMalloc(&d_vec, vecSize);
	cudaMalloc(&d_mat, matSize);
	cudaMalloc(&d_res, vecSize);

	cudaMemcpy(d_vec, vec, vecSize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_mat, mat, matSize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_res, res, vecSize, cudaMemcpyHostToDevice);

	MatVecDistance << <1, N >> > (d_vec, d_mat, d_res, N);

	cudaMemcpy(res, d_res, vecSize, cudaMemcpyDeviceToHost);

	cudaFree(d_vec);
	cudaFree(d_mat);
	cudaFree(d_res);

	for (int i = 0; i < N; i++) {
		printf("%f ", res[i]);
	}
} 

int main() {
	func2();
}
