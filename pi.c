#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double GetTime() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

double f(double x) {
  return 4.0 / (1.0 + x * x);
}

double CalcPi(int n) {
  const double fH = 1.0 / (double)n;
  double fSum = 0.0;
  double fX;
  int i;
  // Bucle principal para paralelizar
  for (i = 0; i < n; i += 1) {
    fX = fH * ((double)i + 0.5);
    fSum += f(fX);
  }
  return fH * fSum;
}

int main(int argc, char *argv[]) {
  double start_time, end_time;
  int n = 100000000; // Número de intervalos
  if (argc > 1) {
    n = atoi(argv[1]);
  }
  start_time = GetTime();
  double pi = CalcPi(n);
  end_time = GetTime();
  printf("Pi aproximado: %.15f\n", pi);
  printf("Tiempo de ejecución: %.6f segundos\n", end_time - start_time);
  return 0;
}