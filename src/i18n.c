#include "i18n.h"
#include <string.h>

static Language current_lang = LANG_ZH;

typedef struct {
  const char *key;
  const char *zh;
  const char *en;
} DictEntry;

static const DictEntry dict[] = {
    {"MENU_TITLE", "黄金矿工", "GOLD MINER"},
    {"MENU_START", "按 空格键 开始", "PRESS SPACE TO START"},
    {"MENU_HIGH_SCORE", "最高分: %d", "HIGH SCORE: %d"},
    {"MENU_INSTR_1", "空格键 = 发射钩子 / 购买", "SPACE = SHOOT HOOK / BUY"},
    {"MENU_INSTR_2", "抓取屏幕上的物品来得分！", "GRAB ITEMS TO SCORE!"},
    {"MENU_LANG_TOGGLE", "按 L 切换语言", "PRESS L TO TOGGLE LANG"},

    {"HUD_LEVEL", "第 %d 关", "LEVEL %d"},
    {"HUD_SCORE", "分数：%d / %d", "SCORE:%d/%d"},
    {"HUD_TIME", "时间：%d", "TIME:%d"},

    {"LC_TITLE", "本关完成！", "LEVEL COMPLETE!"},
    {"LC_SUBTITLE", "已通关第 %d 关", "LEVEL %d CLEARED"},
    {"LC_SCORE", "当前分数: %d / 目标: %d", "SCORE: %d / TARGET: %d"},
    {"LC_CONTINUE", "按 空格键 继续 / 进入商店", "PRESS SPACE TO CONTINUE"},

    {"GO_TITLE", "游戏结束", "GAME OVER"},
    {"GO_FINAL", "最终得分: %d", "FINAL SCORE: %d"},
    {"GO_NEW_HIGH", "新最高分纪录！", "NEW HIGH SCORE!"},
    {"GO_HIGH", "历史最高分: %d", "HIGH SCORE: %d"},
    {"GO_RESTART", "按 空格键 重新开始", "PRESS SPACE TO RESTART"},

    {"SHOP_TITLE", "老头商店", "SHOP"},
    {"SHOP_GOLD", "你目前的金钱: $%d", "YOUR GOLD: $%d"},
    {"SHOP_INSTR", "↑/↓选择 | 空格键购买 | 回车键进入下关", "UP/DOWN=SELECT  SPACE=BUY  ENTER=NEXT"},
    {"SHOP_OWNED", "已拥有", "OWNED"},
    {"SHOP_PRICE", "$%d", "$%d"},

    {"ITEM_STR_NAME", "大力水", "Strength Potion"},
    {"ITEM_STR_DESC", "回收重物时速度大幅提升", "Faster hook retract speed"},
    {"ITEM_LUCKY_NAME", "幸运草", "Lucky Clover"},
    {"ITEM_LUCKY_DESC", "抓取物品得分额外增加 50%", "+50%% score bonus"},
    {"ITEM_BOMB_NAME", "炸药", "Dynamite"},
    {"ITEM_BOMB_DESC", "抓到石头时立刻炸毁", "Destroy stone instantly"},

    {NULL, NULL, NULL}
};

void i18n_init(void) { current_lang = LANG_ZH; }
void i18n_toggle_language(void) { current_lang = (current_lang == LANG_EN) ? LANG_ZH : LANG_EN; }
Language i18n_get_language(void) { return current_lang; }

const char *i18n_get(const char *key) {
  int i = 0;
  if (key == NULL) return "";
  while (dict[i].key != NULL) {
    if (strcmp(dict[i].key, key) == 0) {
      if (current_lang == LANG_ZH) {
          return dict[i].zh;
      }
      return dict[i].en;
    }
    i++;
  }
  return key;
}
