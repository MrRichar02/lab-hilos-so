#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    long long *fib;
    int N;
} FibonacciData;

void *GenerateFibonacci(void *arg)
{
    FibonacciData *data = (FibonacciData *)arg;

    if (data->N > 0)
        data->fib[0] = 0;

    if (data->N > 1)
        data->fib[1] = 1;

    for (int i = 2; i < data->N; i++)
    {
        data->fib[i] = data->fib[i - 1] + data->fib[i - 2];
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    if (N <= 0)
    {
        printf("N debe ser mayor que 0\n");
        return 1;
    }

    long long *fib = malloc(N * sizeof(long long));

    if (fib == NULL)
    {
        printf("Error reservando memoria\n");
        return 1;
    }

    FibonacciData data;
    data.fib = fib;
    data.N = N;

    pthread_t worker;

    if (pthread_create(&worker, NULL,
                       GenerateFibonacci,
                       &data) != 0)
    {
        printf("Error creando hilo\n");
        free(fib);
        return 1;
    }

    pthread_join(worker, NULL);

    printf("Secuencia de Fibonacci:\n");

    for (int i = 0; i < N; i++)
    {
        printf("%lld ", fib[i]);
    }

    printf("\n");

    free(fib);

    return 0;
}
