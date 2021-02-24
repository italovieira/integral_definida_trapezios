#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NTHREADS 6

typedef struct {
  int n;
  double a;
  double b;
} Trapezoids;


int f1(double x) {
  return 5;
}

int f2(double x) {
  return sin(2 * x) + cos(5 * x);
}

void * area(Trapezoids *trapezoids) {
  int local_n = trapezoids->n / NTHREADS;
  int local_a = trapezoids->a;
  double h = (double) (trapezoids->b - trapezoids->a) / trapezoids->n;
  int local_b = local_a + h;

  double acc = (double) (f2(local_a) + f2(local_b)) / 2;
  for (int i = 1; i < local_n; i++) {
    float x_i = local_a + i * h;
    acc += f2(x_i);
  }

  double *area = malloc(sizeof (double));
  *area = h * acc;

  printf("Área parcial é %f\n", *area);

  pthread_exit(area);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    return 1;
  }

  int t = strtol(argv[1], NULL, 10);
  int n = strtol(argv[2], NULL, 10);
  pthread_t threads[t];
  double *acc_areas[t];

  Trapezoids *trapezoids = malloc(sizeof (Trapezoids));
  trapezoids->n = n;
  trapezoids->a = 0;
  trapezoids->b = 2 * M_PI;

  // Criando threads
  for (int i = 0; i < NTHREADS; i++) {
    int status = pthread_create(&threads[i], NULL, (void *) area, (void *) trapezoids);

    if (status < 0) {
      return 1;
    }
  }

  // Obtendo o valor de retorno das threads e somando para obter a área total
  double total_area = 0;
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], (void *) &acc_areas[i]);
    total_area += *acc_areas[i];
  }

  printf("%.2e\n", total_area);
}
