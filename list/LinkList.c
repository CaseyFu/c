#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\custom.h"
#include "..\test.c"

/**
 * 链表
 */

typedef int ElemType;
typedef struct Node {
  ElemType data;
  struct Node *next;
} Node;

typedef struct LinkList {
  Node *head;
  Node *tail;
  int length;
} LinkList;

/**
 * 初始化头结点,给data和next赋值
 */
void initList(LinkList *L) {
  L->head = (Node *)malloc(sizeof(Node));
  if (!L->head) return;
  L->head->next = NULL;
  L->tail = L->head;
  L->length = 0;
}

/**
 * 追加
 */
void append(LinkList *L, ElemType e) {
  Node *p = (Node *)malloc(sizeof(Node));
  if (!p) return;
  p->data = e;
  p->next = L->tail->next;
  L->tail->next = p;
  L->tail = p;
  L->length++;
}

/**
 * i插,在第i个位置插入,在最后插入就是尾插
 */
void insert(LinkList *L, int i, ElemType e) {
  if (i < 0) return;
  Node *p = (Node *)malloc(sizeof(Node));
  if (!p) return;
  p->data = e;

  Node *q = L->head;
  for (int j = 0; j < i; j++) {
    q = q->next;
    if (q == NULL) {
      printf("i超出链表长度,不合法\n");
      return;
    }
  }
  if (q->next == NULL) {
    // 表明在最后插入,改变tail指针
    L->tail = p;
  }
  p->next = q->next;
  q->next = p;
  L->length++;
}

/**
 * 删除第i个元素,并用e返回
 */
void delete (LinkList *L, int i, ElemType *e) {
  if (i < 0) return;
  Node *q = L->head;
  for (int j = 0; j < i; j++) {
    q = q->next;
    if (q->next == NULL) {
      printf("i值不合法,超出链表长度\n");
      return;
    }
  }
  Node *p = q->next;
  if (p->next == NULL) {
    // 改变tail
    L->tail = q;
  }
  *e = p->data;
  q->next = p->next;
  L->length--;
  free(p);
}
/**
 * 遍历L
 */
void traverse(LinkList *L, char *message) {
  Node *p = L->head->next;
  printf("=============================\n");
  printf("%s:\n", message);
  while (p != NULL) {
    printf("%d\t", p->data);
    p = p->next;
    if (p == NULL) printf("\n");
  }
  printf("=============================\n");
}

/**
 * 合并La和Lb中的元素按值非递减排列,结果放在La
 */
void mergeList(LinkList *La, LinkList *Lb) {
  Node *p = La->head->next, *q = Lb->head->next, *r;
  if (p->data < q->data) {
    La->head->next = p;
    r = p;
    p = p->next;
  } else {
    La->head->next = q;
    r = q;
    q = q->next;
  }

  while (p != NULL && q != NULL) {
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
  r->next = (p != NULL) ? p : q;
}
/**
 * 链表的逆序
 */
void reverseList(LinkList *L) {
  Node *p = L->head->next->next;
  Node *q = p->next;
  L->head->next->next = NULL;
  while (p != NULL) {
    p->next = L->head->next;
    L->head->next = p;
    p = q;
    if (q != NULL) q = q->next;
  }
}

/**
 * 制空链表
 */
void clearList(LinkList *L) {
  while (L->head->next != NULL) {
    Node *p = L->head->next;
    L->head->next = p->next;
    free(p);
  }
}

void createList(LinkList *La, LinkList *Lb) {
  initList(La);
  initList(Lb);
  for (int i = 0; i < 10; i++) {
    insert(La, i, i);
  }
  for (int i = 0; i < 10; i++) {
    insert(Lb, i, i + 5);
  }
}

int main() {
  LinkList *La = (LinkList *)malloc(sizeof(LinkList));
  LinkList *Lb = (LinkList *)malloc(sizeof(LinkList));
  createList(La, Lb);
  traverse(La, "traverse La");
  insert(La, 0, 999);
  insert(La, 9, 888);
  append(La, 100000);
  ElemType e;
  delete (La, 1, &e);
  printf("删除e:%d\n", e);
  printf("长度:%d\n", La->length);
  traverse(La, "traverse La");
  return 0;
}