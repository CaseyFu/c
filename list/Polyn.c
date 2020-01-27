#include <stdio.h>
#include <stdlib.h>
/**
 * 多项式,多项式相加、相减、相乘
 */
typedef struct List {
  int length;
  struct Node* head;
  struct Node* tail;
} List;
typedef struct Node {
  double coefficient;  // 系数
  int exponent;        // 指数
  struct Node* next;
} Node;

void appendNodeAddress(List* L, Node* node);
void traversePolynomial(List* L, char* message);

/**
 * 初始化List
 */
void initPolynomial(List* L) {
  L->head = (Node*)malloc(sizeof(Node));
  if (!L->head)
    return;
  L->head->next = NULL;
  L->tail = L->head;
  L->length = 0;
}
void destroyPolynomial(List* L) {
  while (L->head->next) {
    Node* p = L->head->next;
    L->head->next = p->next;
    free(p);
  }
  free(L->head);
}

/**
 * 两个多项式相加结果放在Lc,制空La,Lb,相当于就是合并链表
 */
void addPolynomial(List* La, List* Lb, List* Lc) {
  Node* p = La->head->next;
  Node* q = Lb->head->next;
  Node* r = Lc->head;
  // 第一个节点
  while (p && q) {
    if (p->exponent > q->exponent) {
      if (Lc->head->next == p) {
        // 第一次选择第一个节点
        Lc->head->next = p;
      } else {
        r->next = p;
      }
      r = p;
      p = p->next;
      Lc->length++;
    } else if (p->exponent < q->exponent) {
      if (Lc->head->next == q) {
        Lc->head->next = q;
      } else {
        r->next = q;
      }
      r = q;
      q = q->next;
      Lc->length++;
    } else {
      // 等于,系数相加
      if (p->coefficient + q->coefficient == 0) {
        // 系数为0抵消, 双杀
        Node* tp = p;
        Node* tq = q;
        p = p->next;
        q = q->next;
        free(tp);
        free(tq);
      } else {
        // 系数不为0,q加到p,p放入Lc
        p->coefficient = p->coefficient + q->coefficient;
        r->next = p;
        r = p;
        p = p->next;
        Node* tq = q;
        q = q->next;
        free(tq);
        Lc->length++;
      }
    }
  }
  while (p) {
    r->next = p;
    r = p;
    p = p->next;
    Lc->length++;
  }
  while (q) {
    r->next = q;
    r = q;
    q = q->next;
    Lc->length++;
  }
  La->head->next = NULL;
  La->length = 0;
  Lb->head->next = NULL;
  Lb->length = 0;
}
/**
 * 迭代,La与Lb相加的结果放在Lc,addPolynomialIteration(Lc,Lb,Lc);
 * 两个多项式相加结果放在Lc,制空La,Lb,相当于就是合并链表
 */
void addPolynomialIteration(List* La, List* Lb, List* Lc) {
  Node* p = La->head->next;
  Node* q = Lb->head->next;
  Node* r = Lc->head;
  Lc->length = 0;
  // 第一个节点
  while (p && q) {
    if (p->exponent > q->exponent) {
      if (Lc->head->next == p) {
        // 第一次选择第一个节点
        Lc->head->next = p;
      } else {
        r->next = p;
      }
      r = p;
      p = p->next;
      Lc->length++;
    } else if (p->exponent < q->exponent) {
      if (Lc->head->next == q) {
        Lc->head->next = q;
      } else {
        r->next = q;
      }
      r = q;
      q = q->next;
      Lc->length++;
    } else {
      // 等于,系数相加
      if (p->coefficient + q->coefficient == 0) {
        // 系数为0抵消, 双杀
        Node* tp = p;
        Node* tq = q;
        p = p->next;
        q = q->next;
        free(tp);
        free(tq);
      } else {
        // 系数不为0,q加到p,p放入Lc
        p->coefficient = p->coefficient + q->coefficient;
        r->next = p;
        r = p;
        p = p->next;
        Node* tq = q;
        q = q->next;
        free(tq);
        Lc->length++;
      }
    }
  }
  while (p) {
    r->next = p;
    r = p;
    p = p->next;
    Lc->length++;
  }
  while (q) {
    r->next = q;
    r = q;
    q = q->next;
    Lc->length++;
  }
  Lb->head->next = NULL;
  Lb->tail = Lb->head;
  Lb->length = 0;
}
/**
 * La-Lb,结果放在Lc,制空La,Lb,先把每个Lb的系数*(-1)然后执行一次La+Lb
 */
void subtractPolynomial(List* La, List* Lb, List* Lc) {
  Node* p = Lb->head->next;
  while (p) {
    p->coefficient = p->coefficient * (-1);
    p = p->next;
  }
  addPolynomial(La, Lb, Lc);
}
/**
 * 两个多项式相乘结果放在Lc,制空La,Lb
 */
void multiplyPolynomial(List* La, List* Lb, List* Lc) {
  if (La->length == 0 || Lb->length == 0)
    return;
  // 先把Lb的首元结点乘以La的每个结点,放在Lc中

  Node* p = Lb->head->next;
  Node* q = La->head->next;
  while (q) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
      return;
    node->exponent = p->exponent + q->exponent;
    node->coefficient = p->coefficient * q->coefficient;
    appendNodeAddress(Lc, node);
    q = q->next;
  }
  p = p->next;
  List* Ld = (List*)malloc(sizeof(Node));
  if (!Ld)
    return;
  initPolynomial(Ld);
  while (p) {
    q = La->head->next;
    while (q) {
      Node* node1 = (Node*)malloc(sizeof(Node));
      if (!node1)
        return;
      node1->exponent = p->exponent + q->exponent;
      node1->coefficient = p->coefficient * q->coefficient;
      appendNodeAddress(Ld, node1);
      q = q->next;
    }
    // 合并
    addPolynomialIteration(Lc, Ld, Lc);
    p = p->next;
  }
  destroyPolynomial(Ld);
}

/**
 * 追加结点
 */
void appendNode(List* L, double coefficient, int exponent) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->coefficient = coefficient;
  node->exponent = exponent;
  node->next = L->tail->next;
  L->tail->next = node;
  L->tail = node;
  L->length++;
}
/**
 * 追加结点
 */
void appendNodeAddress(List* L, Node* node) {
  node->next = L->tail->next;
  L->tail->next = node;
  L->tail = node;
  L->length++;
}
/**
 * 创建多项式
 */
void createPolynomial(List* La, List* Lb, List* Lc) {
  initPolynomial(La);
  appendNode(La, 1, 5);
  appendNode(La, 1, 3);
  appendNode(La, 1, 1);

  initPolynomial(Lb);
  appendNode(Lb, 2, 3);
  appendNode(Lb, -1, 2);
  appendNode(Lb, 1, 1);
  //   appendNode(Lb, -1, 1);
  initPolynomial(Lc);
}
void traversePolynomial(List* L, char* message) {
  if (!L || !L->head || !L->head->next) {
    printf("链表为空\n");
    return;
  }
  Node* p = L->head->next;
  printf("%s:f(x) = ", message);
  while (p) {
    if (p == L->head->next) {
      printf("%.2fX^%d", p->coefficient, p->exponent);
    } else if (p->coefficient < 0)
      printf(" - %.2fX^%d", (-1) * p->coefficient, p->exponent);
    else if (p->coefficient > 0) {
      printf(" + %.2fX^%d", p->coefficient, p->exponent);
    }
    p = p->next;
  }
  printf("\n");
}
int main() {
  List* La = (List*)malloc(sizeof(List));
  List* Lb = (List*)malloc(sizeof(List));
  List* Lc = (List*)malloc(sizeof(List));
  createPolynomial(La, Lb, Lc);
  //   addPolynomial(La, Lb, Lc);
  //   subtractPolynomial(La, Lb, Lc);
  multiplyPolynomial(La, Lb, Lc);
  traversePolynomial(La, "La");
  traversePolynomial(Lb, "Lb");
  traversePolynomial(Lc, "Lc");
  printf("Lc长度:%d", Lc->length);
  return 0;
}