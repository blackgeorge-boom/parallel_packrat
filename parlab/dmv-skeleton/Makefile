SUBDIRS = common opencl cuda

.PHONY: all clean query $(SUBDIRS)
.NOTPARALLEL: $(SUBDIRS)
.DEFAULT: all

all: $(SUBDIRS)

DEBUG ?= 1

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean:	$(SUBDIRS)
	$(RM) -f $(PROGRAMS) $(dmv_OBJECTS)

query:
	@echo '\n' | $(CUDA_PREFIX)/samples/C/bin/linux/release/deviceQuery

help:
	@echo 'Mini help for using this Makefile:'; \
	echo '\tBuild release version: make DEBUG=0 [default: 1]'; \
	echo '\tDisable building of the serial kernel: make SERIAL=0 [default: 1]'; \
	echo '\tDisable building of the OpenMP kernel: make OPENMP=0 [default: 1]'; \
	echo '\tDisable building of the GPU kernels: make GPU=0 [default: 1]'; \
	echo '\tDisable printing of GPU register info: make REGINFO=0 [default: 1]'; \
	echo 'You can also customly set CC, CPPFLAGS, CFLAGS and LDFLAGS as usual. Use the GPU_ prefix for gpu-specific flags.'; \
	echo 'NOTE: Disabling the serial kernel, also disables results checking. Consider avoiding this feature when debugging.'
