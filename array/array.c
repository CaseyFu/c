#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ARRAY_DIM 8  //数组最大维数
typedef int ElemType;
typedef struct Array {
  ElemType* base;  // 数组基址
  int dim;         // 数组维数
  int* bounds;     // 数组维界基址
  int* constants;  // 常量基址
} Array;

int value(Array* arr, ElemType* e, ...);
/**
 * 初始化数组
 */
void initArray(Array* arr, int dim, ...) {
  if (dim < 1 || dim > MAX_ARRAY_DIM)
    return;
  arr->dim = dim;
  arr->bounds = (int*)malloc(sizeof(int));
  if (!arr->bounds)
    return;
  int elemtotal = 1;
  va_list ap;
  va_start(ap, dim);
  for (int i = 0; i < dim; i++) {
    arr->bounds[i] = va_arg(ap, int);
    if (arr->bounds[i] < 0)
      return;
    elemtotal *= arr->bounds[i];
  }
  va_end(ap);
  arr->base = (ElemType*)malloc(sizeof(ElemType) * elemtotal);
  if (!arr->base)
    return;
  arr->constants = (int*)malloc(sizeof(int) * dim);
  arr->constants[dim - 1] = 1;
  for (int i = dim - 2; i >= 0; i--) {
    arr->constants[i] = arr->bounds[i + 1] * arr->constants[i + 1];
  }
}
/**
 * 销毁数组
 */
void destroyArray(Array* arr) {
  if (arr->base)
    free(arr->base);
  if (arr->bounds)
    free(arr->bounds);
  if (arr->constants)
    free(arr->constants);
  arr->base = NULL;
  arr->bounds = NULL;
  arr->constants = NULL;
}
/**
 * 遍历数组
 */
void traverseArray(Array* arr) {
  if (arr->dim == 2) {
    // dim为2,专门遍历二维数组
    for (int i = 0; i < arr->bounds[0]; i++) {
      for (int j = 0; j < arr->bounds[1]; j++) {
        ElemType e;
        value(arr, &e, i, j);
        printf("%d\t", e);
      }
      printf("\n");
    }
  } else {
    // 变量其他维度数组,以行表示
    int count = 1;
    printf("dim:%d\n", arr->dim);
    printf("bounds:\n");
    for (int i = 0; i < arr->dim; i++) {
      printf("%d\t", arr->bounds[i]);
      count *= arr->bounds[i];
    }
    printf("\n");
    printf("constants:\n");
    for (int i = 0; i < arr->dim; i++) {
      printf("%d\t", arr->constants[i]);
    }
    printf("\n");
    printf("data以行表示:\n");
    for (int i = 0; i < count; i++) {
      printf("%d\t", arr->base[i]);
    }
    printf("\n");
  }
}

/**
 * 求出该元素在arr中的相对地址
 */
int locate(Array* arr, va_list ap, int* offset) {
  *offset = 0;
  int temp = 0;
  for (int i = 0; i < arr->dim; i++) {
    temp = va_arg(ap, int);
    if (temp < 0 || temp >= arr->bounds[i])
      return 0;
    *offset += arr->constants[i] * temp;
  }
  return 1;
}
/**
 * arr是n维数组,e为元素变量,随后是n个下标值
 * 若下标不超界,则将e的值赋给所指定的A的元素
 * 赋值,如二维数组
 * arr[3][3]=999;   等价于assign(arr,999,3,3);
 */
void assign(Array* arr, ElemType e, ...) {
  va_list ap;
  int offset = 0;
  va_start(ap, e);
  locate(arr, ap, &offset);
  arr->base[offset] = e;
  va_end(ap);
}
/**
 * arr为n维数组,e为元素变量,随后是n个下标值
 * 若各下标不超界,则e数值为所指定的arr的元素值
 * 获取值,如二维数组
 * arr[3][2]   value(arr,&e,3,2);
 * printf("%d",e);
 */
int value(Array* arr, ElemType* e, ...) {
  va_list ap;
  va_start(ap, e);
  int offset;
  int result = locate(arr, ap, &offset);
  *e = arr->base[offset];
  va_end(ap);
  return result;
}

int main() {
  // 初始化一个3x3的二维数组
  Array arr;
  initArray(&arr, 2, 3, 3);  // 形如arr[3][3]
  for (int i = 0; i < arr.bounds[0]; i++) {
    for (int j = 0; j < arr.bounds[1]; j++) {
      assign(&arr, i * 10 + j, i, j);
    }
  }
  assign(&arr, 999, 2, 2);  // 赋值,如arr[2][2] = 999;
  printf("取值:\n");
  ElemType e;
  value(&arr, &e, 2, 2);
  printf("e=%d", e);
  traverseArray(&arr);
  destroyArray(&arr);
  return 0;
}