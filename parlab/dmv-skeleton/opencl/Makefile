.PHONY: all clean query
.DEFAULT: all

DEBUG ?= 1
SERIAL ?= 1
OPENMP ?= 1
OPENCL ?= 1
CUDA_PREFIX = /usr/local/cuda
CC ?= gcc
RM = /bin/rm

MAKE_CPPFLAGS = -D__FLOAT_VALUES -DEPS=1e-2 -I../common -I/usr/local/cuda-7.0/targets/x86_64-linux/include -I/usr/local/cuda-7.5/targets/x86_64-linux/include
MAKE_CFLAGS = -c -Wall -std=gnu99 -fopenmp

ifeq ($(SERIAL), 1)
	MAKE_CPPFLAGS += -DSERIAL_KERNEL
else
    # we don't implement the serial version, so don't check
	MAKE_CPPFLAGS += -D_NOCHECK_
endif

ifeq ($(OPENMP), 1)
	MAKE_CPPFLAGS += -DOPENMP_KERNEL
endif

ifeq ($(OPENCL), 1)
	MAKE_CPPFLAGS += -DGPU_KERNEL
endif

ifeq ($(DEBUG), 1)
	MAKE_CPPFLAGS += -D_DEBUG_
	MAKE_CFLAGS += -g -O0
else
	MAKE_CPPFLAGS += -DNDEBUG
	MAKE_CFLAGS += -O3
endif

CPPFLAGS ?= $(MAKE_CPPFLAGS)
CFLAGS ?= $(MAKE_CFLAGS)
LDFLAGS ?= -L/various/common_tools/clBLAS/lib64 -lclBLAS -L$(CUDA_PREFIX)/lib64 -lOpenCL -fopenmp 

CPU_COMPILE = $(CC) $(CPPFLAGS) $(CFLAGS)

PROGRAMS = dmv_main
dep_OBJECTS = $(patsubst %.o, ../common/%.o, alloc.o error.o timer.o)
dmv_OBJECTS = dmv.o dmv_main.o

all: $(PROGRAMS)

dmv_main: $(dep_OBJECTS) $(dmv_OBJECTS)
	$(CC) -o $@ $(dep_OBJECTS) $(dmv_OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CPU_COMPILE) -o $@ $<

%.o: %.h
dmv_main.o: ../common/alloc.h dmv.h ../common/error.h ../common/timer.h

clean:
	$(RM) -f $(PROGRAMS) $(dmv_OBJECTS)

help:
	@echo 'Mini help for using this Makefile:'; \
	echo '\tBuild release version: make DEBUG=0 [default: 1]'; \
	echo '\tDisable building of the serial kernel: make SERIAL=0 [default: 1]'; \
	echo '\tDisable building of the OpenMP kernel: make OPENMP=0 [default: 1]'; \
	echo '\tDisable building of the GPU kernels: make GPU=0 [default: 1]'; \
	echo '\tDisable printing of GPU register info: make REGINFO=0 [default: 1]'; \
	echo 'You can also customly set CC, CPPFLAGS, CFLAGS and LDFLAGS as usual. Use the GPU_ prefix for gpu-specific flags.'; \
	echo 'NOTE: Disabling the serial kernel, also disables results checking. Consider avoiding this feature when debugging.'
