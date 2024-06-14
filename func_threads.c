#include "func_threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *alocaMatriz(unsigned int numLinCol){
    int* matriz;

    if(numLinCol <= 0){
        return NULL;
    }

    if((matriz = (int *) malloc(numLinCol*numLinCol*sizeof(int))) == NULL){
        fprintf(stderr, "Problemas na alocacao da matriz.\n");
        exit(EXIT_FAILURE);
    };

    return matriz;
}

parametrosLerGravar *alocaVetorParametrosThreadsLeitura(unsigned int numThreads){
    parametrosLerGravar* vetor;

    if(numThreads <= 0){
        return NULL;
    }

    if((vetor = (parametrosLerGravar *) malloc(numThreads*sizeof(parametrosLerGravar))) == NULL){
        fprintf(stderr, "Problemas na alocacao do vetor de parametros de leitura.\n");
        exit(EXIT_FAILURE);
    };

    return vetor;
}

parametrosSomaMul *alocaVetorParametrosThreadsSomaMul(unsigned int numThreads){
    parametrosSomaMul* vetor;

    if(numThreads <= 0){
        return NULL;
    }

    if((vetor = (parametrosSomaMul *) malloc(numThreads*sizeof(parametrosSomaMul))) == NULL){
        fprintf(stderr, "Problemas na alocacao do vetor de parametros de soma\\multiplicacao.\n");
        exit(EXIT_FAILURE);
    };

    return vetor;
}

pthread_t *alocaVetorIdsThreads(unsigned int numThreads){
    pthread_t* vetor;

    if(numThreads <= 0){
        return NULL;
    }

    if((vetor = (pthread_t *) malloc(numThreads*sizeof(pthread_t))) == NULL){
        fprintf(stderr, "Problemas na alocacao do vetor de parametros de leitura.\n");
        exit(EXIT_FAILURE);
    };

    return vetor;
}

void* threadLeMatriz(void* args){
    int* matriz = ((parametrosLerGravar*) args)->matriz;
    char* nomeArq = ((parametrosLerGravar*) args)->nomeArqMatriz;
    register unsigned int numLinCol = ((parametrosLerGravar*) args)->numLinCol;

    FILE *arq = fopen(nomeArq, "r");
    if(arq==NULL){
        fprintf(stderr, "Problemas ao abrir o arquivo %s.\n", nomeArq);
    }

    for(register int i=0; i<numLinCol; i++){
        for(register int j=0; j<numLinCol; j++){
            fscanf(arq, "%d ", &matriz[i*numLinCol+j]);
        }
    }

    fclose(arq);

    return NULL;
}

void leMatriz(unsigned int numMatrizes, int** matrizes, char** nomesMatrizes, unsigned int numLinCol, unsigned int numThreads){
    parametrosLerGravar* parametros;

    if(numThreads == 1){
        parametros = alocaVetorParametrosThreadsLeitura(1);
        parametros[0].matriz = matrizes[0];
        parametros[0].nomeArqMatriz = nomesMatrizes[0];
        parametros[0].numLinCol = numLinCol;

        threadLeMatriz((void*) &parametros[0]);

        if(numMatrizes == 2){
            parametros[0].matriz = matrizes[1];
            parametros[0].nomeArqMatriz = nomesMatrizes[1];
            parametros[0].numLinCol = numLinCol;

            threadLeMatriz((void*) &parametros[0]);
        }
        
    }else{
        int err;
        pthread_t* idsThreads;

        parametros = alocaVetorParametrosThreadsLeitura(2);
        idsThreads = alocaVetorIdsThreads(2);

        for(unsigned int i=0; i<2; i++){
            parametros[i].matriz = matrizes[i];
            parametros[i].nomeArqMatriz = nomesMatrizes[i];
            parametros[i].numLinCol = numLinCol;

            err = pthread_create(&idsThreads[i], NULL, threadLeMatriz, (void*) &parametros[i]); 
            if(err != 0){
                fprintf(stderr, "Erro na criacao do thread de leitura.\n");
                exit(EXIT_FAILURE);
            }
        }

        for(unsigned int i=0; i<2; i++){
            err = pthread_join(idsThreads[i], NULL);
            if(err != 0){
                fprintf(stderr, "Erro na juncao do thread de leitura.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void* threadSoma(void* args){
    register unsigned int i, inicio, fim;
    int *matriz1, *matriz2, *matrizResultante;

    inicio = ((parametrosSomaMul*) args)->inicioThread;
    fim = ((parametrosSomaMul*) args)->fimThread;
    matriz1 = ((parametrosSomaMul*) args)->matriz1;
    matriz2 = ((parametrosSomaMul*) args)->matriz2;
    matrizResultante = ((parametrosSomaMul*) args)->matrizResultante;

    for(i = inicio; i <= fim; i++){
        matrizResultante[i] = matriz1[i] + matriz2[i];
    }

    return NULL;
}

void somaMatrizes(int* matriz1, int* matriz2, int* matrizResultante, unsigned int numLinCol, unsigned int numThreads){
    parametrosSomaMul* parametros;
    pthread_t* idsThreads;
    register unsigned int i, numElementos;
    int err;

    parametros = alocaVetorParametrosThreadsSomaMul(numThreads);
    idsThreads = alocaVetorIdsThreads(numThreads);

    if((numLinCol*numLinCol) % numThreads != 0){
        fprintf(stderr, "O numero de elementos da matriz nao eh divisivel pelo numero de threads.\n");
        exit(EXIT_FAILURE);
    }

    numElementos = (numLinCol*numLinCol)/numThreads;

    if(numThreads == 1){
        parametros[0].inicioThread = 0;
        parametros[0].fimThread = numElementos - 1;
        parametros[0].matriz1 = matriz1;
        parametros[0].matriz2 = matriz2;
        parametros[0].matrizResultante = matrizResultante;

        threadSoma((void*) &parametros[0]);
    }else{
        for(i=0; i<numThreads; i++){
            parametros[i].inicioThread = numElementos*i;
            parametros[i].fimThread = numElementos*(i+1) - 1;
            parametros[i].matriz1 = matriz1;
            parametros[i].matriz2 = matriz2;
            parametros[i].matrizResultante = matrizResultante;

            err = pthread_create(&idsThreads[i], NULL, threadSoma, (void*) &parametros[i]);

            if(err != 0){
                fprintf(stderr, "Erro na criacao do thread de soma.\n");
                exit(EXIT_FAILURE);
            }
        }

        for(i=0; i<numThreads; i++){
            err = pthread_join(idsThreads[i], NULL);

            if(err != 0){
                fprintf(stderr, "Erro na juncao do thread de soma.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void* threadGravaMatriz(void* args){
    int* matriz = ((parametrosLerGravar*) args)->matriz;
    char* nomeArq = ((parametrosLerGravar*) args)->nomeArqMatriz;
    register unsigned int numLinCol = ((parametrosLerGravar*) args)->numLinCol;

    FILE *arq = fopen(nomeArq, "w");
    
    if(arq==NULL){
        fprintf(stderr, "Problemas ao abrir o arquivo %s.\n", nomeArq);
    }

    for(register int i=0; i<numLinCol; i++){
        for(register int j=0; j<numLinCol; j++)
            fprintf(arq, "%d ", matriz[i*numLinCol+j]);
        fprintf(arq, "\n");
    }
    fclose(arq);

    return NULL;
}

void gravarLerMatrizes(int* matrizGravar, int* matrizLer, char* nomeArqMatrizGravar, char*nomeArqMatrizLer, unsigned int numLinCol, unsigned int numThreads){
    parametrosLerGravar* parametros;

    parametros = alocaVetorParametrosThreadsLeitura(2);

    parametros[0].matriz = matrizGravar;
    parametros[0].nomeArqMatriz = nomeArqMatrizGravar;
    parametros[0].numLinCol = numLinCol;
    
    parametros[1].matriz = matrizLer;
    parametros[1].nomeArqMatriz = nomeArqMatrizLer;
    parametros[1].numLinCol = numLinCol;

    if(numThreads == 1){
        threadGravaMatriz((void*) &parametros[0]);
        threadLeMatriz((void*) &parametros[1]);
    }else{
        pthread_t* idsThreads;
        int err;

        idsThreads = alocaVetorIdsThreads(2);

        err = pthread_create(&idsThreads[0], NULL, threadGravaMatriz, (void*) &parametros[0]); 
        if(err != 0){
            fprintf(stderr, "Erro na criacao do thread de escrita.\n");
            exit(EXIT_FAILURE);
        }

        err = pthread_create(&idsThreads[1], NULL, threadLeMatriz, (void*) &parametros[1]); 
        if(err != 0){
            fprintf(stderr, "Erro na criacao do thread de leitura.\n");
            exit(EXIT_FAILURE);
        }

        for(unsigned int i=0; i<2; i++){
            err = pthread_join(idsThreads[i], NULL);
            if(err != 0){
                fprintf(stderr, "Erro na juncao do thread de leitura\\escrita.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void* threadMultiplicacao(void* args){
    register unsigned int i, j, k, inicio, fim, numLinCol;
    int *matriz1, *matriz2, *matrizResultante;

    inicio = ((parametrosSomaMul*) args)->inicioThread;
    fim = ((parametrosSomaMul*) args)->fimThread;
    matriz1 = ((parametrosSomaMul*) args)->matriz1;
    matriz2 = ((parametrosSomaMul*) args)->matriz2;
    matrizResultante = ((parametrosSomaMul*) args)->matrizResultante;
    numLinCol = ((parametrosSomaMul*) args)->numLinCol;

    for(i=inicio; i<=fim; i++){
        for(j=0; j<numLinCol; j++){
            int somaMul = 0;
            for(k=0; k<numLinCol; k++){
                somaMul += matriz1[i*numLinCol+k] * matriz2[j*numLinCol+k];
            }
            matrizResultante[i*numLinCol+j] = somaMul;
        }
    }

    return NULL;
}

void multiplicaMatrizes(int* matriz1, int* matriz2, int* matrizResultante, unsigned int numLinCol, unsigned int numThreads){
    parametrosSomaMul* parametros;
    register unsigned int numElementos;

    parametros = alocaVetorParametrosThreadsSomaMul(numThreads);

    if(numLinCol % numThreads != 0){
        fprintf(stderr, "O numero de elementos da matriz nao eh divisivel pelo numero de threads.\n");
        exit(EXIT_FAILURE);
    }

    numElementos = numLinCol/numThreads;

    if(numThreads == 1){
        parametros[0].inicioThread = 0;
        parametros[0].fimThread = numElementos-1;
        parametros[0].matriz1 = matriz1;
        parametros[0].matriz2 = matriz2;
        parametros[0].matrizResultante = matrizResultante;
        parametros[0].numLinCol = numLinCol;

        threadMultiplicacao((void*) &parametros[0]);
    }else{
        pthread_t* idsThreads;
        idsThreads = alocaVetorIdsThreads(numThreads);
        register unsigned int i;
        int err;

        for(i=0; i<numThreads; i++){
            parametros[i].inicioThread = numElementos*i;
            parametros[i].fimThread = numElementos*(i+1) - 1;
            parametros[i].matriz1 = matriz1;
            parametros[i].matriz2 = matriz2;
            parametros[i].matrizResultante = matrizResultante;
            parametros[i].numLinCol = numLinCol;

            err = pthread_create(&idsThreads[i], NULL, threadMultiplicacao, (void*) &parametros[i]);

            if(err != 0){
                fprintf(stderr, "Erro na criacao do thread de multiplicacao.\n");
                exit(EXIT_FAILURE);
            }
        }

        for(i=0; i<numThreads; i++){
            err = pthread_join(idsThreads[i], NULL);

            if(err != 0){
                fprintf(stderr, "Erro na juncao do thread de multiplicacao.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void* threadReducao(void* args){
    register unsigned int i, inicio, fim;
    int *soma;
    int* matriz;

    inicio = ((parametrosSomaMul*) args)->inicioThread;
    fim = ((parametrosSomaMul*) args)->fimThread;
    matriz = ((parametrosSomaMul*) args)->matriz1;

    if((soma = (int*) malloc(sizeof(int))) == NULL){
        fprintf(stderr, "Problemas na alocacao da soma.\n");
        exit(EXIT_FAILURE);
    }

    *soma = 0;

    for(i = inicio; i <= fim; i++){
        *soma += matriz[i];
    }

    return ((void*) soma);
}

int gravarReduzirMatriz(int* matriz, char* nomeArqMatriz, unsigned int numLinCol, unsigned int numThreads){
    parametrosLerGravar* parametrosGravar;
    parametrosSomaMul* parametrosReducao;
    
    void* reducaoParcial = NULL;
    int reducao = 0;

    parametrosGravar = alocaVetorParametrosThreadsLeitura(1);

    parametrosGravar[0].matriz = matriz;
    parametrosGravar[0].nomeArqMatriz = nomeArqMatriz;
    parametrosGravar[0].numLinCol = numLinCol;

    if(numThreads == 1){    
        parametrosReducao = alocaVetorParametrosThreadsSomaMul(1);
        parametrosReducao[0].inicioThread = 0;
        parametrosReducao[0].fimThread = (numLinCol*numLinCol)-1;
        parametrosReducao[0].matriz1 = matriz;

        threadGravaMatriz((void*) &parametrosGravar[0]);
        reducaoParcial = threadReducao((void*) &parametrosReducao[0]); 
        reducao = *((int*)reducaoParcial);
    }else{
        pthread_t* idsThreads;
        int err;

        register unsigned int i, numElementos;
        parametrosReducao = alocaVetorParametrosThreadsSomaMul(numThreads);
        idsThreads = alocaVetorIdsThreads(numThreads);

        if((numLinCol*numLinCol) % numThreads != 0){
            fprintf(stderr, "O numero de elementos da matriz nao eh divisivel pelo numero de threads.\n");
            exit(EXIT_FAILURE);
        }

        numElementos = (numLinCol*numLinCol)/numThreads;

        for(i=0; i<numThreads; i++){
            parametrosReducao[i].inicioThread = numElementos*i;
            parametrosReducao[i].fimThread = numElementos*(i+1) - 1;
            parametrosReducao[i].matriz1 = matriz;

            err = pthread_create(&idsThreads[i], NULL, threadReducao, (void*) &parametrosReducao[i]);

            if(err != 0){
                fprintf(stderr, "Erro na criacao do thread de reducao.\n");
                exit(EXIT_FAILURE);
            }
        }

        threadGravaMatriz((void*) &parametrosGravar[0]);

        for(i=0; i<numThreads; i++){
            err = pthread_join(idsThreads[i], &reducaoParcial);

            if(err != 0){
                fprintf(stderr, "Erro na juncao do thread de reducao.\n");
                exit(EXIT_FAILURE);
            }

            reducao += *((int*) reducaoParcial);
        }

    }
    return reducao;
}