#include <stdio.h>
#include <stdlib.h>

/**
 * 顺序循环队列
 */
#define INITSIZE 50
#define INCREMENTSIZE 10
typedef int ElemType;
typedef struct SqQueue {
  ElemType* base;
  int front;
  int rear;
  int length;  // 当前已用大小
  int size;    // 总分配大小
} SqQueue;
void initQueue(SqQueue* queue) {
  queue->base = (ElemType*)malloc(sizeof(ElemType) * INITSIZE);
  if (!queue->base)
    return;
  queue->front = 0;
  queue->rear = 0;
  queue->size = INITSIZE;
}
void destroyQueue(SqQueue* queue) {
  free(queue->base);
  free(queue);
}
int length(SqQueue* queue) {
  return (queue->rear - queue->front + INITSIZE) % INITSIZE;
}
int isEmpty(SqQueue* queue) {
  return length(queue) ? 0 : 1;
}
/**
 * 对头节点,front节点的地址
 */
ElemType* getHead(SqQueue* queue) {
  return queue->base + queue->front;
}
/**
 * 入队,rear节点
 */
void enQueue(SqQueue* queue, ElemType e) {
  if ((queue->rear + 1) % queue->size == queue->front) {
    // 扩容
    ElemType* newQueue = (ElemType*)realloc(
        queue->base, sizeof(ElemType) * (queue->size + INCREMENTSIZE));
    if (!newQueue)
      return;
    queue->base = newQueue;
    queue->size += INCREMENTSIZE;
  }
  queue->base[queue->rear] = e;
  queue->rear = (queue->rear + 1) % queue->size;
}
/**
 * 出队,front节点
 */
void deQueue(SqQueue* queue, ElemType* e) {
  *e = queue->base[queue->front];
  queue->front = (queue->front + 1) % queue->size;
}
void traverse(SqQueue* queue) {
  int p = queue->front;
  printf("队列头\t");
  while (p != queue->rear) {
    printf("%d\t", queue->base[p]);
    p = (p + 1) % queue->size;
  }
  printf("队列尾\n");
}
void createQueue(SqQueue* queue) {
  initQueue(queue);
  for (int i = 0; i < 9; i++) {
    enQueue(queue, i);
  }
}
int main() {
  SqQueue* queue = (SqQueue*)malloc(sizeof(SqQueue));
  if (!queue)
    return 0;
  createQueue(queue);
  traverse(queue);
  ElemType e;
  deQueue(queue, &e);
  deQueue(queue, &e);
  deQueue(queue, &e);
  printf("%d\n", e);
  printf("length=%d\n", length(queue));
  printf("size=%d\n", queue->size);
  traverse(queue);
  destroyQueue(queue);
  return 0;
}