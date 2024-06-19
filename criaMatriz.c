#include <stdio.h>

void criaMatriz(int numeroLinhaColuna, char letra);

int main(int argc, char* argv[]){
    int numLinCol = atoi(argv[1]);
    criaMatriz(numLinCol, 'A');
    criaMatriz(numLinCol, 'B');
    criaMatriz(numLinCol, 'C');
    
    return 0;
}

void criaMatriz(int numeroLinhaColuna, char letra){
    char matriz[9];
    snprintf(matriz, 10, "arq%c.dat", letra);
    FILE *arq = fopen(matriz, "w+");
    for(int i=0; i<numeroLinhaColuna; i++){
        for(int j=0; j<numeroLinhaColuna; j++)
            fprintf(arq, "%d ", 1);
        fprintf(arq, "\n");
    }
    fclose(arq);
}