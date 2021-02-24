#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MIN(a, b) ((a) < (b) ? a : b)

typedef struct {
  int t;
  int n;
  int tid;
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

  // Tratamento para n/t não inteiro
  int remainder = values->n % values->t;
  int rest = values->tid < remainder;
  int increment = MIN(values->tid, remainder);
  double local_a = values->a + h * (local_n * values->tid + increment);
  double local_b = local_a + h * (local_n + rest);

  double acc = ((*f)(local_a) + (*f)(local_b)) / 2;
  for (int i = 1; i < (local_n + rest); i++) {
    double x_i = local_a + i * h;
    acc += (*f)(x_i);
  }

  double *area = malloc(sizeof (double));
  *area = h * acc;

  pthread_exit(area);
}

double area(int t, int n, double a, double b, double (*f) (double)) {
  pthread_t threads[t];
  double *acc_areas[t];

  // Criando threads
  for (int i = 0; i < t; i++) {
    Values *values = malloc(sizeof (Values));
    values->tid = i;
    values->n = n;
    values->a = a;
    values->b = b;
    values->t = t;
    values->f = f;

    int status = pthread_create(&threads[i], NULL, (void *) subarea, (void *) values);
    if (status != 0) {
      fprintf(stderr, "Falha ao tentar criar uma thread.");
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
    fprintf(stderr, "Está faltando o número de threads e/ou trapézios\n");
    return 1;
  }

  int t = strtol(argv[1], NULL, 10);
  int n = strtol(argv[2], NULL, 10);

  printf("Cálculo será feito usando %d threads e %d trapézios\n\n", t, n);

  printf("Teste com f1 (a = 0, b = 10): %.2e\n", area(t, n, 0, 10, &f1));
  printf("Teste com f2 (a = 0, b = 2*PI): %.2e\n", area(t, n, 0, 2 * M_PI, &f2));
}
