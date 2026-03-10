/*
 * level.c - 关卡管理模块实现
 */

#include "level.h"
#include "objects.h"
#include "types.h"
#include <stdlib.h>

/* 随机抖动范围 */
#define JITTER_X 45
#define JITTER_Y 35
#define OBJ_X_MIN 55.0f
#define OBJ_X_MAX 745.0f
#define OBJ_Y_MIN 130.0f
#define OBJ_Y_MAX 560.0f

void level_create_objects(LevelConfig *config, LinkedList *objects) {
  int i;
  GameObject *obj;
  float x, y;

  if (config == NULL || objects == NULL) {
    return;
  }

  for (i = 0; i < config->object_count; i++) {
    /* 每次生成时施加随机偏移，让布局不固定 */
    x = config->object_configs[i].x + (float)((rand() % (JITTER_X * 2 + 1)) - JITTER_X);
    y = config->object_configs[i].y + (float)((rand() % (JITTER_Y * 2 + 1)) - JITTER_Y);

    /* 边界约束，确保物品在可见游戏区域内 */
    if (x < OBJ_X_MIN) x = OBJ_X_MIN;
    if (x > OBJ_X_MAX) x = OBJ_X_MAX;
    if (y < OBJ_Y_MIN) y = OBJ_Y_MIN;
    if (y > OBJ_Y_MAX) y = OBJ_Y_MAX;

    obj = object_create(config->object_configs[i].type, x, y,
                        config->object_configs[i].radius);
    if (obj != NULL) {
      list_insert_tail(objects, obj);
    }
  }
}

int level_is_complete(int score, int target_score) {
  return (score >= target_score) ? 1 : 0;
}

int level_has_valuable_objects(LinkedList *objects) {
  Node *cur = objects->head;
  while (cur != NULL) {
    GameObject *obj = (GameObject *)cur->data;
    if (!obj->collected && obj->type != OBJ_STONE) {
      return 1;
    }
    cur = cur->next;
  }
  return 0;
}
