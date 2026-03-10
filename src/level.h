/*
 * level.h - 关卡管理模块
 *
 * 定义关卡配置结构并提供关卡物品生成和通关判断。
 */

#ifndef LEVEL_H
#define LEVEL_H

#include "list.h"
#include "types.h"

/* ==================== 关卡配置 ==================== */
typedef struct {
  int level_num;    /* 关卡编号(从1开始) */
  int target_score; /* 目标分数 */
  int time_limit;   /* 时间限制(秒) */
  int object_count; /* 物品数量 */
  struct {
    ObjectType type;
    float x, y, radius;
  } object_configs[MAX_OBJECTS];
} LevelConfig;

/* ==================== 公开接口 ==================== */

/* 根据配置生成物品链表 */
void level_create_objects(LevelConfig *config, LinkedList *objects);

/* 判断是否通关: score >= target 返回 1 */
int level_is_complete(int score, int target_score);

/* 检查场景中是否还有非石头类的值钱物品 */
int level_has_valuable_objects(LinkedList *objects);

#endif /* LEVEL_H */
