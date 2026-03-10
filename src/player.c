/*
 * player.c - 玩家与钩子模块实现
 */

#include "player.h"
#include <math.h>
#include <stdlib.h>

/* ---- 初始化 ---- */
void player_init(Player *player) {
  if (player == NULL) {
    return;
  }
  player->origin_x = (float)HOOK_ORIGIN_X;
  player->origin_y = (float)HOOK_ORIGIN_Y;
  player->state = HOOK_SWINGING;
  player->angle = 0.0f;
  player->swing_speed = HOOK_SWING_SPEED;
  player->swing_dir = 1.0f;
  player->length = 0.0f;
  player->max_length = HOOK_MAX_LENGTH;
  player->extend_speed = HOOK_EXTEND_SPEED;
  player->retract_speed = HOOK_RETRACT_SPEED;
  player->grabbed = NULL;
  player->score = 0;
  player->strength_boost = 0;
  player->lucky_boost = 0;
  player->has_bomb = 0;
}

/* ---- 获取钩子尖端坐标 ---- */
void player_get_hook_tip(Player *player, float *x, float *y) {
  if (player == NULL) {
    return;
  }
  /* 角度 0 = 正下方, sin 给出 x 偏移, cos 给出 y 偏移 */
  if (x != NULL) {
    *x = player->origin_x + player->length * sinf(player->angle);
  }
  if (y != NULL) {
    *y = player->origin_y + player->length * cosf(player->angle);
  }
}

/* ---- 每帧更新 ---- */
void player_update(Player *player, float dt) {
  float tip_x, tip_y;
  float actual_retract_speed;

  if (player == NULL || dt <= 0.0f) {
    return;
  }

  switch (player->state) {
  case HOOK_SWINGING:
    /* 更新摆动角度 */
    player->angle += player->swing_speed * player->swing_dir * dt;

    /* 到达边界时反向 */
    if (player->angle > HOOK_MAX_ANGLE) {
      player->angle = HOOK_MAX_ANGLE;
      player->swing_dir = -1.0f;
    } else if (player->angle < -HOOK_MAX_ANGLE) {
      player->angle = -HOOK_MAX_ANGLE;
      player->swing_dir = 1.0f;
    }
    break;

  case HOOK_EXTENDING:
    /* 绳索伸长 */
    player->length += player->extend_speed * dt;

    /* 到达最大长度或超出屏幕 → 开始缩回 */
    player_get_hook_tip(player, &tip_x, &tip_y);
    if (player->length >= player->max_length || tip_x < 0 ||
        tip_x > WINDOW_WIDTH || tip_y > WINDOW_HEIGHT) {
      player->state = HOOK_RETRACTING;
    }
    break;

  case HOOK_RETRACTING:
    /* 缩回速度受重量影响 */
    actual_retract_speed = player->retract_speed;
    if (player->grabbed != NULL) {
      /* 重量越大缩回越慢，最低为基础速度的 20% */
      float weight_factor =
          1.0f / (1.0f + (float)player->grabbed->weight * 0.3f);
      if (weight_factor < 0.2f) {
        weight_factor = 0.2f;
      }
      actual_retract_speed *= weight_factor;

      /* 力量药水加成 */
      if (player->strength_boost > 0) {
        actual_retract_speed *= 1.8f;
      }
    }

    player->length -= actual_retract_speed * dt;

    /* 收回到原点 */
    if (player->length <= 0.0f) {
      player->length = 0.0f;
      if (player->grabbed != NULL) {
        player_finish_grab(player);
      }
      player->state = HOOK_SWINGING;
    }
    break;
  }
}

/* ---- 发射 ---- */
void player_shoot(Player *player) {
  if (player == NULL) {
    return;
  }
  if (player->state == HOOK_SWINGING) {
    player->state = HOOK_EXTENDING;
    player->length = 0.0f;
    player->grabbed = NULL;
  }
}

/* ---- 碰撞检测 ---- */
GameObject *player_check_collision(Player *player, LinkedList *objects) {
  float tip_x, tip_y;
  Node *cur;

  if (player == NULL || objects == NULL) {
    return NULL;
  }
  if (player->state != HOOK_EXTENDING) {
    return NULL;
  }

  player_get_hook_tip(player, &tip_x, &tip_y);

  cur = objects->head;
  while (cur != NULL) {
    GameObject *obj = (GameObject *)cur->data;
    if (!obj->collected) {
      float dx = tip_x - obj->x;
      float dy = tip_y - obj->y;
      float dist = sqrtf(dx * dx + dy * dy);
      float threshold = (float)HOOK_RADIUS + obj->radius;

      if (dist < threshold) {
        /* 碰撞! 抓住物品 */
        obj->collected = 1;
        player->grabbed = obj;
        player->state = HOOK_RETRACTING;

        /* 炸弹效果: 石头直接炸掉 */
        if (player->has_bomb && obj->type == OBJ_STONE) {
          player->grabbed = NULL;
          player->has_bomb = 0; /* 炸弹消耗 */
          player->state = HOOK_RETRACTING;
        }
        return obj;
      }
    }
    cur = cur->next;
  }
  return NULL;
}

/* ---- 完成抓取 ---- */
void player_finish_grab(Player *player) {
  int bonus;

  if (player == NULL || player->grabbed == NULL) {
    return;
  }

  bonus = player->grabbed->value;

  /* 幸运草加成: +50% */
  if (player->lucky_boost > 0) {
    bonus = bonus + bonus / 2;
  }

  /* 神秘袋随机 */
  if (player->grabbed->type == OBJ_MYSTERY_BAG) {
    bonus = 100 + rand() % 401; /* 100~500 */
  }

  player->score += bonus;
  player->grabbed = NULL;
}
