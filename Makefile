SIZES ?= 200 500 750 1250 2200
VALGRIND_SIZES ?= 100 300
VERSIONS ?= 1 2 3 4

all: matmult

matmult: matmult.c
	gcc matmult.c -o matmult -O2 -msse4.2 -fno-tree-vectorize -g

run: matmult
	./matmult

bench: matmult
	@for n in $(SIZES); do \
	echo "=================";\
	echo "N = $$n"; \
	echo "================="; \
	./matmult $$n 0; \
	echo; \
	done

bench_perf: matmult
	@for n in $(SIZES); do \
	echo "=================="; \
	echo "PERF: N = $$n"; \
	echo "=================="; \
	for v in $(VERSIONS); do \
		echo "Versão $$v"; \
		perf stat -e cycles,instructions,cache-references,cache-misses ./matmult $$n $$v; \
		echo; \
		done \
	done

bench_valgrind: matmult
	@for n in $(VALGRIND_SIZES); do \
	echo "==================="; \
	echo "VALGRIND: N = $$n"; \
	echo "==================="; \
	for v in $(VERSIONS); do \
		echo "Versão $$v"; \
		valgrind --leak-check=full --track-origins=yes ./matmult $$n $$v; \
		echo; \
		done \
	done

clean:
	@rm -f matmult
