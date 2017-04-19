#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void generaNumero(int *a, unsigned long elementos);
void suma(int *arreglo, int nt, unsigned long elementos);
void reduce(int *s, int p);

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (argc != 3)
    {
        fprintf(stderr, "error, ejecutar programa como ./prog N\n nt");
        exit(EXIT_FAILURE);
    }

    int nt = atoi(argv[2]);
    unsigned long elementos = atoi(argv[1]);

    int *x = (int *)malloc(sizeof(int) * elementos);
    generaNumero(x, elementos);
    suma(x, nt, elementos);

    return EXIT_SUCCESS;
}

void generaNumero(int *a, unsigned long elementos)
{
    for (unsigned long i = 0; i < elementos; i++)
    {
        //a[i] = rand() % 100;
        a[i] = 1;
    }
}

void suma(int *arreglo, int nt, unsigned long elementos)
{
    omp_set_num_threads(nt);
    int cant = elementos / nt;

    int *s = (int *)malloc(sizeof(int) * nt);
    double t1 = omp_get_wtime();

#pragma omp parallel shared(arreglo, cant, s)
    {
        int tid = omp_get_thread_num();
        int t = tid * cant;
        int sum = 0;
        for (int i = t; i < (t + cant); i++)
        {
            sum += arreglo[i]; 
        }
        s[tid] = sum;
#pragma omp barrier
        reduce(s, nt / 2);
    }
    double t2 = omp_get_wtime();
    printf("Tiempo %f \n", t2 - t1);
}

void reduce(int *s, int p)
{
    int tid = omp_get_thread_num();
    if (p == 0)
    {
        if (tid == 0)
        {
            printf("%i \n", s[0]);
        }
        return;
    }

    if (tid < p)
    {
        s[tid] = s[tid] + s[tid + p];
    }

#pragma omp barrier
    reduce(s, p / 2);
}
