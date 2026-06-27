            Tarefa de otimização de algortimo de multiplicação de matrizes
    
        Compilar "matmult.c" e gerar executável:
    ```bash
    make 
    ```

        Teste padrão
    ```bash
    make run 
    ```
 
        Teste de benchmarks (gera arquivo de log .txt)
    ```bash
    make bench
    ```

        Teste de benchmarks com perfilador perf
    ```bash
    make bench_perf
    ```
        
        Teste de benchmarks com perfilador valgrind
    ```bash
    make bench_valgrind
    ```

        É possível customizar os "bench" com o uso de parâmetros SIZES e VERSIONS
        Ex.: make bench SIZES="375 425" VERSIONS="2 4"

        Limpar arquivos de log
    ```bash
    make clean-log
    ```

        Limpar arquivo binário
    ```bash
    make clean
    ```
