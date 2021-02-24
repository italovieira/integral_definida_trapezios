#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int t;
  int n;
  double a;
  double b;
  double (*f) (double);
} Values;


double f1(double x) {
  return 5;
}

double f2(double x) {
  return sin(2 * x) + cos(5 * x);
}

void * subarea(Values *values) {
  double (*f) (double) = values->f;
  int local_n = values->n / values->t;
  double h = (values->b - values->a) / values->n;
  double local_a = values->a + h * local_n;
  double local_b = local_a + local_n * h;

  double acc = ((*f)(local_a) + (*f)(local_b)) / 2;
  for (int i = 1; i < local_n; i++) {
    float x_i = local_a + i * h;
    acc += (*f)(x_i);
  }

  double *area = malloc(sizeof (double));
  *area = h * acc;

  pthread_exit(area);
}

double area(Values *values) {
  int t = values->t;
  pthread_t threads[t];
  double *acc_areas[t];

  // Criando threads
  for (int i = 0; i < t; i++) {
    int status = pthread_create(&threads[i], NULL, (void *) subarea, (void *) values);

    if (status < 0) {
      exit(1);
    }
  }

  // Obtendo o valor de retorno das threads e somando para obter a área total
  double total_area = 0;
  for (int i = 0; i < t; i++) {
    pthread_join(threads[i], (void *) &acc_areas[i]);
    total_area += *acc_areas[i];
  }

  return total_area;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    return 1;
  }

  int t = strtol(argv[1], NULL, 10);
  int n = strtol(argv[2], NULL, 10);

  printf("Cálculo será feito usando %d threads e %d trapézios\n\n", t, n);

  Values *values_f1 = malloc(sizeof (Values));
  values_f1->n = n;
  values_f1->a = 0;
  values_f1->b = 10;
  values_f1->t = t;
  values_f1->f = &f1;
  printf("Teste com f1 (a = 0, b = 10): %.2e\n", area(values_f1));

  Values *values_f2 = malloc(sizeof (Values));
  values_f2->n = n;
  values_f2->a = 0;
  values_f2->b = 2 * M_PI;
  values_f2->t = t;
  values_f2->f = &f2;
  printf("Teste com f2 (a = 0, b = 2*PI): %.2e\n", area(values_f2));
}
