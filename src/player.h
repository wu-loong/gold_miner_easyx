/*
 * player.h - 玩家与钩子模块
 *
 * 管理钩子的摆动/伸出/缩回状态机和碰撞检测。
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "list.h"
#include "objects.h"
#include "types.h"

/* ==================== 玩家结构体 ==================== */
typedef struct {
  /* 锚点(矿工位置) */
  float origin_x;
  float origin_y;

  /* 钩子状态机 */
  HookState state;
  float angle;       /* 当前角度(弧度)，0=正下方 */
  float swing_speed; /* 摆动角速度 */
  float swing_dir;   /* 摆动方向: +1 或 -1 */

  /* 伸缩 */
  float length;        /* 当前绳长 */
  float max_length;    /* 最大绳长 */
  float extend_speed;  /* 伸出速度 */
  float retract_speed; /* 基础缩回速度 */

  /* 抓取 */
  GameObject *grabbed; /* 当前抓住的物品(NULL=没抓) */

  /* 分数和道具效果 */
  int score;
  int strength_boost; /* 力量药水: 加快缩回 */
  int lucky_boost;    /* 幸运草: 分数加成 */
  int has_bomb;       /* 炸弹: 石头直接炸掉 */
} Player;

/* ==================== 公开接口 ==================== */

/* 初始化玩家 */
void player_init(Player *player);

/* 每帧更新(dt 为秒) */
void player_update(Player *player, float dt);

/* 发射钩子(仅在 SWINGING 状态有效) */
void player_shoot(Player *player);

/* 碰撞检测: 返回碰到的物品，未碰到返回 NULL */
GameObject *player_check_collision(Player *player, LinkedList *objects);

/* 获取钩子尖端坐标 */
void player_get_hook_tip(Player *player, float *x, float *y);

/* 完成一次抓取(加分+释放物品引用) */
void player_finish_grab(Player *player);

#endif /* PLAYER_H */
