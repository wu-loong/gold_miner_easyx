/*
 * game.h - 游戏状态机模块
 *
 * 核心协调者: 管理所有游戏状态的切换和模块间协作。
 */

#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "list.h"
#include "player.h"
#include "render.h"
#include "types.h"

/* ==================== 输入键位枚举 ==================== */
typedef enum {
  KEY_NONE = 0,
  KEY_SPACE,
  KEY_UP,
  KEY_DOWN,
  KEY_ENTER,
  KEY_L
} InputKey;

/* ==================== 游戏主结构体 ==================== */
typedef struct {
  GameState state;
  Player player;
  LinkedList objects;    /* 当前关矿物链表 */
  LinkedList shop_items; /* 商店道具链表 */
  LevelConfig levels[MAX_LEVELS];
  int total_levels;
  int current_level; /* 从 0 开始的索引 */
  float time_left;   /* 剩余时间(秒) */
  int highscore;
  int shop_selected; /* 商店选中项 */
  int victory;       /* 通关全部关卡 */
  RenderContext render_ctx;
} Game;

/* ==================== 公开接口 ==================== */

int game_init(Game *game);
void game_cleanup(Game *game);
void game_handle_input(Game *game, InputKey key);
void game_update(Game *game, float dt);
void game_render(Game *game);

#endif /* GAME_H */
