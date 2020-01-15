#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\custom.h"
#include "..\test.c"

/**
 * 静态链表,使用index的相对位置作为指针,方便在不设指针类型的程序中使用链表结构
 * 预先需要分配较大的数组空间
 * 说白了就是链表,只不过是片连续的地址空间
 */
int LISTSIZE = 20;
typedef int ElemType;
typedef struct SLinkList {
  ElemType data;
  int cur;
} SLinkList;

/**
 * 初始化
 */
void initSLinkList(SLinkList *L) {
  // L[0]->data保存未分配数组的长度,不包括第一个和最后一个
  for (int i = 0; i < LISTSIZE - 1; i++) {
    L[i].cur = i + 1;
  }
  L[0].data = LISTSIZE - 2;
  // L[LISTSIZE].cur分配了的结点的首地址,L[LISTSIZE].data保存分配了的长度
  L[LISTSIZE - 1].cur = 0;
  L[LISTSIZE - 1].data = 0;
}

/**
 * 遍历L
 */
void traverseSLinkList(SLinkList *L) {
  if (L[LISTSIZE - 1].data == 0) return;
  int i = L[LISTSIZE - 1].cur;
  while (i != 0) {
    printf("%d\t", L[i].data);
    i = L[i].cur;  // 类似于p=p->next
  }
  printf("\n");
}

/**
 * 在第i个位置插入e
 */
int listInsert(SLinkList *L, int i, ElemType e) {
  if (L[0].data == 0) {
    // L[0].data==0表明未分配的空间已经用完,重新分配一个空间
    printf("空间不足");
    return 0;
  }
  if (i > L[LISTSIZE - 1].data + 1) {
    printf("位置i不合法");
    return 0;
  }
  // 在未分配空间的L拿出一个空间来, 就拿第一个
  int index = L[0].cur;
  L[0].cur = L[index].cur;

  L[index].data = e;  // 为这个空间赋值

  // 找已分配链表的第i个位置
  int k = LISTSIZE - 1;
  for (int j = 0; j < i; j++) {
    k = L[k].cur;
  }
  L[index].cur = L[k].cur;  // 连接
  L[k].cur = index;

  L[0].data--;             // 用掉一个结点
  L[LISTSIZE - 1].data++;  // 增加一个分配了的结点
  return 1;
}

/**
 * 删除一个元素
 */
int listDelete(SLinkList *L, int i, ElemType *e) {
  if (L[LISTSIZE - 1].data == 0) return 0;
  // 找到第i个位置
  int k = LISTSIZE - 1;
  for (int j = 0; j < i; j++) {
    k = L[k].cur;
  }
  int index = L[k].cur;
  L[k].cur = L[index].cur;

  *e = L[index].data;  // 赋值

  // 加入空闲的链表
  L[index].cur = L[0].cur;
  L[0].cur = index;

  L[0].data++;             // 增加一个未分配结点
  L[LISTSIZE - 1].data--;  // 减少一个分配了的结点
  return 1;
}

/**
 * (A-B)V(B-A)
 * 算法思想:循环每个B,如果A中有的就删除A中的,A中没有就插入
 * 用La指针返回结果
 */
void A_BVB_A(SLinkList *La, SLinkList *Lb) {
  for (int i = Lb[LISTSIZE - 1].cur; i != 0; i = Lb[i].cur) {
    ElemType key = Lb[i].data;
    int index = La[LISTSIZE - 1].cur;

    for (int j = 0; j < La[LISTSIZE - 1].data; j++) {
      if (La[index].data == key) {
        // A中有,删除
        int e;
        listDelete(La, j, &e);
        printf("删除%d\t", e);
        break;
      }
      index = La[index].cur;
    }
    if (index == 0) {
      // A中没有
      listInsert(La, 0, key);
    }
  }
}

/**
 * 测试
 */
void createList(SLinkList *La, SLinkList *Lb) {
  initSLinkList(La);
  initSLinkList(Lb);
  for (int i = 0; i < 8; i++) {
    listInsert(La, 0, i);
  }
  for (int i = 4; i < 12; i++) {
    listInsert(Lb, 0, i);
  }
}
int main() {
  SLinkList *La = (SLinkList *)malloc(sizeof(SLinkList) * LISTSIZE);
  SLinkList *Lb = (SLinkList *)malloc(sizeof(SLinkList) * LISTSIZE);
  createList(La, Lb);
  printf("空间总长度%d\n", LISTSIZE);
  printf("La已用空间长度%d\n", La[LISTSIZE - 1].data);
  printf("La未用空间长度%d\n", La[0].data);
  printf("Lb已用空间长度%d\n", Lb[LISTSIZE - 1].data);
  printf("Lb未用空间长度%d\n", Lb[0].data);
  traverseSLinkList(La);

  traverseSLinkList(Lb);
  A_BVB_A(La, Lb);

  traverseSLinkList(La);

  traverseSLinkList(Lb);

  return 0;
}