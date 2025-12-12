#define _POSIX_C_SOURCE 199309L // Necessaria para a funcao gettime
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void matriz_sequencial(int N, int matriz[2][N][N]);
void matriz_paralela(int N, int matriz[2][N][N]);
void mostra_matriz(int N, int matriz[N][N]);

int main() {
    srand(time(0));
    int N=-1;
    while(N < 1){
        printf("Insira a ordem da matriz:\n");
        scanf("%d", &N);
    }

    int matriz[2][N][N];
    printf("Insira os valores das matrizes:\n");

    for (int m = 0; m < 2; m++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                //scanf("%d", &matriz[m][i][j]);
                matriz[m][i][j] = rand() % 10;
            }
        }
        printf("------ \n");
    }
    
    struct timespec inicio, fim;
    double tempoS, tempoP;

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    // Multiplicacao sequencial
    matriz_sequencial(N, matriz);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempoS = (double)(fim.tv_sec - inicio.tv_sec);
    tempoS += (double)(fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("------ \n");

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    // Multiplicacao paralela
    matriz_paralela(N, matriz);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempoP = (double)(fim.tv_sec - inicio.tv_sec);
    tempoP += (double)(fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
    printf("------ \n");

    printf("O tempo da funcao sequencial foi %lf s \n", tempoS);
    printf("O tempo da funcao paralela foi: %lf s \n", tempoP);

    return 0;
}

void matriz_sequencial(int N, int matriz[2][N][N]){
    int resultado[N][N], nPares = 0;
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            resultado[i][j] = 0;
            for(int k=0; k<N; k++){
                resultado[i][j] += matriz[0][i][k] * matriz[1][k][j];
            }
            (resultado[i][j] % 2 == 0)? (nPares++) : (nPares = nPares);
        }
    }

    //mostra_matriz(N, resultado);

    printf("Quantidade de pares: %d \n", nPares);

}

void matriz_paralela(int N, int matriz[2][N][N]){
    int resultado[N][N], nPares = 0;
    #pragma omp parallel for
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            resultado[i][j] = 0;
            for(int k=0; k<N; k++){
                resultado[i][j] += matriz[0][i][k] * matriz[1][k][j];
            }
            #pragma omp critical
            {
            (resultado[i][j] % 2 == 0)? nPares++ : (nPares = nPares);
            }
        }
    }

    //mostra_matriz(N, resultado);

    printf("Quantidade de pares: %d \n", nPares);
}
void mostra_matriz(int N, int matriz[N][N]){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}