#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct
{
  long long start;
  long long end;
  double fH;
  double partial_sum;
} __attribute__((aligned(64))) ThreadData;

double GetTime()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

double f(double x)
{
  return 4.0 / (1.0 + x * x);
}

void *ThreadCalcPi(void *arg)
{
  ThreadData *data = (ThreadData *)arg;
  data->partial_sum = 0.0;

  for (long long i = data->start; i < data->end; i++)
  {
    double x = data->fH * ((double)i + 0.5);
    data->partial_sum += f(x);
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  double start_time, end_time;
  long long n = 100000000; // Número de intervalos
  int n_threads = 4;       // Número de hilos
  if (argc > 1)
  {
    n = atoll(argv[1]);
  }
  if (argc > 2)
  {
    n_threads = atoi(argv[2]);
  }
  if (n_threads <= 0 || n <= 0)
  {
    fprintf(stderr, "Error: n y n_threads deben ser positivos\n");
    return 1;
  }

  double fH = 1.0 / (double)n;
  pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
  ThreadData *thread_data = malloc(n_threads * sizeof(ThreadData));
  long long intervals_per_thread = n / n_threads;
  double total_sum = 0.0;
  start_time = GetTime();
  for (int t = 0; t < n_threads; t++)
  {
    thread_data[t].start = t * intervals_per_thread;
    thread_data[t].end = (t == n_threads - 1) ? n : (t + 1) * intervals_per_thread;
    thread_data[t].fH = fH;
    pthread_create(&threads[t], NULL, ThreadCalcPi, &thread_data[t]);
  }
  for (int t = 0; t < n_threads; t++)
  {
    pthread_join(threads[t], NULL);
    total_sum += thread_data[t].partial_sum;
  }
  double pi = fH * total_sum;
  end_time = GetTime();
  printf("Pi aproximado: %.15f\n", pi);
  printf("Número de hilos: %d\n", n_threads);
  printf("Tiempo de ejecución: %.6f segundos\n", end_time - start_time);
  free(threads);
  free(thread_data);
  return 0;
}