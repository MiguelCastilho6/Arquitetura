SIZES ?= 200 500 750 1250 2200
VALGRIND_SIZES ?= 100 300
VERSIONS ?= 1 2 3 4

all: matmult

matmult: matmult.c
	gcc matmult.c -o matmult -O2 -msse4.2 -fno-tree-vectorize -g

run: matmult
	./matmult

bench: matmult
	@> log_bench.txt
	@for n in $(SIZES); do \
		echo "=================" >> log_bench.txt;\
		echo "N = $$n" >> log_bench.txt; \
		echo "=================" >> log_bench.txt; \
		./matmult $$n 0 >> log_bench.txt; \
		echo >> log_bench.txt; \
	done
	cat log_bench.txt

bench_perf: matmult
	@> log_perf.txt
	@for n in $(SIZES); do \
	echo "==================" >> log_perf.txt; \
	echo "PERF: N = $$n" >> log_perf.txt; \
	echo "==================" >> log_perf.txt; \
	for v in $(VERSIONS); do \
		echo "Versão $$v" >> log_perf.txt; \
		perf stat -e cycles,instructions,cache-references,cache-misses ./matmult $$n $$v 2>> log_perf.txt; \
		echo >> log_perf.txt; \
		done \
	done
	cat log_perf.txt

bench_valgrind: matmult
	@> log_valgrind.txt
	@for n in $(VALGRIND_SIZES); do \
	echo "===================" >> log_valgrind.txt; \
	echo "VALGRIND: N = $$n" >> log_valgrind.txt; \
	echo "===================" >> log_valgrind.txt; \
	for v in $(VERSIONS); do \
		echo "Versão $$v" >> log_valgrind.txt; \
		valgrind --leak-check=full --track-origins=yes ./matmult $$n $$v 2>> log_valgrind.txt; \
		echo >> log_valgrind.txt; \
		done \
	done
	cat log_valgrind.txt

clean-log:
	@rm -f log_bench.txt
	@rm -f log_perf.txt
	@rm -f log_valgrind.txt

clean:
	@rm -f matmult
