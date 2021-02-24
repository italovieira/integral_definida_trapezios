#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5

typedef struct {
  int n;
  float a;
  float b;
} Trapezoids;


int f(int x) {
  return 5;
}

void * area(Trapezoids *trapezoids) {
  int local_n = trapezoids->n / NTHREADS;
  int local_a = trapezoids->a;
  float h = (float) (trapezoids->b - trapezoids->a) / trapezoids->n;
  int local_b = local_a + h;

  float acc = (float) (f(local_a) + f(local_b)) / 2;
  for (int i = 1; i < local_n; i++) {
    float x_i = local_a + i * h;
    acc += f(x_i);
  }

  float *area = malloc(sizeof (float));
  *area = h * acc;

  pthread_exit(area);
}

int main(void) {
  pthread_t threads[NTHREADS];
  float *acc_areas[NTHREADS];

  Trapezoids *trapezoids = malloc(sizeof (Trapezoids));
  trapezoids->n = 200;
  trapezoids->a = 0;
  trapezoids->b = 10;

  // Criando threads
  for (int i = 0; i < NTHREADS; i++) {
    int status = pthread_create(&threads[i], NULL, (void *) area, (void *) trapezoids);

    if (status < 0) {
      return 1;
    }
  }

  // Obtendo o valor de retorno das threads e somando para obter a área total
  float total_area = 0;
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], (void *) &acc_areas[i]);
    total_area += *acc_areas[i];
  }

  printf("%.2e\n", total_area);
}
