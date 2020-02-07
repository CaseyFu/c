#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * 银行离散模拟用户逗留平均时间- -!
 */

/* 链表基本操作 */
typedef struct Event {
  int occurTime;  // 发生时刻
  int type;       // 0表示事件到达,1表示1号窗口离开事件
  struct Event* next;
} Event;
typedef struct EventList {
  Event* head;
  Event* tail;
  int length;
} EventList;

void initList(EventList* L) {
  L->head = (Event*)malloc(sizeof(Event));
  if (!L->head)
    return;
  L->head->next = NULL;
  L->length = 0;
  L->tail = L->head;
}
void destroyList(EventList* L) {
  while (L->head->next) {
    Event* p = L->head->next;
    L->head->next = p->next;
    free(p);
  }
  free(L->head);
  free(L);
}
/**
 * 插入保证链表升序
 */
void listInsert(EventList* L, Event* e) {
  Event* p = L->head;
  while (p) {
    if (p->next == NULL) {
      e->next = p->next;
      p->next = e;
      L->tail = e;
      break;
    } else if (p->next->occurTime >= e->occurTime) {
      e->next = p->next;
      p->next = e;
      break;
    }
    p = p->next;
  }
  L->length++;
}
/**
 * 删除第i个位置
 */
void listDelete(EventList* L, int i, Event* e) {
  if (i < 0 || i > L->length - 1)
    return;
  Event* p = L->head;
  for (int j = 0; j < i; j++) {
    p = p->next;
  }
  Event* q = p->next;
  e->occurTime = q->occurTime;
  e->type = q->type;
  p->next = q->next;
  if (p->next) {
    free(q);
  } else {
    L->tail = p;
  }
  L->length--;
}
void traverseList(EventList* L) {
  Event* p = L->head->next;
  printf("链表头:\n");
  while (p) {
    printf("<occurTime,type> <%d,%d>\n", p->occurTime, p->type);
    p = p->next;
  }
  printf("链表尾\n");
}
Event* newEvent(int occurTime, int type) {
  Event* event = (Event*)malloc(sizeof(Event));
  event->occurTime = occurTime;
  event->type = type;
  event->next = NULL;
  return event;
}
/* 队列基本操作 */
typedef struct QEvent {
  int arrivalTime;
  int duration;
  struct QEvent* next;
} QEvent;
typedef struct SqQueue {
  QEvent* front;
  QEvent* rear;
  int length;
} Queue;
void initQueue(Queue* queue) {
  queue->front = (QEvent*)malloc(sizeof(QEvent));
  if (!queue->front)
    return;
  queue->front->next = NULL;
  queue->rear = queue->front;
  queue->length = 0;
}
void destroyQueue(Queue* queue) {
  while (queue->front->next) {
    QEvent* p = queue->front->next;
    queue->front->next = p->next;
    free(p);
  }
  free(queue->front);
  free(queue);
}
int queueLength(Queue* queue) {
  return queue->length;
}
int isEmpty(Queue* queue) {
  return queueLength(queue) ? 0 : 1;
}
void enQueue(Queue* queue, QEvent* qEvent) {
  qEvent->next = queue->rear->next;
  queue->rear->next = qEvent;
  queue->rear = qEvent;
  queue->length++;
}
void deQueue(Queue* queue, QEvent* e) {
  if (isEmpty(queue))
    return;
  QEvent* p = queue->front->next;
  e->arrivalTime = p->arrivalTime;
  e->duration = p->duration;
  e->next = NULL;
  if (p->next) {
    queue->front->next = p->next;
  } else {
    queue->front->next = NULL;
    queue->rear = queue->front;
  }
  free(p);
  queue->length--;
}
QEvent* getFront(Queue* queue) {
  if (isEmpty(queue))
    return NULL;
  return queue->front->next;
}
QEvent* newQEvent(int occurTime, int duration) {
  QEvent* p = (QEvent*)malloc(sizeof(QEvent));
  if (!p)
    return NULL;
  p->duration = duration;
  p->arrivalTime = occurTime;
  p->next = NULL;
  return p;
}
void traverseQueue(Queue* queue) {
  QEvent* p = queue->front->next;
  printf("队头:\n");
  while (p) {
    printf("<arrivalTime,duration> <%d,%d>\n", p->arrivalTime, p->duration);
    p = p->next;
  }
  printf("队尾\n");
}

/* 银行业务 */
EventList* eventList;
Event* event;
Queue* queue[5];
QEvent customer;  // <duration, arrivalTime>
int totalTime;    // 总耗时
int customerNum;  // 顾客数
int closeTime = 10;
int t;
int i;

/**
 * 初始化
 */
void openForDay() {
  totalTime = 0;
  customerNum = 0;
  eventList = (EventList*)malloc(sizeof(EventList));
  if (!eventList)
    return;
  initList(eventList);
  for (int i = 1; i < 5; i++) {
    queue[i] = (Queue*)malloc(sizeof(Queue));
    if (!queue[i])
      return;
    initQueue(queue[i]);
  }
  event = (Event*)malloc(sizeof(Event));
  if (!event)
    return;
  event->occurTime = 0;
  event->type = 0;
  listInsert(eventList, event);
}
/**
 * 返回queue[4]最短的length的Index
 */
int minIndex() {
  int index = 1;
  int minLength = INT_MAX;
  for (int i = 1; i < 5; i++) {
    if (queue[i]->length < minLength) {
      minLength = queue[i]->length;
      index = i;
    }
  }
  return index;
}
void geRandom(int* duration, int* interTime) {
  // duration范围在0<duration<=30
  // 0<interTime<=5

  *duration = (rand() % 30) + 1;
  *interTime = (rand() % 5) + 1;
  printf("生成随机数<%d,%d>\n", *duration, *interTime);
}
void customerArrived() {
  customerNum++;
  int duration = 0, interTime = 0;
  geRandom(&duration, &interTime);
  t = event->occurTime + interTime;
  if (t < closeTime) {
    listInsert(eventList, newEvent(t, 0));
  }
  i = minIndex();
  enQueue(queue[i], newQEvent(event->occurTime, duration));
  if (queueLength(queue[i]) == 1) {
    listInsert(eventList, newEvent(event->occurTime + duration, i));
  }
  traverseList(eventList);
}

void customerDeparture() {
  i = event->type;
  deQueue(queue[i], &customer);
  totalTime += event->occurTime - customer.arrivalTime;
  if (!isEmpty(queue[i])) {
    QEvent* p = getFront(queue[i]);
    customer.arrivalTime = p->arrivalTime;
    customer.duration = p->duration;
    listInsert(eventList, newEvent(event->occurTime + customer.arrivalTime, i));
  }
  traverseList(eventList);
}

/**
 * 关门, 计算平均值
 */
void closeForDay() {
  printf("平均时间:%.2f", (float)totalTime / customerNum);
  for (int i = 1; i < 5; i++) {
    destroyQueue(queue[i]);
  }
  //   free(event);
  destroyList(eventList);
}

/* test */
void createList() {
  EventList* L = (EventList*)malloc(sizeof(EventList));
  if (!L)
    return;
  initList(L);
  for (int i = 0; i < 5; i++) {
    listInsert(L, newEvent(i, 0));
  }
  traverseList(L);
  listInsert(L, newEvent(4, 1));
  traverseList(L);
  printf("%d", L->length);
  destroyList(L);
}
void createQueue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  if (!queue)
    return;
  initQueue(queue);
  for (int i = 0; i < 5; i++) {
    enQueue(queue, newQEvent(0, i));
  }
  QEvent e;
  deQueue(queue, &e);
  printf("%d\n", queueLength(queue));
  traverseQueue(queue);
}
int main() {
  srand(time(NULL));
  openForDay();  // 开业
  while (eventList->length != 0) {
    Event e;
    listDelete(eventList, 0, &e);
    event = &e;
    if (event->type == 0) {
      customerArrived();
    } else {
      customerDeparture();
    }
  }
  closeForDay();
  return 0;
}