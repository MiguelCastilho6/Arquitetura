# Tarefa - Arquitetura de Computadores
    
## Comandos Makefile

### Compilar "matmult.c" e gerar executável:
```bash
make 
```

### Teste padrão
```bash
make run 
```
 
### Teste de benchmarks (gera arquivo de log .txt)
```bash
make bench
```

### Teste de benchmarks com perfilador perf
```bash
make bench_perf
```
        
### Teste de benchmarks com perfilador valgrind
```bash
make bench_valgrind
```

#### É possível customizar os "bench" com o uso de parâmetros SIZES e VERSIONS
#### Ex.: make bench SIZES="375 425" VERSIONS="2 4"

### Limpar arquivos de log
```bash
make clean-log
```

###  Limpar arquivo binário
```bash
make clean
```


## Resumo:
    A tarefa da disciplina se propõe a otimizar um algoritmo simples de multiplicação de matrizes. Para tal, primeiro foi programado o algoritmo denominado "controle", que utiliza da implementação clássica desse tipo de exercício. Em seguida, uma alteração neste código foi feita para melhorar seu desempenho. O que antes seria B[k * N + j] dentro de um loop *for* com incremento *k++*; se torna um B[k * N + j] dentro de um *for* que incrementa a varável j. Isso otimiza muito o acesso do processador aos dados, já que evita saltos de tamanho N em cada iteração. 
    Para a segunda versão da multiplicação, foi implementada a tecnologia sse4.2, presente no processador celeron utilizado. Essa tecnologia, pertencente ao paradigma SIMD (Single Instruction, Multiple Data), permite que o processador aplique uma mesma instrução a múltiplos dados simultaneamente em um único ciclo de clock, utilizando para tal registradores do tipo XMM com capacidade de 128 bits. A nível de código, foi usada a biblioteca **immintrin.h** que permite a alocação dos bits destes registradores através de variáveis de tipo mm128 e operações como mm_mul_ps().Assim, a multiplicação ocorre de forma muito mais rápida, executando mais instruções com menor número de ciclos de clock. 
    Por fim, a terceira otimização do algoritmo foi atingida através da implementação do _Loop Unrolling_. Que consiste em repetir o código *x* vezes dentro de um loop (Já que nosso algoritmo trabalha com diversos loops) para que o processador perca menos tempo fazendo saltos e incrementos das variáveis. 
    Vale ressaltar que as versões (4 no total: controle, inversão de laços, uso do sse e uso de loop unrolling) tem caráter incremental. A última versão tem a implementação de todas as anteriores junto com a mais nova adição.

## Resultados:   
