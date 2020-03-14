/*
 *  dmv_gpu.cl -- Template for DMV GPU kernels
 *
 *  Copyright (C) 2017, Computing Systems Laboratory (CSLab)
 *  Copyright (C) 2017, Athena Elafrou
 */ 

#include "common.h"

/*
 *  Naive kernel
 */ 
__kernel void dmv_gpu_naive(__global const value_t *A,
			    __global const value_t *x,
			    __global value_t *y,
			    int N)
{
  /*
   * FILLME: fill the code for the naive kernel.
   */

  int tid = (int)get_global_id(0); // row index
  if (tid < N) {
    value_t sum = 0.0f;
    for (int k = 0; k < N; ++k)
      sum += A[tid * N + k] * x[k];
    y[tid] = sum;
  }
}

/*
 *  Coalesced memory acceses
 */
__kernel void dmv_gpu_coalesced(__global const value_t *a, __global const value_t *x,
				__global value_t *y, uint n)
{
    /*
     * FILLME: fill the code for the coalesced kernel.
     */ 
}

/*
 *  Use of shared memory
 */
__kernel void dmv_gpu_shmem(__global const value_t *a, __global const value_t *x,
			    __global value_t *y, uint n)
{
    /*
     * FILLME: fill the code for the shared memory kernel.
     */ 
}
