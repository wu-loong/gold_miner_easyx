/*
 * objects.c - 游戏物品模块实现
 */

#include "objects.h"
#include <stdlib.h>
#include <string.h>

/* ==================== 属性查找表 ==================== */

/* 各类型的基准分值 */
static const int VALUE_TABLE[OBJ_TYPE_COUNT] = {
    50,  /* OBJ_GOLD_SMALL  */
    250, /* OBJ_GOLD_LARGE  */
    600, /* OBJ_DIAMOND     */
    10,  /* OBJ_STONE       */
    200  /* OBJ_MYSTERY_BAG (基准值，实际随机) */
};

/* 各类型的重量 */
static const int WEIGHT_TABLE[OBJ_TYPE_COUNT] = {
    2, /* OBJ_GOLD_SMALL  */
    6, /* OBJ_GOLD_LARGE  */
    1, /* OBJ_DIAMOND     */
    8, /* OBJ_STONE       */
    3  /* OBJ_MYSTERY_BAG */
};

/* 各类型的名称 */
static const char *NAME_TABLE[OBJ_TYPE_COUNT] = {
    "Small Gold", "Large Gold", "Diamond", "Stone", "Mystery Bag"};

/* ==================== 接口实现 ==================== */

GameObject *object_create(ObjectType type, float x, float y, float radius) {
  GameObject *obj;

  if (type < 0 || type >= OBJ_TYPE_COUNT) {
    return NULL;
  }

  obj = (GameObject *)malloc(sizeof(GameObject));
  if (obj == NULL) {
    return NULL;
  }

  obj->type = type;
  obj->x = x;
  obj->y = y;
  obj->radius = radius;
  obj->value = VALUE_TABLE[type];
  obj->weight = WEIGHT_TABLE[type];
  obj->collected = 0;

  return obj;
}

void object_destroy(GameObject *obj) {
  if (obj != NULL) {
    free(obj);
  }
}

int object_get_value(ObjectType type) {
  if (type < 0 || type >= OBJ_TYPE_COUNT) {
    return 0;
  }
  return VALUE_TABLE[type];
}

int object_get_weight(ObjectType type) {
  if (type < 0 || type >= OBJ_TYPE_COUNT) {
    return 0;
  }
  return WEIGHT_TABLE[type];
}

const char *object_get_name(ObjectType type) {
  if (type < 0 || type >= OBJ_TYPE_COUNT) {
    return "Unknown";
  }
  return NAME_TABLE[type];
}
