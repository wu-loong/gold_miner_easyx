/*
 * list.h - 通用链表模块
 *
 * 提供基于 void* 的通用单链表，支持头插/尾插/删除/查找/遍历/清空。
 * 用于管理游戏中的矿物列表和商店道具列表。
 */

#ifndef LIST_H
#define LIST_H

/* ==================== 链表节点 ==================== */
typedef struct Node {
  void *data;        /* 指向实际数据 */
  struct Node *next; /* 下一节点 */
} Node;

/* ==================== 链表头 ==================== */
typedef struct {
  Node *head; /* 首节点 */
  int count;  /* 节点计数 */
} LinkedList;

/* ==================== 回调函数类型 ==================== */
/* 匹配函数: 返回 1 表示匹配 */
typedef int (*MatchFunc)(void *data, void *context);
/* 访问函数: 对每个节点执行操作 */
typedef void (*VisitFunc)(void *data, void *context);

/* ==================== 公开接口 ==================== */

/* 初始化链表（head=NULL, count=0） */
void list_init(LinkedList *list);

/* 头部插入（O(1)） */
void list_insert_head(LinkedList *list, void *data);

/* 尾部插入（O(n)） */
void list_insert_tail(LinkedList *list, void *data);

/* 按数据指针删除节点，成功返回 1，未找到返回 0 */
int list_remove(LinkedList *list, void *data);

/* 按回调条件查找，返回第一个匹配的 data，未找到返回 NULL */
void *list_find(LinkedList *list, MatchFunc match, void *context);

/* 遍历链表，对每个节点调用 visit */
void list_foreach(LinkedList *list, VisitFunc visit, void *context);

/* 清空链表，对每个节点调用 free_func 释放数据，传 NULL 则仅释放节点 */
void list_clear(LinkedList *list, VisitFunc free_func);

/* 返回节点数量 */
int list_count(LinkedList *list);

#endif /* LIST_H */
