#ifndef FUNC_THREADS_H
#define FUNC_THREADS_H
#include <pthread.h>

typedef struct {
    pthread_t tidPrincipal;
    int *matrizA, *matrizB, *matrizC, *matrizD, *matrizE;
    int numLinCol;
} parametrosThreads;



#endif