all: matmult

matmult: matmult.c
	gcc matmult.c -o matmult -O3 -msse4.2 -fno-tree-vectorize -fno-unroll-loops

run: matmult
	./matmult

clean:
	rm -f matmult
