CC=gcc
SRC=src
OUT=SpMV
WFLAGS=-Wall -Wextra -Wpedantic -g
DLL=-lm

all: $(OUT)

$(OUT):$(SRC)/*.c
	$(CC) $(WFLAGS) $^ -o $@ $(DLL)

.PHONY clean:
	@rm $(OUT)
