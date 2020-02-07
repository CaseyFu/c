#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * 顺序栈
 */

#define STACK_INIT_SIZE 50
#define STACKINCREMENT 10

typedef char ElemType;
typedef struct Stack {
  ElemType* top;
  ElemType* base;
  int length;  // 已分配空间
  int size;    // 总分配空间
} SqStack;

void initStack(SqStack* stack) {
  ElemType* p = (ElemType*)malloc(sizeof(ElemType) * STACK_INIT_SIZE);
  if (!p)
    return;
  stack->base = p;
  stack->top = stack->base;
  stack->length = 0;
  stack->size = STACK_INIT_SIZE;
}

void clearStack(SqStack* stack) {
  stack->top = stack->base;
}

void destroyStack(SqStack* stack) {
  free(stack->base);
  free(stack);
}

/**
 * 栈是空则返回true,1
 */
int isEmpty(SqStack* stack) {
  return stack->top == stack->base ? 1 : 0;
}

/**
 * 返回栈总分配空间
 */
int getSize(SqStack* stack) {
  printf("size:%d\n", stack->size);
  return stack->size;
}

/**
 * 返回栈当前已分配的长度
 */
int getLength(SqStack* stack) {
  printf("length:%d\n", stack->length);
  return stack->length;
}

// *(stack->top) ~ stack->top[0]指针指向的当前位置取值
// *(stack->top+1) ~ stack->top[1]指针向后移一位,再取值
// (*stack->top)+1 指针指向的当前位置取值后值+1

// c语言右结合
// *stack->top-- ~ *stack->top,stack->top--;先取值,指针再前移
// *--stack->top ~ stack->top-1,*stack->top;先前移,指针再取值
// stack->top-- ~ stack->top = stack->top - 1;

/**
 * 返回栈顶元素,不删除值
 */
int getTop(SqStack* stack, ElemType* e) {
  if (isEmpty(stack))
    return -1;
  *e = *(stack->top - 1);
}

/**
 * 入栈操作
 */
void push(SqStack* stack, ElemType e) {
  // 判断空间
  if (stack->length == stack->size) {
    ElemType* p = (ElemType*)realloc(
        stack->base, sizeof(ElemType) * (stack->size + STACKINCREMENT));
    if (!p)
      return;
    stack->base = p;
    stack->top = stack->base + stack->size;
    stack->size += STACKINCREMENT;
  }
  *stack->top++ = e;
  stack->length++;
}

/**
 * 返回栈顶元素,删除值
 */
int pop(SqStack* stack, ElemType* e) {
  if (isEmpty(stack))
    return -1;
  *e = *--stack->top;
  stack->length--;
}

/**
 * 遍历栈表,即出栈
 */
void traverseStack(SqStack* stack) {
  ElemType* p = stack->top - 1;
  printf("遍历栈表,输出出栈的顺序\n");
  while (1) {
    printf("%d\t", *p);
    if (p == stack->base)
      break;
    p--;
  }
  printf("\n");
}

void createStack(SqStack* stack) {
  initStack(stack);
  for (int i = 0; i < 100; i++) {
    push(stack, i);
  }
}

// 栈的应用
/**
 * 数制转换,输入一个十进制数,转换为2、8、16进制
 */
void conversion(long long num) {
  SqStack* stack = (SqStack*)malloc(sizeof(SqStack));
  initStack(stack);
  ElemType e;
  printf("十进制%d===>\n", num);
  int lt0 = 0;
  if (num < 0) {
    num *= -1;
    lt0 = 1;
  }
  // 转换为二进制
  int divisor = num;
  while (1) {
    push(stack, divisor % 2);
    divisor /= 2;  // 右移1位
    if (divisor == 0)
      break;
  }
  printf("二进制:");
  if (lt0)
    printf("-");
  while (!isEmpty(stack)) {
    pop(stack, &e);
    printf("%d", e);
  }
  printf("\n");
  // 转换为八进制
  divisor = num;
  while (1) {
    push(stack, divisor % 8);
    divisor /= 8;  // 右移3位
    if (divisor == 0)
      break;
  }
  printf("八进制:");
  if (lt0)
    printf("-");
  while (!isEmpty(stack)) {
    pop(stack, &e);
    printf("%d", e);
  }
  printf("\n");
  // 转换为十六进制
  divisor = num;
  while (1) {
    push(stack, divisor % 16);
    divisor /= 16;  // 右移4位,除以16
    if (divisor == 0)
      break;
  }
  printf("十六进制:");
  if (lt0)
    printf("-");
  while (!isEmpty(stack)) {
    pop(stack, &e);
    if (e >= 0 && e <= 9) {
      printf("%d", e);
    } else {
      switch (e) {
        case 10:
          printf("A");
          break;
        case 11:
          printf("B");
          break;
        case 12:
          printf("C");
          break;
        case 13:
          printf("D");
          break;
        case 14:
          printf("E");
          break;
        case 15:
          printf("F");
          break;
      }
    }
  }
  printf("\n");
  destroyStack(stack);
}

/**
 * 括号匹配
 * ASCII ()40,41; {}123,125、[]91,93
 * 改ElemType为char
 */
void bracketMatch() {
  SqStack* stack = (SqStack*)malloc(sizeof(SqStack));
  initStack(stack);
  ElemType e;
  char* bracket = "([[{{}{}}{}]])";

  int len = strlen(bracket);
  for (int i = 0; i < len; i++) {
    if (getTop(stack, &e) != -1) {
      // 栈不为空时得到栈顶元素,根据ASCII码,要么相差1要么相差2
      if (e == *(bracket + i) - 1 || e == *(bracket + i) - 2) {
        // 相等,出栈
        pop(stack, &e);
      } else {
        // 不等,入栈
        push(stack, *(bracket + i));
      }
    } else {
      // 栈为空,直接入栈
      push(stack, *(bracket + i));
    }
  }
  if (isEmpty(stack)) {
    // 栈为空,匹配
    printf("括号相匹配");
  } else {
    printf("括号不匹配");
  }
  destroyStack(stack);
}

/**
 * 行编辑,#使前一个字符无效,@使整行无效
 * 改ElemType为char
 */
void lineEdit() {
  SqStack* stack = (SqStack*)malloc(sizeof(SqStack));
  initStack(stack);
  ElemType e;
  char* line = "abcdefg##hijk@123#whli##ile";
  for (int i = 0; i < strlen(line); i++) {
    if (*(line + i) == '#') {
      pop(stack, &e);
    } else if (*(line + i) == '@') {
      clearStack(stack);
    } else {
      push(stack, *(line + i));
    }
  }
  while (!isEmpty(stack)) {
    pop(stack, &e);
    printf("%c", e);
  }
  destroyStack(stack);
}

/**
 * 汉诺塔递归
 */
void move(char a, char c) {
  printf("%c--->%c\n", a, c);
}
void hanoi(int n, char a, char b, char c) {
  if (n < 3) {
    hanoi(n + 1, a, c, b);
    move(a, c);
    hanoi(n + 1, b, a, c);
  }
}

int main() {
  //   hanoi(0, 'A', 'B', 'C');
  conversion(-15);
  return 0;
}