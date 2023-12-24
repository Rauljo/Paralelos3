#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

//Lo que hacemos es una version no paralela del programa

int veces = 1;

double* CrearMatriz(int tamanho){
    double* matriz = (double*)malloc(tamanho * tamanho * sizeof(double));
    if (!matriz){
        perror("No se ha podido reservar memoria para la matriz\n");
        return NULL;
    }

    for (long long int i = 0; i < tamanho; i++){
        for (long long int j = 0; j < tamanho; j++){
            matriz[i*tamanho + j] = veces * j;
        }
    }

    veces++;

    return matriz;
}

void imprimirMatriz(double* matriz, int tamano) {
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            printf("%lf ", matriz[i * tamano + j]);
        }
        printf("\n");
    }
}

// Función para leer una matriz desde un archivo
double* leerMatrizDesdeArchivo(const char* nombreArchivo, int* tamano) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Lee el tamaño de la matriz desde la primera línea del archivo
    fscanf(archivo, "%d", tamano);

    int N = *tamano;

    // Reserva memoria para la matriz
    double* matriz = (double*)malloc(N * N * sizeof(double));
    //double* matriz_transpuesta;
   

    // Lee los elementos de la matriz desde el archivo
    for (int i = 0; i < *tamano; i++) {
        for (int j = 0; j < *tamano; j++) {
            fscanf(archivo, "%lf", &matriz[i * N + j]);
        }
    }

    // Cierra el archivo
    fclose(archivo);

    //matriz_transpuesta = transposeMatrix(matriz, N);

    //free(matriz);



    return matriz;
}

int main(int argc, char** argv){
    if (argc != 4){
        printf("Error en el numero de argumentos\n");
        return -1;
    }

    int paso;
    int N;
    double REAL;

    double t_resolution;
    double t_end;
    double measure_overhead;
    double t_start;


    //Primero vemos el overhead
    t_resolution = MPI_Wtick();
    t_start = MPI_Wtime();
    t_end = MPI_Wtime();
    measure_overhead = t_end - t_start;

    printf("Overhead: %lf\n", measure_overhead);

    double *matrizA, *matrizB, *matrizC;
    int tamanhoMatriz;

    if (N > 1024){
            //printf("Tamaño de matriz demasiado grande, se creará en el programa\n");
            matrizA = CrearMatriz(N);
            matrizB = CrearMatriz(N);
        }
        else{
            matrizA = leerMatrizDesdeArchivo("matrizA.txt", &tamanhoMatriz);
            matrizB = leerMatrizDesdeArchivo("matrizB.txt", &tamanhoMatriz);
        }

    t_start = MPI_Wtime();

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            matrizC[i*N + j] = matrizA[i*N + j] * REAL + matrizB[i*N + j];
        }
    }

    t_end = MPI_Wtime();

    double t_elapsed = (t_end - t_start) + measure_overhead;
    printf("The calculation has taken %e globally.\n", t_elapsed);

    free(matrizA);
    free(matrizB);
    free(matrizC);




}