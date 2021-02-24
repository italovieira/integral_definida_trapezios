#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int t;
  int n;
  double a;
  double b;
} Values;


double f1(double x) {
  return 5;
}

double f2(double x) {
  return sin(2 * x) + cos(5 * x);
}

void * area(Values *values) {
  int local_n = values->n / values->t;
  double h = (values->b - values->a) / values->n;
  double local_a = values->a + h * local_n;
  double local_b = local_a + local_n * h;

  double acc = (f2(local_a) + f2(local_b)) / 2;
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

  Values *values = malloc(sizeof (Values));
  values->n = n;
  values->a = 0;
  values->b = 2 * M_PI;
  values->t = t;

  // Criando threads
  for (int i = 0; i < t; i++) {
    int status = pthread_create(&threads[i], NULL, (void *) area, (void *) values);

    if (status < 0) {
      return 1;
    }
  }


  // Obtendo o valor de retorno das threads e somando para obter a área total
  double total_area = 0;
  for (int i = 0; i < t; i++) {
    pthread_join(threads[i], (void *) &acc_areas[i]);
    total_area += *acc_areas[i];
  }

  printf("%.2e\n", total_area);
}
