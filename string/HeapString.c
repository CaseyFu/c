#include <stdio.h>
#include <stdlib.h>

/**
 * 字符串依靠heap来分配空间
 */
typedef struct HString {
  char* ch;
  int length;
} HString;

int strLength(HString* str);
void clearString(HString* str);
void printString(HString* str);
int indexOfSubstr(HString* s, HString* t, int k);
/**
 * 字符串赋值:str1赋值给str
 */
void initString(HString* str) {
  str->ch = NULL;
  str->ch = 0;
}
void strAssign(HString* str, char* str1) {
  if (str->ch)
    free(str->ch);
  int len = 0;
  for (char* p = str1; *p; p++) {
    len++;
  }
  if (len) {
    str->ch = (char*)malloc(sizeof(char) * len);
    if (!str->ch)
      return;
    for (int i = 0; i < len; i++) {
      str->ch[i] = str1[i];
    }
    str->length = len;
  } else {
    str->ch = NULL;
    str->length = 0;
  }
}
/**
 * 字符串复制
 */
void strCopy(HString* str, HString* str1) {
  clearString(str);
  strAssign(str, str1->ch);
}
/**
 * 字符串为NULL空串返回1
 */
int isEmpty(HString* str) {
  return str->length ? 0 : 1;
}
/**
 * 字符串比较
 *  if:s==t返回0,
 *  else:第一个不同字母,s<t返回差值,s>t返回差值
 */
int strCompare(HString* str, HString* str1) {
  for (int i = 0; i < strLength(str); i++) {
    if (str->ch[i] != str1->ch[i]) {
      return str->ch[i] - str1->ch[i];
    }
  }
  return str->length - str1->length;
}
/**
 * 字符串长度
 */
int strLength(HString* str) {
  return str->length;
}
/**
 * 将字符串置为空串
 */
void clearString(HString* str) {
  if (str->ch)
    free(str->ch);
  str->ch = NULL;
  str->length = 0;
}

/**
 * 拼接两个字符串
 */
void concat(HString* str, HString* str1, HString* e) {
  int len = str->length + str1->length;
  e->ch = (char*)malloc(sizeof(char) * len);
  if (!e->ch)
    return;
  e->length = 0;
  for (char* p = str->ch; *p; p++) {
    e->ch[e->length++] = *p;
  }
  for (char* p = str1->ch; *p; p++) {
    e->ch[e->length++] = *p;
  }
}
/**
 * 用e返回主串str的第i个字符起,长度为len的子串
 */
void subString(HString* str, HString* e, int i, int len) {
  if (i < 0 || i > strLength(str) || len <= 0)
    return;
  e->ch = (char*)malloc(sizeof(char) * len);
  if (!e->ch)
    return;
  e->length = len;
  int index = 0;
  for (int j = i; j < strLength(str) && index < strLength(str); j++) {
    e->ch[index++] = str->ch[j];
  }
}

/**
 * 将t插入到str中
 */
void insert(HString* str, HString* t, int k) {
  int strLen = strLength(str);
  int tLen = strLength(t);
  if (k < 0 || k > strLen)
    return;
  str->ch = (char*)realloc(str->ch, sizeof(char) * (strLen + tLen));
  if (!str->ch)
    return;
  for (int i = strLen - 1; i >= k; i--) {
    str->ch[i + tLen] = str->ch[i];
  }
  int index = k;
  for (int i = 0; i < tLen; i++) {
    str->ch[index++] = t->ch[i];
  }
  str->length = strLen + tLen;
}
void delete (HString* str, int k, int len) {
  if (k < 0 || k > strLength(str))
    return;
  for (int i = len + k; i < strLength(str); i++) {
    str->ch[i - len] = str->ch[i];
  }
  str->ch = (char*)realloc(str->ch, sizeof(char) * (strLength(str) - len));
  str->length -= len;
}
void replace(HString* str, HString* s, HString* t) {
  int i = 0;
  while (i != -1) {
    i = indexOfSubstr(str, s, 0);
    if (i != -1) {
      delete (str, i, strLength(s));
      insert(str, t, i);
    }
  }
}
/* 串的模式匹配 */
/**
 * 两重循环法,就相当于两个for循环
 * 返回子串自i个字符后第一次出现的位置,否则返回-1
 */
int indexOfSubstr(HString* s, HString* t, int k) {
  if (k < 0 || k >= strLength(s))
    return -1;
  int i = k;
  int j = 0;
  while (i < strLength(s) && j < strLength(t)) {
    if (s->ch[i] == t->ch[j]) {
      i++;
      j++;
    } else {
      i = i - j + 1;
      j = 0;
    }
  }
  if (j >= strLength(t)) {
    return i - strLength(t);
  } else {
    return -1;
  }
}
/**
 * 销毁字符串
 */
void destroyString(HString* str) {
  if (str->ch)
    free(str->ch);
}
/**
 * 打印字符串
 */
void printString(HString* str) {
  if (!strLength(str))
    return;
  for (int i = 0; i < str->length; i++) {
    printf("%c", str->ch[i]);
  }
  printf("\n");
  printf("长度:%d\n", str->length);
}
int main() {
  HString str;
  HString str1;
  HString e;
  initString(&str);
  initString(&str1);
  initString(&e);
  strAssign(&str, "bcbababaaaab");
  strAssign(&str1, "aaaab");
  strAssign(&e, "xfk");
  printf("%d\n", indexOfSubstr(&str, &str1, 0));
  printString(&str);
  return 0;
}