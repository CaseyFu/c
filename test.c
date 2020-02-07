#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void rotate() {
  int i = rand() % 10;
  printf("%d\n", i);
}

int main() {
  srand(time(NULL));
  char* p = "   ";
  int i = 0;
  for (char* c = p; *c; c++) {
    i++;
  }
  printf("%d", *p);
  return 0;
}
