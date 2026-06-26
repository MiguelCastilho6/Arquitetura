N ?= 1000
SIZES ?= 200 500 750 1250 2200


all: matmult

matmult: matmult.c
	gcc matmult.c -o matmult -O2 -msse4.2 -fno-tree-vectorize -DN=$(N)

run: matmult
	./matmult

bench: 
	@for n in $(SIZES); do \
	echo "=================";\
	echo "N = $$n"; \
	echo "================="; \
	$(MAKE) --no-print-directory clean; \
	gcc matmult.c -o matmult -O2 -msse4.2 -fno-tree-vectorize -DN=$$n; \
	./matmult; \
	echo; \
	done

clean:
	@rm -f matmult
