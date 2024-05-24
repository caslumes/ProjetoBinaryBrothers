#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "func_threads.h"

void multiplicaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol);
int reduzMatriz(int *matriz, int numLinCol);

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
    printf("Leitura matrizA e matrizB feita.\n");

    inicioFun = clock();
    somaMatrizes(matrizA, matrizB, matrizD, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoSoma = ((double) fimFun)/CLOCKS_PER_SEC;
    printf("Soma matrizA e matrizB feita.\n");

    gravarLerMatrizes(matrizD, matrizC, nomeArqMatrizD, nomeArqMatrizC, numLinCol, numThreads);
    printf("Gravação matrizD e leitura matrizC feita.\n");

    inicioFun = clock();
    multiplicaMatrizes(matrizC, matrizD, matrizE, numLinCol, numThreads);
    printf("Multiplicação matrizC e matrizD feita.\n");
    fimFun = clock() - inicioFun;
    tempoMultiplicacao = ((double) fimFun)/CLOCKS_PER_SEC;

    inicioFun = clock();
    reducao = gravarReduzirMatriz(matrizE, nomeArqMatrizE, numLinCol, numThreads);
    printf("Gravação e redução matrizE feita.\n");
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

void multiplicaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol){
    for(register int i=0; i<numLinCol; i++){
        for(register int j=0; j<numLinCol; j++){
            int somaMul = 0;
            for(int k=0; k<numLinCol; k++)
                somaMul += matriz1[i*numLinCol+k] * matriz2[j*numLinCol+k];
            matrizResultante[i*numLinCol+j] = somaMul;
        }
    }
}

int reduzMatriz(int *matriz, int numLinCol){
    int reducao = 0;
    for(register int i=0; i<numLinCol*numLinCol; i++)
        reducao += matriz[i];
    
    return reducao;
}
