/*
 * shop.c - 商店模块实现
 */

#include "shop.h"
#include <stdlib.h>

/* ---- 创建单个道具 ---- */
static ShopItem *create_item(ItemType type, const char *name, const char *desc,
                             int price) {
  ShopItem *item = (ShopItem *)malloc(sizeof(ShopItem));
  if (item == NULL) {
    return NULL;
  }
  item->type = type;
  item->name = name;
  item->desc = desc;
  item->price = price;
  item->owned = 0;
  return item;
}

/* ---- 初始化商品链表 ---- */
void shop_init(LinkedList *items) {
  if (items == NULL) {
    return;
  }
  list_init(items);

  list_insert_tail(items, create_item(ITEM_STRENGTH, "Strength Potion",
                                      "Faster hook retract speed", 300));

  list_insert_tail(
      items, create_item(ITEM_LUCKY, "Lucky Clover", "+50% score bonus", 400));

  list_insert_tail(items, create_item(ITEM_BOMB, "Dynamite",
                                      "Destroy stone instantly", 250));
}

/* ---- 查找匹配类型的道具 ---- */
static int match_item_type(void *data, void *context) {
  ShopItem *item = (ShopItem *)data;
  ItemType *target = (ItemType *)context;
  return (item->type == *target) ? 1 : 0;
}

/* ---- 购买 ---- */
int shop_buy(LinkedList *items, ItemType type, int *player_score) {
  ShopItem *item;

  if (items == NULL || player_score == NULL) {
    return 0;
  }

  item = (ShopItem *)list_find(items, match_item_type, &type);
  if (item == NULL) {
    return 0;
  }

  /* 已拥有 */
  if (item->owned) {
    return 0;
  }

  /* 金币不足 */
  if (*player_score < item->price) {
    return 0;
  }

  *player_score -= item->price;
  item->owned = 1;
  return 1;
}

/* ---- 应用效果 ---- */
void shop_apply_effects(LinkedList *items, Player *player) {
  Node *cur;

  if (items == NULL || player == NULL) {
    return;
  }

  cur = items->head;
  while (cur != NULL) {
    ShopItem *item = (ShopItem *)cur->data;
    if (item->owned) {
      switch (item->type) {
      case ITEM_STRENGTH:
        player->strength_boost = 1;
        break;
      case ITEM_LUCKY:
        player->lucky_boost = 1;
        break;
      case ITEM_BOMB:
        player->has_bomb = 1;
        break;
      default:
        break;
      }
    }
    cur = cur->next;
  }
}

/* ---- 重置玩家增益 ---- */
void shop_reset_effects(Player *player) {
  if (player == NULL) {
    return;
  }
  player->strength_boost = 0;
  player->lucky_boost = 0;
  player->has_bomb = 0;
}

/* ---- 重置已购状态 ---- */
void shop_reset_owned(LinkedList *items) {
  Node *cur;

  if (items == NULL) {
    return;
  }

  cur = items->head;
  while (cur != NULL) {
    ShopItem *item = (ShopItem *)cur->data;
    item->owned = 0;
    cur = cur->next;
  }
}
