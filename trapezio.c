#include <stdio.h>

int f(int x) {
  return 5;
}

float area(void) {
  int a = 0;
  int b = 10;
  int n = 200;


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
  float y = area();
  printf("O resultado é %f", y);
}
