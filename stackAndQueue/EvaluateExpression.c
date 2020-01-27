#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * 顺序栈
 */

#define STACK_INIT_SIZE 50
#define STACKINCREMENT 10

typedef char* ElemType;
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
ElemType getTop(SqStack* stack) {
  if (isEmpty(stack))
    return NULL;
  return *(stack->top - 1);
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
  return 1;
}

/**
 * 遍历栈表,即出栈
 */
void traverseStack(SqStack* stack) {
  ElemType* p = stack->top - 1;
  printf("遍历栈表,输出出栈的顺序\n");
  while (1) {
    printf("%s\t", *p);
    if (p == stack->base)
      break;
    p--;
  }
  printf("\n");
}

/**
 * 表达式求值
 * ElemType = char*;
 * ASCII :'+'43,'-'45,'*'42,'/'47,'#'35
 */
/**
 * 连续出现的算符x和y满足的优先级表格 + - * / ( ) #
 */
char percede(char x, char y) {
  int i, j;
  if (x == '+') {
    i = 0;
  } else if (x == '-') {
    i = 1;
  } else if (x == '*') {
    i = 2;
  } else if (x == '/') {
    i = 3;
  } else if (x == '(') {
    i = 4;
  } else if (x == ')') {
    i = 5;
  } else if (x == '#') {
    i = 6;
  }
  if (y == '+') {
    j = 0;
  } else if (y == '-') {
    j = 1;
  } else if (y == '*') {
    j = 2;
  } else if (y == '/') {
    j = 3;
  } else if (y == '(') {
    j = 4;
  } else if (y == ')') {
    j = 5;
  } else if (y == '#') {
    j = 6;
  }
  char table[7][7] = {
      {'>', '>', '<', '<', '<', '>', '>'}, {'>', '>', '<', '<', '<', '>', '>'},
      {'>', '>', '>', '>', '<', '>', '>'}, {'>', '>', '>', '>', '<', '>', '>'},
      {'<', '<', '<', '<', '<', '=', ' '}, {'>', '>', '>', '>', ' ', '>', '>'},
      {'<', '<', '<', '<', '<', ' ', '='},
  };
  return table[i][j];
}
/**
 * 计算字符串b,a关于运算符号theta的结果
 * b-a, b+a, b*a, b/a
 */
ElemType calculate(ElemType b, ElemType theta, ElemType a) {
  double double_b = atof(b);
  double double_a = atof(a);
  double result;
  if (strcmp(theta, "+") == 0) {
    result = double_b + double_a;
  } else if (strcmp(theta, "-") == 0) {
    result = double_b - double_a;
  } else if (strcmp(theta, "*") == 0) {
    result = double_b * double_a;
  } else if (strcmp(theta, "/") == 0) {
    result = double_b / double_a;
  }
  ElemType buffer = (ElemType)malloc(sizeof(char));
  sprintf(buffer, "%.2f", result);
  return buffer;
}

void evaluateExpression() {
  char* expression[] = {"4", "+", "2", "*", "3", "-", "10", "/", "5", "#"};
  int length = sizeof(expression) / sizeof(expression[0]);
  SqStack* operator=(SqStack*) malloc(sizeof(SqStack));  // 操作符
  SqStack* operand = (SqStack*)malloc(sizeof(SqStack));  // 操作数
  initStack(operator);
  initStack(operand);
  push(operator, "#");  // 先push一个#，表达式中遇到#结束表达式
  for (int i = 0; i < length; i++) {
    char e = **(expression + i);
    if (e == '+' || e == '-' || e == '*' || e == '/' || e == '(' || e == ')' ||
        e == '#') {
      ElemType temp = getTop(operator);
      switch (percede(*temp, e)) {  // 确定栈顶和当前循环操作符的优先级
        case ('<'): {
          // 栈顶符号优先级低
          push(operator, *(expression + i));
          break;
        }
        case ('='): {
          // 优先级相等,是括号pop
          ElemType kkk;
          pop(operator, & kkk);
          break;
        }
        case ('>'): {
          // 栈顶符号优先级大于当前符号，符号出栈运算，操作数结果入栈
          ElemType theta;
          pop(operator, & theta);
          ElemType a, b;
          pop(operand, &a);
          pop(operand, &b);
          ElemType result = calculate(b, theta, a);
          push(operand, result);
          i--;  // 重要,
                // 栈顶操作符优先级>当前操作符执行操作数的运算,这一次的循环i值就不应该改变
        }
      }
    } else {
      // 数字,直接push
      push(operand, *(expression + i));
    }
  }
  ElemType result = getTop(operand);
  printf("%s\n", result);
}
int main() {
  evaluateExpression();
  return 0;
}