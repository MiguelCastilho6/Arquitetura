#include <stdio.h>
#include <immintrin.h> 
#include <time.h>
#include <stdlib.h>

#define N 1000

void inicializa_matriz(float *mat){
	for(int i = 0; i < N; i++){
		mat[i] = (float)(rand() % 10 + 1);
	}
}

// funcao para apagar lixo na memória alocada
void zera_matriz(float *mat){
	for(int i = 0; i < N * N; i++){
		mat[i] = 0.0f;
	}
}

// grupo de controle:
void mult_controle(float *A, float *B, float *C){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			for(int k = 0; k < N; k++){
				C[i * N + j] += A[i * N + k] * B[i * N + j];
			}
		}
	}
}

// primeira otimizacao : inversao de lacos (lacos k e j foram invertidos)
void mult_inversao(float *A, float *B, float *C){
	for(int i = 0; i < N; i++){ 
		for(int k = 0; k < N; k++){
			float tempA = A[i * N + k]; // evitar ler a mesma posicao
			for(int j = 0; j < N; j++){
				C[i * N + j] += tempA * B[k * N + j];
			}
		}
	}
}

// segunda otimização : utilizacao do SSE
void mult_sse(float *A, float *B, float *C){
	for(int i = 0; i< N; i++){
		for(int k = 0; k < N; k++){
			__m128 vet_a = _mm_set1_ps(A[i * N + k]); // multiplica o mesmo elemento de A quatro vezes no vetor

			for(int j = 0; j < N; j += 4){
				__m128 vet_b = _mm_loadu_ps(&B[k * N + j]); // carrega 4 elementos de b no vet_b
				__m128 vet_c = _mm_loadu_ps(&C[i * N + j]); // carrega 4 elementos de c em vet_c

				__m128 vet_mult = _mm_mul_ps(vet_a, vet_b); // multiplicacao e soma: C = (A * B) + C
				vet_c = _mm_add_ps(vet_c, vet_mult);

				_mm_storeu_ps(&C[i * N + j], vet_c); // guarda os 4 resultados na memoria
			}
		}
	}
}

// terceira otimizacao : loop unrolling

void mult_unroll(float *A, float *B, float *C){
	for(int i = 0; i< N; i++){
		for(int k = 0; k < N; k++){

			__m128 vet_a = _mm_set1_ps(A[i * N + k]); // multiplica o mesmo elemento de A quatro vezes no vetor

			// agora vai pular de 8 em 8
			for(int j = 0; j < N; j += 8){

				__m128 vet_b1 = _mm_loadu_ps(&B[k * N + j]); // carrega 4 elementos de b no vet_b1
				__m128 vet_c1 = _mm_loadu_ps(&C[i * N + j]); // carrega 4 elementos de c em vet_c1

				__m128 vet_mult1 = _mm_mul_ps(vet_a, vet_b1); // multiplicacao e soma: C = (A * B) + C
				vet_c1 = _mm_add_ps(vet_c1, vet_mult1);

				_mm_storeu_ps(&C[i * N + j], vet_c1); // guarda os 4 resultados na memoria


				__m128 vet_b2 = _mm_loadu_ps(&B[k * N + j + 4]); // carrega os 4 proximos elementos de b no vet_b2
				__m128 vet_c2 = _mm_loadu_ps(&C[i * N + j + 4]); // carrega os 4 proximos elementos de c em vet_c2

				__m128 vet_mult2 = _mm_mul_ps(vet_a, vet_b2); // multiplicacao e soma: C = (A * B) + C
				vet_c2 = _mm_add_ps(vet_c2, vet_mult2);

				_mm_storeu_ps(&C[i * N + j + 4], vet_c2); // guarda os 4 proximos resultados na memoria
			}
		}
	}
}


int main(){
	float *A = (float *)malloc(N * N * sizeof(float));
	float *B = (float *)malloc(N * N * sizeof(float));
	float *C = (float *)malloc(N * N * sizeof(float));

	inicializa_matriz(A);
	inicializa_matriz(B);

	clock_t inicio, fim;
	double tempo_controle;
	double tempo_inversao;
	double tempo_sse;
	double tempo_unroll;

	printf("Testes matriz %dx%d\n\n", N, N);

	// execucao controle
	zera_matriz(C);
	inicio = clock();
	mult_controle(A, B, C);
	fim = clock();
	tempo_controle = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
	printf("Tempo de controle: %f segundos\n", tempo_controle);

	// execucao inversao
	zera_matriz(C);
	inicio = clock();
	mult_inversao(A, B, C);
	fim = clock();
	tempo_inversao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
	printf("Tempo com inversao de matrizes: %f segundos (Speedup: %.2fx)\n", tempo_inversao, tempo_controle / tempo_inversao);

	// execucao sse
	zera_matriz(C);
	inicio = clock();
	mult_sse(A, B, C);
	fim = clock();
	tempo_sse = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
	printf("Tempo com inversao de matrizes e sse: %f segundos (Speedup: %.2fx)\n", tempo_sse, tempo_controle / tempo_sse);

	// execucao loop unrolling
	zera_matriz(C);
	inicio = clock();
	mult_unroll(A, B, C);
	fim = clock();
	tempo_unroll = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
	printf("Tempo com inversao de matrizes, sse e loop unroll: %f segundos (Speedup: %.2fx)\n", tempo_unroll, tempo_controle / tempo_unroll);


	free(A);
	free(B);
	free(C);

	return 0;
}
