/*
 *  dmv.h -- Declarations and definitions related to the DMV
 *           multiplication kernels.
 *
 *  Copyright (C) 2010-2012, Computing Systems Laboratory (CSLab)
 *  Copyright (C) 2010-2012, Vasileios Karakasis
 */ 
#ifndef __DMV_H
#define __DMV_H

#include <stddef.h>
#include <CL/cl.h>
#include "common.h"

void mat_transpose(value_t **a, size_t n);
void mat_init_rand(value_t **a, size_t n, value_t max);

void vec_init(value_t *v, size_t n, value_t val);
void vec_init_rand(value_t *v, size_t n, value_t max);
int vec_equals(const value_t *v1, const value_t *v2, size_t n, value_t eps);
void vec_print(const value_t *v, size_t n);

void dmv_serial(const value_t *const *a, const value_t *x, value_t *y,
                size_t n);
void dmv_omp(const value_t *const *a, const value_t *x, value_t *y, size_t n);

#define __MAKE_KERNEL_NAME(name) "dmv_gpu" # name
#define MAKE_KERNEL_NAME(name) __MAKE_KERNEL_NAME(name)

typedef struct {
  const char *name;
} gpu_kernel_t;

enum {
  GPU_NAIVE = 0,
  GPU_COALESCED,
  GPU_SHMEM,
  GPU_KERNEL_END
};

static gpu_kernel_t gpu_kernels[] = {
  {
    .name = MAKE_KERNEL_NAME(_naive),
  },
  
  {
    .name = "coalesced",
  },
  
  {
    .name = "shmem",
  }
};

#endif  /* __DMV_H */
