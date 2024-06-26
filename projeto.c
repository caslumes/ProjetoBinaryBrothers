#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "func_threads.h"

int main(int argc, char *argv[]){
    // verificacao se ha todos os argumentos necessarios
    if(argc < 8){
        fprintf(stderr, "Numero de argumentos invalido.");
        exit(EXIT_FAILURE);
    }

    int reducao=0;
    unsigned int numThreads = atoi(argv[1]);
    unsigned int numLinCol = atoi(argv[2]);
    int *matrizA, *matrizB, *matrizC, *matrizD, *matrizE;
    char *nomeArqMatrizA, *nomeArqMatrizB, *nomeArqMatrizC, *nomeArqMatrizD, *nomeArqMatrizE;
    clock_t inicio, fim, inicioFun, fimFun;
    double tempoSoma=0, tempoMultiplicacao=0, tempoReducao=0, tempoTotal=0;

    // alocacao dinamica de todas as matrizes
    matrizA = alocaMatriz(numLinCol);
    matrizB = alocaMatriz(numLinCol);
    matrizC = alocaMatriz(numLinCol);
    matrizD = alocaMatriz(numLinCol);
    matrizE = alocaMatriz(numLinCol);

    // atribuicao dos nomes dos arquivos das matrizes de acordo com os argumentos
    nomeArqMatrizA = argv[3];
    nomeArqMatrizB = argv[4];
    nomeArqMatrizC = argv[5];
    nomeArqMatrizD = argv[6];
    nomeArqMatrizE = argv[7];

    // inicio do clock de tempo total e leitura das matrizes A e B
    inicio = clock();
    leMatriz(matrizA, matrizB, nomeArqMatrizA, nomeArqMatrizB, numLinCol, numThreads);

    // soma das matrizes A e B e armazenamento na matriz D e calculo do tempo de soma
    inicioFun = clock();
    somaMatrizes(matrizA, matrizB, matrizD, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoSoma = ((double) fimFun)/CLOCKS_PER_SEC;

    // gravacao da matriz D no arquivo e leitura do arquivo da matriz C
    gravarLerMatrizes(matrizD, matrizC, nomeArqMatrizD, nomeArqMatrizC, numLinCol, numThreads);

    // multiplicacao das matriz C e D e armazenamento na matriz E e calculo do tempo de multiplicacao
    inicioFun = clock();
    multiplicaMatrizes(matrizC, matrizD, matrizE, numLinCol, numThreads);
    fimFun = clock() - inicioFun;
    tempoMultiplicacao = ((double) fimFun)/CLOCKS_PER_SEC;

    // atribuicao do valor da reducao com a funcao que grava e reduz matrizes e resgate do tempo de reducao
    reducao = gravarReduzirMatriz(matrizE, nomeArqMatrizE, numLinCol, numThreads, &tempoReducao);

    // final do clock de tempo total e calculo do tempo total
    fim = clock() - inicio;
    tempoTotal = ((double) fim)/CLOCKS_PER_SEC;

    // apresentacao dos resultados
    printf("Redução: %d\n\n", reducao);
    printf("Tempo soma: %.3f segundos.\n\n", tempoSoma);
    printf("Tempo multiplicação: %.3f segundos.\n\n", tempoMultiplicacao);
    printf("Tempo redução: %.3f segundos.\n\n", tempoReducao);
    printf("Tempo total: %.3f segundos.\n", tempoTotal);

    // liberacao da memoria alocada para as matrizes
    free(matrizA);
    free(matrizB);
    free(matrizC);
    free(matrizD);
    free(matrizE);

    return 0;
}
