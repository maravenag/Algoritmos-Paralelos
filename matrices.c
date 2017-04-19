#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int calculaResultado(float *matriz1, float *matriz2, float *resultante, int i, int j, int k, int blocks, int N)
{

    float temporal = 0;
    for (int a = i; a < i + blocks; a++)
    {
        for (int b = j; b < j + blocks; b++)
        {
            temporal = 0;
            for (int k = 0; k < blocks; k++)
            {
                temporal += matriz1[(a * N) + (j + k)] * matriz2[((i + k) * N) + b];
            }
            resultante[a * N + b] += temporal;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "error, ejecutar programa como ./prog N nt B \n");
        exit(EXIT_FAILURE);
    }

    int nt = atoi(argv[2]);

    unsigned long N = atoi(argv[1]);

    srand(time(NULL));

    float *matriz1 = (float *)malloc(sizeof(float) * (N * N));
    float *matriz2 = (float *)malloc(sizeof(float) * (N * N));

    int blocks = atoi(argv[3]);

    for (unsigned long i = 0; i < N; i++)
    {
        for (unsigned long j = 0; j < N; j++)
        {
            matriz1[i * N + j] = rand();
            matriz2[i * N + j] = rand();
        }
    }

    float *resultante = (float *)malloc(sizeof(float) * (N * N));
    double t1 = omp_get_wtime();

    omp_set_num_threads(nt);
    omp_set_nested(1);

#pragma omp parallel for shared(matriz1, matriz2, resultante)
    for (unsigned int i = 0; i < N; i += blocks)
    {

#pragma omp parallel for shared(matriz1, matriz2, resultante)
        for (unsigned int j = 0; j < N; j += blocks)
        {
            //float temporal = 0;

            for (unsigned int k = 0; k < N; k += blocks)
            {
                calculaResultado(matriz1, matriz2, resultante, i, j, k, blocks, N);
                //temporal += matriz1[(i * N) + k] * matriz2[(k * N) + j];
            }
            //resultante[i * N + j] = temporal;
            //printf("%f \n", resultante[i * N + j]);
        }
    }

    /*
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%f ", resultante[i * N + j]);
        }
        printf("\n");
    }
    */
    double t2 = omp_get_wtime();
    printf("tiempo: %f \n", t2 - t1);

    return 0;
}
