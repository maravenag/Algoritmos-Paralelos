#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "error, ejecutar programa como ./prog N\n nt");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int c = 3;
    int nt = atoi(argv[2]);
    unsigned long elementos = atoi(argv[1]);

    float *x = (float *)malloc(sizeof(float) * elementos);
    float *y = (float *)malloc(sizeof(float) * elementos);

    omp_set_num_threads(nt);
    double t1 = omp_get_wtime();
#pragma omp parallel for shared(c, x, y)
    for (unsigned long i = 0; i < elementos; i++)
    {
        x[i] = y[i] = (float)i;
        y[i] = c * x[i] + y[i];
        //printf("y[%i] = %i \n", i, y[i]);
    }
    double t2 = omp_get_wtime();
    printf("tiempo = %f sec \n", t2 - t1);
    return EXIT_SUCCESS;
}