/*
 * objects.h - 游戏物品模块
 *
 * 定义矿物/物品数据结构和操作接口。
 * 物品类型包括: 小金块、大金块、钻石、石头、神秘袋
 */

#ifndef OBJECTS_H
#define OBJECTS_H

#include "types.h"

/* ==================== 游戏物品结构体 ==================== */
typedef struct {
  ObjectType type; /* 物品类型 */
  float x, y;      /* 中心坐标 */
  float radius;    /* 碰撞半径 & 绘制大小 */
  int value;       /* 分值 */
  int weight;      /* 重量(影响缩回速度) */
  int collected;   /* 是否已被收集 */
} GameObject;

/* ==================== 公开接口 ==================== */

/* 创建物品(堆分配)，属性根据类型自动填充 */
GameObject *object_create(ObjectType type, float x, float y, float radius);

/* 销毁物品(释放内存)，传 NULL 安全 */
void object_destroy(GameObject *obj);

/* 查表获取某类型的基准分值 */
int object_get_value(ObjectType type);

/* 查表获取某类型的重量 */
int object_get_weight(ObjectType type);

/* 获取类型名称字符串 */
const char *object_get_name(ObjectType type);

#endif /* OBJECTS_H */
