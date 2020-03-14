/*
 *  common.h -- Basic definitions and declarations.
 *
 *  Copyright (C) 2017, Computing Systems Laboratory (CSLab)
 *  Copyright (C) 2017, Athena ELafrou
 */ 
#ifndef __COMMON_H
#define __COMMON_H

#if defined(__FLOAT_VALUES)
#   define MAKE_VALUE_CONSTANT(v)   v ## f
#   define VALUE_FORMAT "f"
#   define FABS    fabsf
typedef float   value_t;
#else
#   define MAKE_VALUE_CONSTANT(v)   v
#   define VALUE_FORMAT "lf"
#   define FABS    fabs
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
typedef double  value_t;
#endif

#endif  /* __COMMON_H */
