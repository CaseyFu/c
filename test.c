#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printChar(char* s) {
  printf("=============================\n");
  printf("output char*:\n");
  printf("%s\n", s);
  printf("=============================\n");
}
void printInt(int i) {
  printf("=============================\n");
  printf("output int:\n");
  printf("%d\n", i);
  printf("=============================\n");
}
void printDouble(double i) {
  printf("=============================\n");
  printf("output double:\n");
  printf("%.2f\n", i);
  printf("=============================\n");
}
// int mistake(int *x, int *y) {
//   // 这是一个错误,看起来是通过交换地址交换变量值,但是x和y没有取*
//   // 只有通过p->x或(*p).x才能改变地址空间
//   //   int *temp;
//   //   temp = x;
//   //   x = y;
//   //   y = temp;
//   //   printf("test:x=%d y=%d\n", x, y);
// }
// int test(Node *p) {
//   printf("test:x=%d y=%d\n", p->x, p->y);
//   int temp = (*p).x;
//   (*p).x = (*p).y;
//   (*p).y = temp;
// }
// int test1(char *s) { printf("test:x=%d", strcmp(s, "hhh")); }

// typedef struct Node {
//   int data;
//   int cur;
// } Node[10];
// int test2(Node *L) {
//   for (int i = 0; i < 10; i++) {
//     (*L + i)->cur = i + 1;
//   }
// }

typedef struct Test {
  int x;
  int y;
} Test;

void test(Test* T, Test* B) {
  B = T;
  printf("(%d,%d)", B->x, B->y);
  free(T);
}
int main() {
  Test* T = (Test*)malloc(sizeof(Test));
  Test B;
  T->x = 10;
  T->y = 10;
  test(T, &B);
  printf("(%d,%d)", B.x, B.y);
}
