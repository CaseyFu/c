#include <stdio.h>
#include <stdlib.h>

/**
 * 双向链表,实现了insert、delete
 */
typedef int ElemType;
typedef struct DList {
  ElemType data;
  struct DList *next;
  struct DList *prior;
} DList;

void initDList(DList *L) {
  L->prior = NULL;
  L->next = NULL;
  L->data = 0;
}

void insert(DList *L, int i, ElemType e) {
  if (i < 0) return;
  DList *p = (DList *)malloc(sizeof(DList));
  if (!p) return;
  p->data = e;
  DList *q = L;
  for (int j = 0; j < i; j++) {
    q = q->next;
    if (q == NULL) {
      // i大了, 链表都走完了
      printf("i不合法\n");
      return;
    }
  }
  if (q->next == NULL) {
    // q指向最后一个,q的下一个没有prior
    p->next = q->next;
    p->prior = q;
    q->next = p;
  } else {
    p->next = q->next;
    p->prior = q->next->prior;
    q->next->prior = p;
    q->next = p;
  }
}

void delete (DList *L, int i, ElemType *e) {
  if (i < 0) return;

  DList *q = L->next;
  for (int j = 0; j < i; j++) {
    q = q->next;
    if (q == NULL) {
      // i大了, 链表都走完了
      printf("i不合法\n");
      return;
    }
  }
  if (q->next == NULL) {
    // 最后一个,没有q->next-prior
    q->prior->next = NULL;
  } else {
    q->next->prior = q->prior;
    q->prior->next = q->next;
  }
  *e = q->data;
  free(q);
}
void traverseDList(DList *L) {
  DList *p = L->next;
  while (p != NULL) {
    printf("%d\t", p->data);
    p = p->next;
  }
  printf("\n");
}
void createList(DList *L) {
  initDList(L);
  for (int i = 0; i < 10; i++) {
    insert(L, i, i);
  }
}
int main() {
  DList *L = (DList *)malloc(sizeof(DList));
  createList(L);
  ElemType e;
  delete (L, 9, &e);
  printf("%d\n", e);
  traverseDList(L);
  return 0;
}