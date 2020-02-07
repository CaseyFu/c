#include <stdio.h>
#include <stdlib.h>

/**
 * 字符串的定长顺序存储表示
 */

#define MAXSTRING 11
typedef char SqString;
int strLength(SqString* str);
void clearString(SqString* str);
void strDelete(SqString* str, int i, int len);
void strInsert(SqString* str, int i, SqString* str1);
/**
 * 给字符串赋值
 */
void strAssign(SqString* str, SqString* str1) {
  int i = 0;
  for (char* p = str1; *p != '\0' && i < MAXSTRING; p++) {
    str[i++] = *p;
  }
  str[i] = '\0';
}
/**
 * 字符串复制,先清空原字符串在复制
 */
void strCopy(SqString* str, SqString* str1) {
  clearString(str);
  strAssign(str, str1);
}
/**
 * 字符串为NULL空串返回1
 */
int isEmpty(SqString* str) {
  return strLength(str) ? 0 : 1;
}
/**
 * 字符串比较,
 * if:s==t返回0,
 * else:第一个不同字母,s<t返回差值,s>t返回差值
 */
int strCompare(SqString* s, SqString* t) {
  for (int i = 0; i < strLength(s) + 1; i++) {
    if (s[i] != t[i]) {
      return s[i] - t[i];
    }
  }
  return strLength(s) - strLength(t);
}
/**
 * 字符串长度, 空串返回-1
 */
int strLength(SqString* str) {
  if (!str)
    return -1;
  int i = 0;
  for (char* p = str; *p != '\0'; p++) {
    i++;
  }
  return i;
}
/**
 * 将字符串置为空串
 */
void clearString(SqString* str) {
  for (char* p = str; *p != '\0'; p++) {
    *p = '\0';
  }
}

/**
 * 拼接两个字符串,将str2接到str1后,考虑截断
 */
void concat(SqString* str1, SqString* str2) {
  int str1Len = strLength(str1);
  if (str1Len == MAXSTRING - 1)
    return;
  int str2Len = strLength(str2);
  int i = str1Len;
  for (i; i - str1Len < str2Len && i < MAXSTRING; i++) {
    str1[i] = str2[i - str1Len];
  }
  str1[i] = '\0';
}

/**
 * 返回主串的第i个字符起,长度为len的子串
 */
SqString* subString(SqString* str, int i, int len) {
  int strLen = strLength(str);
  if (i < 0 || i > strLen || len <= 0 || len > strLength(str))
    return NULL;
  SqString* string = (SqString*)malloc(sizeof(SqString) * len);
  if (!string)
    return NULL;
  int index = 0;
  for (int j = i; j < strLen && index < len; j++) {
    string[index++] = str[j];
  }
  string[index] = '\0';
  return string;
}
/**
 * 返回子串自i个字符后第一次出现的首字母的位置,否则返回-1
 */
int indexOfSub(SqString* s, SqString* t, int k) {
  int i = k;
  int j = 0;
  while (i < strLength(s) && j < strLength(t)) {
    if (s[i] == t[j]) {
      i++;
      j++;
    } else {
      i = i - j + 1;
      j = 0;
    }
  }
  if (j >= strLength(t)) {
    return i - strLength(t);
  }
  return 0;
}
/**
 * 用str2替换所有在str中出现的str1
 */
void replace(SqString* str, SqString* str1, SqString* str2) {
  int strLen = strLength(str);
  int str1Len = strLength(str1);
  int str2Len = strLength(str2);
  int index = 0;
  while (index != -1) {
    index = indexOfSub(str, str1, 0);
    if (index != -1) {
      strDelete(str, index, str1Len);
      strInsert(str, index, str2);
    }
  }
}
/**
 * 主串str的第i个字符前插入串str1
 */
void strInsert(SqString* str, int i, SqString* str1) {
  int strLen = strLength(str);
  if (i < 0 || i > strLen)
    return;
  int str1Len = strLength(str1);
  int j = strLen - 1;
  for (j; j >= i; j--) {
    if (j + str1Len > MAXSTRING)
      continue;
    str[j + str1Len] = str[j];
  }
  for (int k = 0; k < str1Len; k++) {
    str[++j] = str1[k];
  }
  str[MAXSTRING + 1] = '\0';
}

/**
 * 删除第i个字符起,长度为len的子串
 */
void strDelete(SqString* str, int i, int len) {
  SqString* e = subString(str, i, len);
  if (e) {
    printf("strDelete()删除:%s\n", e);
    free(e);
  }
}
/**
 * 销毁字符串
 */
void destroyString() {}

/**
 * 打印字符串
 */
void printStr(SqString* str) {
  if (!str)
    return;
  printf("打印字符串:");
  for (char* p = str; *p != '\0'; p++) {
    printf("%c", *p);
  }
  printf("\n");
}

int main() {
  SqString str1[MAXSTRING + 1], str2[MAXSTRING + 1];  // +1用于放'\0'
  strAssign(str1, "01234e23");
  strAssign(str2, "23");
  return 0;
}