CC=gcc
SRC=src
OUT=SpMV
WFLAGS=-Wall -Wextra -Wpedantic -g

all: $(OUT)

$(OUT):$(SRC)/*.c
	$(CC) $(WFLAGS) $^ -o $@

.PHONY clean:
	@rm $(OUT)
