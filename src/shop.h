/*
 * shop.h - 商店模块
 *
 * 管理道具列表(用链表存储)和购买逻辑。
 */

#ifndef SHOP_H
#define SHOP_H

#include "list.h"
#include "player.h"
#include "types.h"

/* ==================== 商店道具结构体 ==================== */
typedef struct {
  ItemType type;
  const char *name;
  const char *desc;
  int price;
  int owned; /* 是否已购买 */
} ShopItem;

/* ==================== 公开接口 ==================== */

/* 初始化商品链表(创建所有道具) */
void shop_init(LinkedList *items);

/* 购买道具, 成功返回 1, 失败(金币不足/已拥有)返回 0 */
int shop_buy(LinkedList *items, ItemType type, int *player_score);

/* 将已购道具效果应用到玩家 */
void shop_apply_effects(LinkedList *items, Player *player);

/* 重置玩家道具增益 */
void shop_reset_effects(Player *player);

/* 重置商店(下一关时清除已购状态) */
void shop_reset_owned(LinkedList *items);

#endif /* SHOP_H */
