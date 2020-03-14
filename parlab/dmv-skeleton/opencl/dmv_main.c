/*
 *  dmv_main.c -- DMV front-end program.
 *
 *  Copyright (C) 2017, Computing Systems Laboratory (CSLab)
 *  Copyright (C) 2017, Athena Elafrou
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CL/cl.h>
#include "alloc.h"
#include "dmv.h"
#include "error.h"
#include "timer.h"

#ifndef VALUES_MAX
#   define VALUES_MAX MAKE_VALUE_CONSTANT(1.)
#endif

#ifndef EPS
#   define EPS MAKE_VALUE_CONSTANT(1.e-6)
#endif

#ifndef NR_ITER
#   define NR_ITER 100
#endif

// Name of the file with the source code for the GPU kernels
static const char *c_source_file = "./dmv_gpu.cl";

static void print_usage()
{
    printf("Usage: [GPU_KERNEL=<kernel_no>] [GPU_BLOCK_SIZE=<size>] "
           "%s <matrix size>\n", program_name);
    printf("GPU_KERNEL defaults to 0\n");
    printf("GPU_BLOCK_SIZE defaults to 256\n");
    printf("Available kernels [id:name]:\n");
    size_t i;
    for (i = 0; i < GPU_KERNEL_END; ++i) {
        printf("\t%zd:%s\n", i, gpu_kernels[i].name);
    }
}

// Round Up Division function
static size_t round_up(int group_size, int global_size) 
{
  int r = global_size % group_size;
  if (r == 0) {
    return global_size;
  } else {
    return global_size + group_size - r;
  }
}

static char *load_prog_from_src(const char *filename, size_t *prog_size)
{
  FILE *file_handle = NULL;
  size_t program_size;
  
  file_handle = fopen(filename, "rb");
  if (file_handle == NULL) {
    perror("Couldn't open the program file");
    exit(1);
  }
  
  fseek(file_handle, 0, SEEK_END); 
  program_size = ftell(file_handle);
  rewind(file_handle);
  
  char *program_buffer = (char *)malloc((program_size + 1) * sizeof(char));
  size_t result;
  if ((result = fread(program_buffer, sizeof(char), program_size, file_handle)) != program_size*sizeof(char)) {
    perror("Couldn't read the program file");
    fclose(file_handle);
    free(program_buffer);
    return NULL;
  }

  fclose(file_handle);
  *prog_size = program_size;
  program_buffer[program_size] = '\0';
  
  return program_buffer;
}

static void check_result(const value_t *test, const value_t *orig, size_t n)
{
    printf("Checking ... ");
    size_t  i_fail = vec_equals(test, orig, n, EPS);
    if (!i_fail) {
        printf("PASSED\n");
    } else {
        printf("FAILED (index: %ld)\n", i_fail - 1);
        printf("%" VALUE_FORMAT " != " "%" VALUE_FORMAT "\n",
               test[i_fail-1], orig[i_fail-1]);
    }
}

static void report_results(xtimer_t *timer, size_t n)
{
    double  elapsed_time = timer_elapsed_time(timer);
    size_t  flops        = 2*n*n*NR_ITER;

    printf("Elapsed time: %lf s\n", elapsed_time);
    printf("Performance:  %lf Gflop/s\n", flops*1.e-9 / elapsed_time);
}

int main(int argc, char **argv)
{
    set_program_name(argv[0]);
    if (argc < 2) {
        warning(0, "too few arguments");
        print_usage();
        exit(EXIT_FAILURE);
    }

    size_t n = atoi(argv[1]);
    if (!n)
        error(0, "invalid argument: %s", argv[1]);

    /* Read block size and kernel to launch from the environment */
    const char *env_gpu_kernel = getenv("GPU_KERNEL");
    const char *env_gpu_block_size = getenv("GPU_BLOCK_SIZE");
    int kernel_id = (env_gpu_kernel) ? atoi(env_gpu_kernel) : GPU_NAIVE;
    int block_size = (env_gpu_block_size) ? atoi(env_gpu_block_size) : 256;
    size_t orig_n = n;  // original matrix size

    /*
     *  FILLME: you can optionally adjust appropriately (increase
     *          only) the matrix size here if that helps you with your
     *          kernel code, e.g., to avoid divergent warps.
     */ 

    printf("Matrix size: %zd\n", orig_n);
    printf("Adjusted matrix size: %zd\n", n);

    /*
     * Allocate the structures on the host.
     * 
     * Initialization to zero is crucial if you adjusted the matrix
     * size.
     */
    value_t **A = (value_t **) calloc_2d(n, n, sizeof(**A));
    if (!A)
        error(1, "alloc_2d failed");

    value_t *x = (value_t *) calloc(n, sizeof(*x));
    if (!x)
        error(1, "malloc failed");

    value_t *y_serial = (value_t *) calloc(n, sizeof(*y_serial));
    if (!y_serial)
        error(1, "malloc failed");
    
    value_t *y = (value_t *) calloc(n, sizeof(*y));
    if (!y)
        error(1, "malloc failed");

    /* Initialize */
    srand48(0);
    mat_init_rand(A, orig_n, VALUES_MAX);
    vec_init_rand(x, orig_n, VALUES_MAX);
    vec_init(y_serial, orig_n, MAKE_VALUE_CONSTANT(0.0));
    vec_init(y, orig_n, MAKE_VALUE_CONSTANT(0.0));

    /* Setup timers */
    xtimer_t timer;

    /* Compute serial */
#ifdef SERIAL_KERNEL
    printf(">>>> Begin of record <<<<\n");
    printf("Serial version:\n");
    timer_clear(&timer);
    timer_start(&timer);
    for (size_t i = 0; i < NR_ITER; ++i)
        dmv_serial(A, x, y_serial, orig_n);
    timer_stop(&timer);
    report_results(&timer, orig_n);
    printf(">>>> End of record <<<<\n");
#endif  // SERIAL_KERNEL

#ifdef OPENMP_KERNEL
    /* Compute OpenMP */
    printf(">>>> Begin of record <<<<\n");
    printf("OpenMP version:\n");
    timer_clear(&timer);
    timer_start(&timer);
    for (size_t i = 0; i < NR_ITER; ++i)
        dmv_omp(A, x, y, orig_n);
    timer_stop(&timer);
#ifndef _NOCHECK_
    check_result(y, y_serial, orig_n);
#endif
    report_results(&timer, orig_n);
    printf(">>>> End of record <<<<\n");
#endif  // OPENMP_KERNEL

#ifdef GPU_KERNEL
    printf(">>>> Begin of record <<<<\n");

    /* Identify a platform */
    cl_platform_id platform;
    cl_int ret = clGetPlatformIDs(1, &platform, NULL);
    if (ret != CL_SUCCESS) {
      perror("Couldn't identify a platform");
      exit(1);
    }

    /* Access a GPU device */
    cl_device_id device;
    ret = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    if (ret == CL_DEVICE_NOT_FOUND) {
      perror("Couldn't access any GPU devices");
      exit(1);
    }

    /* Create device context */
    cl_context ctx = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a context");
      exit(1);   
    }

    /* Create device command queue */
    cl_command_queue cmd_queue = clCreateCommandQueue(ctx, device, 0, &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a command queue");
      exit(1);   
    }

    /* Perform GPU allocations */
    cl_mem gpu_A = clCreateBuffer(ctx,
				  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				  n*n*sizeof(value_t), A[0], &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a buffer");
      exit(1);   
    }

    cl_mem gpu_x = clCreateBuffer(ctx,
				  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
				  n*sizeof(value_t), x, &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a buffer");
      exit(1);   
    }

    vec_init(y, n, MAKE_VALUE_CONSTANT(0.0));
    cl_mem gpu_y = clCreateBuffer(ctx,
				  CL_MEM_WRITE_ONLY,
				  n*sizeof(value_t), NULL, &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a buffer");
      exit(1);   
    }

    /* Read the OpenCL kernel from source file */
    size_t program_size;
    char *cl_source = load_prog_from_src(c_source_file, &program_size);

    /* Create the program */
    cl_program program = clCreateProgramWithSource(ctx, 1, (const char **)&cl_source, &program_size, &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create the program");
      exit(1);
    }
    free(cl_source);

    /* Build the program */
    ret = clBuildProgram(program, 0, NULL, "-D__FLOAT_VALUES", NULL, NULL);
    if (ret != CL_SUCCESS) {
      size_t log_size = 0;
      clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
			    0, NULL, &log_size);
      char *program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';
      clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
			    log_size + 1, program_log, NULL);
      printf("%s\n", program_log);
      free(program_log);
      exit(1);
    }

    /* Create kernel */
    if (kernel_id >= GPU_KERNEL_END)
        error(0, "the requested kernel does not exist");

    printf("GPU kernel version: %s\n", gpu_kernels[kernel_id].name);
    cl_kernel kernel = clCreateKernel(program, "dmv_gpu_naive", &ret);
    if (ret != CL_SUCCESS) {
      perror("Couldn't create a kernel");
      exit(1);
    }

    /* Set kernel arguments */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &gpu_A);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &gpu_x);
    ret |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &gpu_y);
    ret |= clSetKernelArg(kernel, 3, sizeof(int), &n);
    if (ret < 0) {
      perror("Couldn't create a kernel argument");
      exit(1);
    }

    /* Set local and global work group sizes */
    size_t local_work_size[1] = {1};   // FILLME: set up the local work size dimensions
    size_t global_work_size[1] = {1};  // FILLME: set up the global work size dimensions
    local_work_size[0] = block_size;
    global_work_size[0] = round_up((int)local_work_size[0], n);

    /* Execute and time the kernel */
    timer_clear(&timer);
    timer_start(&timer);
    for (size_t i = 0; i < NR_ITER; ++i) {
      ret = clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
#ifdef _DEBUG_
      if (ret < 0) {
	perror("Couldn't enqueue the kernel");
	exit(1);
      }
#endif
      clFinish(cmd_queue);
    }
    timer_stop(&timer);

    /* Copy result back to host and check */
    ret = clEnqueueReadBuffer(cmd_queue, gpu_y, CL_TRUE, 0, n*sizeof(value_t), y, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
      perror("Couldn't read the buffer");
      exit(1);
    }

#ifndef _NOCHECK_
    check_result(y, y_serial, orig_n);
#endif
    report_results(&timer, orig_n);
    printf(">>>> End of record <<<<\n");
#endif  // GPU_KERNEL

    /* Free resources on host */
    free_2d((void **) A);
    free(x);
    free(y);
    free(y_serial);

#ifdef GPU_KERNEL
    /* Free resources on GPU */
    clReleaseContext(ctx);
    clReleaseMemObject(gpu_A);
    clReleaseMemObject(gpu_x);
    clReleaseMemObject(gpu_y);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(cmd_queue);
    clReleaseProgram(program);
#endif  // GPU_KERNEL 

    return EXIT_SUCCESS;
}
