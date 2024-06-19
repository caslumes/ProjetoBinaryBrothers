#ifndef FUNC_THREADS_H
#define FUNC_THREADS_H
#include <pthread.h>

typedef struct {
    unsigned int inicioThread;
    unsigned int fimThread;
    unsigned int numLinCol;
    int *matriz1, *matriz2, *matrizResultante;
} parametrosSomaMul;

typedef struct {
    int* matriz;
    char* nomeArqMatriz;
    unsigned int numLinCol;
} parametrosLerGravar;

int* alocaMatriz(unsigned int numLinCol);
parametrosLerGravar* alocaVetorParametrosThreadsLeitura(unsigned int numThreads);
pthread_t* alocaVetorIdsThreads(unsigned int numThreads);

void* threadLeMatriz(void* args);
void leMatriz(unsigned int numMatrizes, int** matrizes, char** nomesMatrizes, unsigned int numLinCol, unsigned int numThreads);

void* threadSoma(void* args);
void somaMatrizes(int* matriz1, int* matriz2, int* matrizResultante, unsigned int numLinCol, unsigned int numThreads);

void* threadGravaMatriz(void* args);
void gravarLerMatrizes(int* matrizGravar, int* matrizLer, char* nomeArqMatrizGravar, char*nomeArqMatrizLer, unsigned int numLinCol, unsigned int numThreads);

void* threadMultiplicacao(void* args);
void multiplicaMatrizes(int* matriz1, int* matriz2, int* matrizResultante, unsigned int numLinCol, unsigned int numThreads);

void* threadReducao(void* args);
int gravarReduzirMatriz(int* matriz, char* nomeArqMatriz, unsigned int numLinCol, unsigned int numThreads, double *tempoRed);

#endif