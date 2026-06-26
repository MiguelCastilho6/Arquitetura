N ?= 1000

all: matmult

matmult: matmult.c
	gcc matmult.c -o matmult -O2 -msse4.2 -fno-tree-vectorize -DN=$(N)

run: matmult
	./matmult

clean:
	rm -f matmult
