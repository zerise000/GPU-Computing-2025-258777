CC=gcc
NVCC=nvcc
WFLAGS=-Wall -Wextra -Wpedantic -g -O2
DLL=-lm

SRC=src
SRC_SERIAL=$(SRC)/serial
SRC_CUDA=$(SRC)/cuda
OUT_SERIAL=SpMV_serial
OUT_CUDA=SpMV_cuda

all: serial cuda

serial:$(SRC_SERIAL)/*.c
	$(CC) $(WFLAGS) $^ -o $(OUT_SERIAL) $(DLL)

cuda:$(SRC_CUDA)/*.cu
	$(NVCC) $^ -o $(OUT_CUDA) $(DLL)

.PHONY clean:
	@rm $(OUT)
