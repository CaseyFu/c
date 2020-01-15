#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\custom.h"
#include "..\test.c"
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
/**
 * 顺序表
 */

typedef int ElemType;
typedef struct {
  ElemType *elem;
  int length;
  int listsize;
} SqList;

/**
 * 初始化顺序表
 */
int initList(SqList *L) {
  if (!L) return 0;
  L->elem = (ElemType *)malloc((LIST_INIT_SIZE) * sizeof(ElemType));
  if (!L->elem) return 0;
  L->length = 0;
  L->listsize = LIST_INIT_SIZE;
  return 1;
}
/**
 * 销毁线性表
 */
int destroyList(SqList *L) {
  if (!L) return 0;
  free((*L).elem);
  L->length = 0;
  L->listsize = 0;
  free(L);
  return 1;
}

/**
 * 置为空表
 */
int clearList(SqList *L) {
  if (!L) return 0;
  L->length = 0;
  return 1;
}

/**
 * 判断L是否为空表
 * 空返回1
 */
int listIsEmpty(SqList L) { return L.length = 0; }
/**
 * 返回元素个数
 */
int listLength(SqList L) { return L.length; }
/**
 * 用e返回L中的第i个数据元素
 */
int getElem(SqList L, int i, ElemType *e) {
  if (i < 0 || i > L.length) return 0;
  *e = L.elem[i];
  return 1;
}

/**
 * 返回L中第一个比e小的数据元素的位置,不存在返回0
 * 返回第一个比e大,比e小,等于e的数的位置
 * 这里是返回第一个比e小的数的位置
 * lt less than
 * gt greater than
 * eq equal
 */
int locateElem(SqList L, ElemType e, char *criterion) {
  if (L.length == 0) return 0;
  if (strcmp(criterion, "lt") == 0) {
    for (int i = 0; i < L.length; i++) {
      if (L.elem[i] < e) {
        return i;
      }
    }
  } else if (strcmp(criterion, "gt") == 0) {
    for (int i = 0; i < L.length; i++) {
      if (L.elem[i] > e) {
        return i;
      }
    }
  } else {
    for (int i = 0; i < L.length; i++) {
      if (L.elem[i] == e) {
        return i;
      }
    }
  }
  return -1;
}
/**
 * 用preE返回第一个为e的前驱结点,没有则null
 */
int priorElem(SqList L, ElemType e, ElemType *preE) {
  if (L.length <= 0) return 0;
  for (int i = 1; i < L.length; i++) {
    if (e == L.elem[i]) {
      *preE = L.elem[i - 1];
      return 1;
    }
  }
  return 0;
}
/**
 * 用nextE返回e的后置结点,没有则null
 */
int nextElem(SqList L, ElemType e, ElemType *nextE) {
  if (L.length < 0) return 0;
  for (int i = 0; i < L.length - 1; i++) {
    if (e == L.elem[i]) {
      *nextE = L.elem[i + 1];
      return 1;
    }
  }
  return 0;
}
/**
 * 在L的第i个位置前插入e,L长度+1
 */
int listInsert(SqList *L, int i, ElemType e) {
  if (i < 0 || i > L->length) return 0;
  // 判断L空间
  if (L->length >= L->listsize - LISTINCREMENT) {
    ElemType *temp = (ElemType *)realloc(
        (*L).elem, (L->listsize + LISTINCREMENT) * sizeof(ElemType));
    if (!temp) return 0;
    (*L).elem = temp;
    L->listsize += LISTINCREMENT;
  }
  if (i == 0) {
    // 插入第一个时
    L->elem[0] = e;
  } else {
    int j = L->length - 1;
    for (; j >= i; j--) {
      L->elem[j + 1] = L->elem[j];
    }
    L->elem[j + 1] = e;
  }
  L->length++;

  return 1;
}
/**
 * 删除第i个元素,并用e返回,L长度-1
 */
int listDelete(SqList *L, int i, ElemType *e) {
  if (i < 0 || i > L->length) return 0;
  *e = L->elem[i];
  for (int j = i; j < L->length; j++) {
    L->elem[j] = L->elem[j + 1];
  }
  L->length--;
  return 1;
}
/**
 * 对L中的每个元素调用visit()
 */
int listTraverse(SqList L, char *message) {
  if (L.length < 0) return 0;
  printf("=============================\n");
  printf("%s:\n", message);
  for (int i = 0; i < L.length; i++) {
    printf("%d\t", L.elem[i]);
    if (i == L.length - 1) {
      printf("\n");
    }
  }
  printf("=============================\n");
  return 1;
}
/**
 * 返回L占的总的空间的大小
 */
int totalSize(SqList *L) { return sizeof(ElemType) * LIST_INIT_SIZE; }

/**
 * 合并La和Lb中的元素按值非递减排列
 */
void mergeList(const SqList La, const SqList Lb, SqList *Lc) {
  int a = 0;
  int b = 0;
  int c = 0;
  int laLength = listLength(La);
  int lbLength = listLength(Lb);
  while (a < laLength && b < lbLength) {
    if (La.elem[a] < Lb.elem[b]) {
      listInsert(Lc, c++, La.elem[a++]);
    } else {
      listInsert(Lc, c++, Lb.elem[b++]);
    }
  }
  while (a < laLength) {
    listInsert(Lc, c++, La.elem[a++]);
  }
  while (b < lbLength) {
    listInsert(Lc, c++, Lb.elem[b++]);
  }
}
/**
 * 在La中插入元素Lb中有的而La中没有的元素
 */
void unionList(SqList *La, const SqList Lb) {
  if (La->length == 0 || Lb.length == 0) return;
  int laLength = La->length;
  for (int i = 0; i < Lb.length; i++) {
    if (locateElem(*La, Lb.elem[i], "eq") == -1) {
      listInsert(La, laLength++, Lb.elem[i]);
    }
  }
  return;
}
int partition(SqList *L, int left, int right) {
  int key = L->elem[left];
  while (left < right) {
    while (left < right && L->elem[right] >= key) {
      right--;
    }
    L->elem[left] = L->elem[right];
    while (left < right && L->elem[left] <= key) {
      left++;
    }
    L->elem[right] = L->elem[left];
  }
  L->elem[left] = key;
  return left;
}
void quickSort(SqList *L, int left, int right) {
  if (left < right) {
    int mid = partition(L, left, right);
    quickSort(L, left, mid - 1);
    quickSort(L, mid + 1, right);
  }
}
void createList(SqList *La, SqList *Lb, SqList *Lc) {
  initList(La);
  initList(Lb);
  initList(Lc);
  listInsert(La, 0, 5);
  listInsert(La, 1, 3);
  listInsert(La, 2, 11);
  listInsert(La, 3, 8);

  listInsert(Lb, 0, 20);
  listInsert(Lb, 1, 6);
  listInsert(Lb, 2, 15);
  listInsert(Lb, 3, 8);
  listInsert(Lb, 4, 2);
  listInsert(Lb, 5, 9);
  listInsert(Lb, 6, 11);
}
int main() {
  SqList La, Lb, Lc;
  createList(&La, &Lb, &Lc);
  //   printInt(Lb.length - 1);
  quickSort(&La, 0, La.length - 1);
  quickSort(&Lb, 0, Lb.length - 1);
  listTraverse(La, "traverse La");
  listTraverse(Lb, "traverse Lb");
  mergeList(La, Lb, &Lc);
  listTraverse(Lc, "traverse Lc");
  int back;
  listDelete(&Lc, 3, &back);
  listTraverse(Lc, "delete third element");
  printInt(back);
  unionList(&La, Lb);

  return 0;
}