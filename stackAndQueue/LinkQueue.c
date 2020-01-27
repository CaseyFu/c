#include <stdio.h>
#include <stdlib.h>

/**
 * 链式队列
 */

typedef int ElemType;
typedef struct Node {
  ElemType data;
  struct Node* next;
} Node;
typedef struct LinkQueue {
  Node* front;
  Node* rear;
  int length;
} LinkQueue;

void initQueue(LinkQueue* queue) {
  Node* p = (Node*)malloc(sizeof(Node));
  if (!p)
    return;
  p->next = NULL;
  queue->front = p;
  queue->rear = p;
  queue->length = 0;
}
int isEmpty(LinkQueue* queue) {
  return queue->length ? 0 : 1;
}

int length(LinkQueue* queue) {
  return queue->length;
}
Node* getHead(LinkQueue* queue) {
  if (queue == NULL || queue->front->next == NULL)
    return NULL;
  return queue->rear;
}
/**
 * 入队
 */
void enQueue(LinkQueue* queue, ElemType e) {
  Node* p = (Node*)malloc(sizeof(Node));
  if (!p)
    return;
  p->data = e;
  p->next = queue->rear->next;
  queue->rear->next = p;
  queue->rear = p;
  queue->length++;
}
/**
 * 出队
 */
void deQueue(LinkQueue* queue, Node* e) {
  if (isEmpty(queue))
    return;
  Node* p = queue->front->next;
  if (p->next != NULL) {
    queue->front->next = p->next;
  } else {
    queue->front->next = NULL;
  }
  e->data = p->data;
  e->next = NULL;
  queue->length--;
  free(p);
}
void traverse(LinkQueue* queue) {
  if (isEmpty(queue)) {
    printf("队列为空\n");
    return;
  }
  printf("队列头:\t");
  Node* p = queue->front->next;
  while (p) {
    printf("%d\t", p->data);
    p = p->next;
  }
  printf("队列尾\n");
}
void destroyQueue(LinkQueue* queue) {
  Node e;
  while (!isEmpty(queue)) {
    deQueue(queue, &e);
  }
  free(queue->front);
  free(queue);
}
void clearQueue(LinkQueue* queue) {
  Node e;
  while (!isEmpty(queue)) {
    deQueue(queue, &e);
  }
}
void createQueue(LinkQueue* queue) {
  initQueue(queue);
  for (int i = 0; i < 3; i++) {
    enQueue(queue, i);
  }
}
int main() {
  LinkQueue* queue = (LinkQueue*)malloc(sizeof(LinkQueue));
  if (!queue)
    return 0;
  createQueue(queue);

  traverse(queue);

  Node e;
  deQueue(queue, &e);
  deQueue(queue, &e);
  deQueue(queue, &e);
  deQueue(queue, &e);
  traverse(queue);
  clearQueue(queue);
  destroyQueue(queue);
  return 0;
}