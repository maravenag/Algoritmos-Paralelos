#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void suma(float *arreglo, int nt, unsigned long elementos);
void reduce(float *s, int p);

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        fprintf(stderr, "error, ejecutar programa como ./prog N \n");
        exit(EXIT_FAILURE);
    }
    int nt = atoi(argv[2]);

    srand(time(NULL));
    int N = atoi(argv[1]);

    float *x = (float *)malloc(sizeof(float) * N);
    float *y = (float *)malloc(sizeof(float) * N);

    float acc = 0;
    for (int i = 0; i < N; i++)
    {
        x[i] = 1;
        //acc += x[i];
        //y[i] = acc;
        //printf("x[i]= %f , y[i]=%f \n",x[i], y[i]);
    }
    suma(x, nt, N);
}

void suma(float *arreglo, int nt, unsigned long elementos)
{
    omp_set_num_threads(nt);
    int cant = elementos / nt;

    float *s = (float *)malloc(sizeof(float) * nt);
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
    for(int i = 0;i < nt; i++){
        printf("%f -", s[i]);
    }
    //printf("Tiempo %f \n", t2 - t1);
}

void reduce(float *s, int p)
{
    int tid = omp_get_thread_num();
    if (p == 0)
    {
        if (tid == 0)
        {
            //printf("%f \n", s[0]);
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