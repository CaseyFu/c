#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PosType {
  int x;
  int y;
} PosType;  // positionType

typedef struct MazeNode {
  struct PosType position;  // 通道块的坐标(x,y)
  int direction;            // 下一通道块的方向0右,1下,2左,3上
  struct MazeNode* next;
} MazeNode;

typedef struct Stack {
  MazeNode* top;
} Stack;

int row = 10;     // 地图的行
int column = 10;  // 地图的列

Stack* initStack() {
  Stack* stack = (Stack*)malloc(sizeof(Stack));
  if (!stack)
    return NULL;
  stack->top = (MazeNode*)malloc(sizeof(MazeNode));
  if (!stack->top)
    return NULL;
  stack->top->next = NULL;
  return stack;
}

void push(Stack* stack, MazeNode* e) {
  MazeNode* node = (MazeNode*)malloc(sizeof(MazeNode));
  if (!node)
    return;
  e->next = stack->top->next;
  stack->top->next = e;
}
/**
 * 返回栈顶元素地址
 */
MazeNode* getTop(Stack* stack) {
  if (stack->top->next == NULL)
    return NULL;
  return stack->top->next;
}
void pop(Stack* stack, MazeNode* node) {
  if (stack->top->next == NULL)
    return;
  MazeNode* p = stack->top->next;
  node->position.x =
      p->position.x;  // 不用node->seat = p->seat,因为后面会free(p);
  node->position.y = p->position.y;
  node->direction = p->direction;
  stack->top->next = p->next;
  free(p);
}
int isEmpty(Stack* stack) {
  return stack->top->next ? 0 : 1;
}
void traverseStack(Stack* stack) {
  MazeNode* p = stack->top->next;
  printf("top\n");
  while (p) {
    printf("direction:%d\t(%d,%d)\n", p->direction, p->position.x,
           p->position.y);
    p = p->next;
  }
  printf("base\n");
}
void destoryStack(Stack* stack) {
  while (!isEmpty(stack)) {
    MazeNode e;
    pop(stack, &e);
  }
  free(stack->top);
  free(stack);
}

/**
 * 返回一个地图
 */
int** createMap(PosType start, PosType end) {
  int** map = (int**)malloc(row * sizeof(int*));
  for (int i = 0; i < row; i++) {
    *(map + i) = (int*)malloc(column * sizeof(int));
  }
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      if (i == 0 || i == 9 || j == 0 || j == 9) {
        map[i][j] = 1;
      } else {
        map[i][j] = 0;
      }
    }
  }
  map[1][3] = 1;
  map[1][7] = 1;
  map[2][3] = 1;
  map[2][7] = 1;
  map[3][5] = 1;
  map[3][6] = 1;
  map[4][2] = 1;
  map[4][3] = 1;
  map[4][4] = 1;
  map[5][4] = 1;
  map[6][2] = 1;
  map[6][6] = 1;
  map[7][2] = 1;
  map[7][3] = 1;
  map[7][4] = 1;
  map[7][6] = 1;
  map[7][7] = 1;
  map[8][1] = 1;
  return map;
}

/**
 * 走不通使该点为-1
 */
void passed(int** map, PosType e) {
  map[e.x][e.y] = -1;
}
void printMap(int** map, PosType start, PosType end) {
  // 打印
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      if (i == start.x && j == start.y) {
        printf("S ");
        continue;
      }
      if (i == end.x && j == end.y) {
        printf("E ");
        continue;
      }
      if (map[i][j] == 1) {
        printf("# ");
      } else if (map[i][j] == -1) {
        printf("& ");
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}
/**
 * 根据栈打印路径
 */
void printPath(Stack* stack, int** map, PosType start, PosType end) {
  while (!isEmpty(stack)) {
    MazeNode e;
    pop(stack, &e);
    // 右下左上在map上分别赋值10,11,12,13
    if (e.direction == 0) {
      map[e.position.x][e.position.y] = 10;
    } else if (e.direction == 1) {
      map[e.position.x][e.position.y] = 11;
    } else if (e.direction == 2) {
      map[e.position.x][e.position.y] = 12;
    } else {
      map[e.position.x][e.position.y] = 13;
    }
  }
  // 遇到10,11,12,13打印箭头
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      if (i == start.x && j == start.y) {
        printf("S ");
        continue;
      }
      if (i == end.x && j == end.y) {
        printf("E ");
        continue;
      }
      if (map[i][j] == 1) {
        printf("# ");
      } else if (map[i][j] == 10) {
        printf("→ ");
      } else if (map[i][j] == 11) {
        printf("↓ ");
      } else if (map[i][j] == 12) {
        printf("← ");
      } else if (map[i][j] == 13) {
        printf("↑ ");
      } else {
        printf("  ");
      }
    }
    printf("\n");
  }
}
/**
 * 一个点的右下左上可通就返回1,用e返回顺时针的第一个可走的点
 */
int available(int** map, MazeNode* maze, PosType* e) {
  int flag = 0;
  if (map[maze->position.x][maze->position.y + 1] == 0) {
    // 下一个方向是右方向
    e->x = maze->position.x;
    e->y = maze->position.y + 1;
    maze->direction = 0;
    flag = 1;
  } else if (map[maze->position.x + 1][maze->position.y] == 0) {
    // 下一个方向是下方向
    e->x = maze->position.x + 1;
    e->y = maze->position.y;
    maze->direction = 1;
    flag = 1;
  } else if (map[maze->position.x][maze->position.y - 1] == 0) {
    // 下一个方向是左方向
    e->x = maze->position.x;
    e->y = maze->position.y - 1;
    maze->direction = 2;
    flag = 1;
  } else if (map[maze->position.x - 1][maze->position.y] == 0) {
    // 下一个方向是上方向
    e->x = maze->position.x - 1;
    e->y = maze->position.y;
    maze->direction = 3;
    flag = 1;
  }
  return flag;
}
void mazePath(int** map, PosType start, PosType end) {
  Stack* stack = initStack();
  PosType cur = start, temp;
  do {
    // 当前位置插入栈顶
    MazeNode* node = (MazeNode*)malloc(sizeof(MazeNode));
    node->position.x = cur.x;
    node->position.y = cur.y;
    push(stack, node);
    passed(map, node->position);  // map(x,y)confer-1

    if (cur.x == end.x && cur.y == end.y) {
      // 如果是出口,则结束
      printf("走出迷宫\n");
      traverseStack(stack);               // 打印栈路径
      printPath(stack, map, start, end);  // 打印路径
      destoryStack(stack);
      return;
    }
    // 右下左上有空通道
    if (available(map, node, &temp)) {
      cur.x = temp.x;
      cur.y = temp.y;
    } else {
      // 右下左上没有空通道,栈不空, 删去栈顶位置,
      while (!isEmpty(stack)) {
        MazeNode* top = getTop(stack);
        if (!available(map, top, &temp)) {
          MazeNode tempNode;
          pop(stack, &tempNode);
        } else {
          cur.x = temp.x;
          cur.y = temp.y;
          break;
        }
      }
    }
  } while (!isEmpty(stack));
  printf("栈空,没有出口\n");
}
void startAndEnd(PosType* start, PosType* end) {
  start->x = 1;
  start->y = 1;
  end->x = 8;
  end->y = 8;
}
int main() {
  PosType start, end;  // 起点、终点
  startAndEnd(&start, &end);
  int** map = createMap(start, end);  // 创建一个地图二维数组
  printMap(map, start, end);
  mazePath(map, start, end);
  return 0;
}