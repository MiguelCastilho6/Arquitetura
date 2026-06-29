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

As otimizações foram todas bem sucedidas, porém geraram resultados dignos de observações importantes. Na multiplicação de matrizes 200x200, o salto do speedup da versão 2 para a versão 3 superou o mesmo avanço feito nas matrizes quadradas de tamanho 500, 750 e 1250, um resultado contraintuitivo. Ele pode ser explicado, no entanto, quando analisamos o tamanho das memórias cache do processador utilizado para rodar o programa e do tamanho necessário para guardar as matrizes. No caso da matriz 200, ele pode ser armazenada por inteira na cache L2, que possui 512 KiB de capacidade. O PERF mostrou uma diminuição de pouco menos que 6 vezes no número de ciclos necessários, o que deve ser decorrência da falta de necessidade de buscar dados através da hierarquia a todo momento. Assim, esse tamanho de matriz é um exemplo de programa compute-bound.

Já os tamanhos 500, 750 e 1250 aparentaram comportamentos bem similares. Em todos os casos o tamanho da matriz é maior que o das caches disponíveis, o que força a utilização da mamória principal e, por consequência, torna a execução memory-bound. A razão de cache misses e o speedup que ocorre entre as versões 1 e 4 é gradativamente menor conforme o tamanho aumenta. Isso pode ser explicado pois a versão de controle ainda é minimamente funcional para esses tamanhos e essas métricas são prejudicadas por conseguinte. 

Para o tamanho 2200, último establecido no benchmark, podem ser observadas mudanças categóricas. O tempo de execução da versão controle sobe vertiginosamente (de 6 para 80 segundos), paralelamente o speedup cresce para 16x (antes era 6). O que esses dados mais mostram é o colapso total da versão de controle ao lidar com matrizes a partir deste tamanho (neste processador), que passa a sofrer severamente com fenômenos como TLB Thrashing e afins. Já as otimizações se provaram absolutamente eficazes em diminuir esses impactos negativos do aumento da matriz, principalmente no que diz respeito a cache references. 

O Valgrind não mostrou resultado nenhum digno de comentário, mas foi importante para nos indicar o bom funcionamento da alocação de memória. 

Estes resultados e suas conclusões, no entanto, falham em contemplar a complexidade toda do problema proposto. Isso se dá pois parâmetros importantes para a análise são impossíveis ou absolutamente desafiadores de se obter informações. Entre eles temos um que talvez seja o mais importante, que seria o comportamento do prefetcher implementado nesta arquitetura x86 (3dnowprefetcher). Mesmo assim, creio que as conclusões são majoritariamente precisas.
