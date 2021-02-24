#include <stdio.h>

typedef struct {
  int n;
  float a;
  float b;
} Trapezoids;


int f(int x) {
  return 5;
}

float area(Trapezoids trapezoids) {
  int n = trapezoids.n;
  int a = trapezoids.a;
  int b = trapezoids.b;

  float h = (float) (b - a) / n;

  float acc = (float) (f(a) + f(b)) / 2;

  for (int i = 1; i < n; i++) {
    float x_i = a + i * h;
    acc += f(x_i);
  }
  printf("area %f", acc);

  float area_total = h * acc;

  return area_total;
}

int main(void) {
  Trapezoids trapezoids = {200, 0, 10};
  float y = area(trapezoids);
  printf("O resultado é %f", y);
}
