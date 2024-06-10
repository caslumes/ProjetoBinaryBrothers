#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "func_threads.h"

int main(int argc, char *argv[]){
    int reducao=0;
    unsigned int numThreads = atoi(argv[1]);
    unsigned int numLinCol = atoi(argv[2]);
    int *matrizA, *matrizB, *matrizC, *matrizD, *matrizE;
    char *nomeArqMatrizA, *nomeArqMatrizB, *nomeArqMatrizC, *nomeArqMatrizD, *nomeArqMatrizE;
    clock_t inicio, fim, inicioFun, fimFun;
    double tempoSoma=0, tempoMultiplicacao=0, tempoReducao=0, tempoTotal=0;

    matrizA = alocaMatriz(numLinCol);
    matrizB = alocaMatriz(numLinCol);
    matrizC = alocaMatriz(numLinCol);
    matrizD = alocaMatriz(numLinCol);
    matrizE = alocaMatriz(numLinCol);

    nomeArqMatrizA = argv[3];
    nomeArqMatrizB = argv[4];
    nomeArqMatrizC = argv[5];
    nomeArqMatrizD = argv[6];
    nomeArqMatrizE = argv[7];
    

    int* matrizes[2] = {matrizA, matrizB};
    char* nomesMatrizes[2] = {nomeArqMatrizA, nomeArqMatrizB};

    inicio = clock();
    leMatriz(2, matrizes, nomesMatrizes, numLinCol, numThreads);

    inicioFun = clock();
    somaMatrizes(matrizA, matrizB, matrizD, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoSoma = ((double) fimFun)/CLOCKS_PER_SEC;

    gravarLerMatrizes(matrizD, matrizC, nomeArqMatrizD, nomeArqMatrizC, numLinCol, numThreads);

    inicioFun = clock();
    multiplicaMatrizes(matrizC, matrizD, matrizE, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoMultiplicacao = ((double) fimFun)/CLOCKS_PER_SEC;

    inicioFun = clock();
    reducao = gravarReduzirMatriz(matrizE, nomeArqMatrizE, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoReducao = ((double) fimFun)/CLOCKS_PER_SEC;

    fim = clock() - inicio;
    tempoTotal = ((double) fim)/CLOCKS_PER_SEC;

    printf("Redução: %d\n\n", reducao);
    printf("Tempo soma: %.3f segundos.\n\n", tempoSoma);
    printf("Tempo multiplicação: %.3f segundos.\n\n", tempoMultiplicacao);
    printf("Tempo redução: %.3f segundos.\n\n", tempoReducao);
    printf("Tempo total: %.3f segundos.", tempoTotal);

    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}