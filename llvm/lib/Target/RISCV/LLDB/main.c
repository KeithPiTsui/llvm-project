#include <stdio.h>

int x = 1;
int y = 2;

int add(int a, int b, int c, int d, int e,
        int f, int g, int h, int i, int j
        /* int k, int l, int m, int n, int o, */
        /* int p, int q, int r, int s, int t */
  ){
  return
      a + b + c + d + e +
      f + g + h + i + j // +
      /* k + l + m + n + o + */
      /* p + q + r + s + t */
      ;
}

int main(int argc, char **argv) {
  printf("hello world!\n");
  return add(x, y, 3, 4, 5, 6, 7, 8, 9, 10); //
             //11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
}
