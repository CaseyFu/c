#include <stdio.h>
#include <stdlib.h>

/**
 * 循环链表
 */

typedef int ElemType;
typedef struct CList {
  ElemType data;
  struct CList *next;
} CList;

/**
 * 初始化,首位相连
 */
void initCList(CList *L) { L->next = L; }

void insert(CList *L, int i, ElemType e) {
  if (i < 0) return;
  CList *p = (CList *)malloc(sizeof(CList));
  if (!p) return;
  p->data = e;
  CList *q = L;
  if (i != 0) {
    // 非0位置
    for (int j = 0; j < i; j++) {
      q = q->next;
      if (q == L) {
        // i值不合法
        printf("i值不合法\n");
        return;
      }
    }
  }
  // 0位置直接连接
  p->next = q->next;
  q->next = p;
}

/**
 * 删除结点
 */
void delete (CList *L, int i, ElemType *e) {
  if (i < 0) return;
  CList *p = L, *q;
  for (int j = 0; j < i; j++) {
    p = p->next;
    if (p->next == L) {
      // i不合法
      printf("i值不合法\n");
      return;
    }
  }
  q = p->next;
  *e = q->data;
  p->next = q->next;
  free(q);
}

/**
 * 遍历L
 */
void traverseCList(CList *L) {
  CList *p = L->next;
  while (p != L) {
    printf("%d\t", p->data);
    p = p->next;
  }
  printf("\n");
}

/**
 * 合并La和Lb为La
 */
void mergeList(CList *La, CList *Lb) {
  CList *p = La->next, *q = Lb->next, *r;
  if (p->data < q->data) {
    r = p;
    p = p->next;
  } else {
    La->next = q;
    r = q;
    q = q->next;
  }

  while (p != La && q != Lb) {
    if (p->data < q->data) {
      r->next = p;
      r = p;
      p = p->next;
    } else {
      r->next = q;
      r = q;
      q = q->next;
    }
  }
  if (p != La) {
    r->next = p;
  } else {
    r->next = q;
    while (1) {
      // 链表Lb的最后一个与La连成环
      if (q->next == Lb) {
        q->next = La;
        break;
      }
      q = q->next;
    }
  }
  free(Lb);
}
void createList(CList *La, CList *Lb) {
  initCList(La);
  initCList(Lb);

  for (int i = 0; i < 10; i++) {
    insert(La, i, i * 2);
  }
  for (int i = 0; i < 10; i++) {
    insert(Lb, i, (i * 3) + 2);
  }
}
int main() {
  CList *La = (CList *)malloc(sizeof(CList));
  CList *Lb = (CList *)malloc(sizeof(CList));
  ElemType e;
  createList(La, Lb);
  //   delete (L, 9, &e);
  //   printf("%d\n", e);
  //   insert(L, 11, 123);
  traverseCList(La);
  traverseCList(Lb);
  mergeList(La, Lb);
  traverseCList(La);
  //   traverseCList(Lb);
  free(La);
  //   free(Lb);
  return 0;
}