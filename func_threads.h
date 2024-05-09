#ifndef FUNC_THREADS_H
#define FUNC_THREADS_H
#include <pthread.h>

typedef struct {
    pthread_t tidPrincipal;
    char* nome;
    int* matriz;
    int numLinCol;
} parametrosThreads;



#endif