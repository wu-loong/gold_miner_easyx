/*
 * list.c - 通用链表模块实现
 */

#include "list.h"
#include <stdlib.h>

/* ---- 初始化 ---- */
void list_init(LinkedList *list) {
  if (list == NULL) {
    return;
  }
  list->head = NULL;
  list->count = 0;
}

/* ---- 头部插入 O(1) ---- */
void list_insert_head(LinkedList *list, void *data) {
  Node *node;

  if (list == NULL) {
    return;
  }

  node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }

  node->data = data;
  node->next = list->head;
  list->head = node;
  list->count++;
}

/* ---- 尾部插入 O(n) ---- */
void list_insert_tail(LinkedList *list, void *data) {
  Node *node;
  Node *curr;

  if (list == NULL) {
    return;
  }

  node = (Node *)malloc(sizeof(Node));
  if (node == NULL) {
    return;
  }

  node->data = data;
  node->next = NULL;

  if (list->head == NULL) {
    list->head = node;
  } else {
    curr = list->head;
    while (curr->next != NULL) {
      curr = curr->next;
    }
    curr->next = node;
  }
  list->count++;
}

/* ---- 按数据指针删除 ---- */
int list_remove(LinkedList *list, void *data) {
  Node *curr;
  Node *prev;

  if (list == NULL || list->head == NULL) {
    return 0;
  }

  /* 检查头节点 */
  if (list->head->data == data) {
    curr = list->head;
    list->head = curr->next;
    free(curr);
    list->count--;
    return 1;
  }

  /* 遍历查找 */
  prev = list->head;
  curr = prev->next;
  while (curr != NULL) {
    if (curr->data == data) {
      prev->next = curr->next;
      free(curr);
      list->count--;
      return 1;
    }
    prev = curr;
    curr = curr->next;
  }

  return 0;
}

/* ---- 按条件查找 ---- */
void *list_find(LinkedList *list, MatchFunc match, void *context) {
  Node *curr;

  if (list == NULL || match == NULL) {
    return NULL;
  }

  curr = list->head;
  while (curr != NULL) {
    if (match(curr->data, context)) {
      return curr->data;
    }
    curr = curr->next;
  }

  return NULL;
}

/* ---- 遍历 ---- */
void list_foreach(LinkedList *list, VisitFunc visit, void *context) {
  Node *curr;

  if (list == NULL || visit == NULL) {
    return;
  }

  curr = list->head;
  while (curr != NULL) {
    visit(curr->data, context);
    curr = curr->next;
  }
}

/* ---- 清空 ---- */
void list_clear(LinkedList *list, VisitFunc free_func) {
  Node *curr;
  Node *next;

  if (list == NULL) {
    return;
  }

  curr = list->head;
  while (curr != NULL) {
    next = curr->next;
    if (free_func != NULL) {
      free_func(curr->data, NULL);
    }
    free(curr);
    curr = next;
  }

  list->head = NULL;
  list->count = 0;
}

/* ---- 计数 ---- */
int list_count(LinkedList *list) {
  if (list == NULL) {
    return 0;
  }
  return list->count;
}
