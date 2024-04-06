#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void armazenaMatriz(char *nomeArq, int *matriz, int numLinCol);
void somaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol);
void multiplicaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol);
int reduzMatriz(int *matriz, int numLinCol);
void gravaMatriz(char *nomeArq, int *matriz, int numLinCol);

int main(int argc, char *argv[]){
    int reducao;
    int numThreads = atoi(argv[1]);
    int numLinCol = atoi(argv[2]);
    int *matrizA, *matrizB, *matrizC, *matrizD, *matrizE;
    clock_t inicio, fim, inicioFuncao, fimFuncao;
    double tempoSoma, tempoMultiplicacao, tempoReducao, tempoTotal;
    inicio = clock();
    matrizA = malloc(numLinCol*numLinCol*(sizeof(int)));
    matrizB = malloc(numLinCol*numLinCol*(sizeof(int)));
    matrizC = malloc(numLinCol*numLinCol*(sizeof(int)));
    matrizD = malloc(numLinCol*numLinCol*(sizeof(int)));
    matrizE = malloc(numLinCol*numLinCol*(sizeof(int)));

    armazenaMatriz(argv[3], matrizA, numLinCol);
    armazenaMatriz(argv[4], matrizB, numLinCol);

    inicioFuncao = clock();
    somaMatriz(matrizA, matrizB, matrizD, numLinCol);
    fimFuncao = clock() - inicioFuncao;
    tempoSoma = ((double) fimFuncao)/CLOCKS_PER_SEC;

    gravaMatriz(argv[6], matrizD, numLinCol);

    armazenaMatriz(argv[5], matrizC, numLinCol);

    inicioFuncao = clock();
    multiplicaMatriz(matrizC, matrizD, matrizE, numLinCol);
    fimFuncao = clock() - inicioFuncao;
    tempoMultiplicacao = ((double) fimFuncao)/CLOCKS_PER_SEC;

    gravaMatriz(argv[7], matrizE, numLinCol);

    inicioFuncao = clock();
    reducao = reduzMatriz(matrizE, numLinCol);
    fimFuncao = clock() - inicioFuncao;
    tempoReducao = ((double) fimFuncao)/CLOCKS_PER_SEC;
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

void armazenaMatriz(char *nomeArq, int *matriz, int numLinCol){
    FILE *arq = fopen(nomeArq, "r");
    for(int i=0; i<numLinCol; i++){
        for(int j=0; j<numLinCol; j++)
            fscanf(arq, "%d ", &matriz[i*numLinCol+j]);
    }
    fclose(arq);
}

void somaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol){
    for(int i=0; i<numLinCol*numLinCol; i++){
        matrizResultante[i] = matriz1[i] + matriz2[i];
    }
}

void multiplicaMatriz(int *matriz1, int *matriz2, int *matrizResultante, int numLinCol){
    for(int i=0; i<numLinCol; i++){
        for(int j=0; j<numLinCol; j++){
            int somaMul = 0;
            for(int k=0; k<numLinCol; k++)
                somaMul += matriz1[i*numLinCol+k] * matriz2[j*numLinCol+k];
            matrizResultante[i*numLinCol+j] = somaMul;
        }
    }
}

int reduzMatriz(int *matriz, int numLinCol){
    int reducao = 0;
    for(int i=0; i<numLinCol*numLinCol; i++)
        reducao += matriz[i];
    
    return reducao;
}

void gravaMatriz(char *nomeArq, int *matriz, int numLinCol){
    FILE *arq = fopen(nomeArq, "w");
    for(int i=0; i<numLinCol; i++){
        for(int j=0; j<numLinCol; j++)
            fprintf(arq, "%d ", matriz[i*numLinCol+j]);
        fprintf(arq, "\n");
    }
    fclose(arq);
}
