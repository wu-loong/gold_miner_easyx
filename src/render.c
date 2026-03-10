/*
 * render.c - 渲染模块实现（Win32 + EasyX 纯 C 版本）
 */

#include "render.h"
#include "i18n.h"
#include "objects.h"
#include "shop.h"
#include "easyx_bridge.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>

#define COLOR_SKY RGB(135, 206, 235)
#define COLOR_GROUND RGB(139, 90, 43)
#define COLOR_UNDERGROUND RGB(101, 67, 33)
#define COLOR_GRASS RGB(50, 150, 50)

static void utf8_to_tchar(const char *src, TCHAR *dst, int capacity) {
  if (dst == NULL || capacity <= 0) {
    return;
  }
  dst[0] = _T('\0');
  if (src == NULL) {
    return;
  }
  MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, capacity);
}

static void draw_text_line(int x, int y, int size, COLORREF color,
                           const char *text) {
  TCHAR wide[512];
  utf8_to_tchar(text, wide, (int)(sizeof(wide) / sizeof(wide[0])));
  ex_setbkmode(TRANSPARENT);
  ex_settextcolor(color);
  ex_settextstyle(size, 0, _T("Microsoft YaHei UI"));
  ex_outtextxy(x, y, wide);
}

static void draw_text_center(int center_x, int y, int size, COLORREF color,
                             const char *text) {
  TCHAR wide[512];
  int width;
  utf8_to_tchar(text, wide, (int)(sizeof(wide) / sizeof(wide[0])));
  ex_setbkmode(TRANSPARENT);
  ex_settextcolor(color);
  ex_settextstyle(size, 0, _T("Microsoft YaHei UI"));
  width = ex_textwidth(wide);
  ex_outtextxy(center_x - width / 2, y, wide);
}

static void draw_filled_circle(int cx, int cy, int radius) {
  ex_solidcircle(cx, cy, radius);
}

static void draw_diamond(int cx, int cy, int radius, COLORREF fill,
                         COLORREF edge) {
  int dy;
  for (dy = -radius; dy <= radius; dy++) {
    float factor = 1.0f - (float)abs(dy) / (float)radius;
    int half_w = (int)(radius * factor);
    ex_setlinecolor(fill);
    ex_line(cx - half_w, cy + dy, cx + half_w, cy + dy);
  }
  ex_setlinecolor(edge);
  ex_line(cx, cy - radius, cx + radius, cy);
  ex_line(cx + radius, cy, cx, cy + radius);
  ex_line(cx, cy + radius, cx - radius, cy);
  ex_line(cx - radius, cy, cx, cy - radius);
  ex_setlinecolor(RGB(255, 255, 255));
  ex_line(cx, cy - radius + 2, cx + radius / 2, cy);
  ex_line(cx, cy - radius + 2, cx - radius / 2, cy);
}

static void render_single_object(GameObject *obj) {
  int cx;
  int cy;
  int rad;

  if (obj == NULL) {
    return;
  }

  cx = (int)obj->x;
  cy = (int)obj->y;
  rad = (int)obj->radius;

  switch (obj->type) {
  case OBJ_GOLD_SMALL:
    ex_setfillcolor(RGB(255, 215, 0));
    draw_filled_circle(cx, cy, rad);
    ex_setfillcolor(RGB(255, 245, 100));
    draw_filled_circle(cx - rad / 4, cy - rad / 4, rad / 3);
    ex_setlinecolor(RGB(255, 255, 200));
    ex_line(cx - rad - 3, cy, cx - rad + 3, cy);
    ex_line(cx - rad, cy - 3, cx - rad, cy + 3);
    break;

  case OBJ_GOLD_LARGE:
    ex_setfillcolor(RGB(218, 165, 32));
    draw_filled_circle(cx, cy, rad);
    ex_setfillcolor(RGB(255, 215, 0));
    draw_filled_circle(cx - rad / 3, cy - rad / 3, rad / 3);
    ex_setlinecolor(RGB(255, 255, 150));
    ex_line(cx + rad + 2, cy - 5, cx + rad + 2, cy + 5);
    ex_line(cx + rad - 3, cy, cx + rad + 7, cy);
    break;

  case OBJ_DIAMOND:
    draw_diamond(cx, cy, rad, RGB(0, 230, 255), RGB(200, 255, 255));
    break;

  case OBJ_STONE:
    ex_setfillcolor(RGB(128, 128, 128));
    ex_solidrectangle(cx - rad, cy - rad, cx + rad, cy + rad);
    ex_setlinecolor(RGB(80, 80, 80));
    ex_rectangle(cx - rad, cy - rad, cx + rad, cy + rad);
    ex_setlinecolor(RGB(100, 100, 100));
    ex_line(cx - rad + 3, cy - rad / 2, cx + rad - 3, cy - rad / 3);
    ex_line(cx - rad + 5, cy + rad / 3, cx + rad / 2, cy + rad / 2);
    break;

  case OBJ_MYSTERY_BAG:
    ex_setfillcolor(RGB(160, 82, 45));
    draw_filled_circle(cx, cy, rad);
    ex_setlinecolor(RGB(120, 60, 30));
    ex_line(cx - rad / 2, cy - rad + 2, cx + rad / 2, cy - rad + 2);
    break;

  default:
    break;
  }
}

int render_init(RenderContext *ctx) {
  if (ctx == NULL) {
    return 0;
  }

  ex_initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
  ex_setbkcolor(RGB(0, 0, 0));
  ex_begin_batch_draw();
  ctx->initialized = 1;
  return 1;
}

void render_cleanup(RenderContext *ctx) {
  if (ctx == NULL || !ctx->initialized) {
    return;
  }
  ex_end_batch_draw();
  ex_closegraph();
  ctx->initialized = 0;
}

void render_clear(RenderContext *ctx) {
  (void)ctx;
  ex_cleardevice();
}

void render_present(RenderContext *ctx) {
  (void)ctx;
  ex_flush_batch_draw();
}

void render_background(RenderContext *ctx) {
  int y;
  (void)ctx;

  ex_setfillcolor(COLOR_SKY);
  ex_solidrectangle(0, 0, WINDOW_WIDTH, GROUND_Y);

  ex_setfillcolor(COLOR_GRASS);
  ex_solidrectangle(0, GROUND_Y - 5, WINDOW_WIDTH, GROUND_Y + 5);

  for (y = GROUND_Y + 5; y < WINDOW_HEIGHT; y++) {
    float t = (float)(y - GROUND_Y) / (float)(WINDOW_HEIGHT - GROUND_Y);
    int cr = GetRValue(COLOR_GROUND) +
             (int)((GetRValue(COLOR_UNDERGROUND) - GetRValue(COLOR_GROUND)) * t);
    int cg = GetGValue(COLOR_GROUND) +
             (int)((GetGValue(COLOR_UNDERGROUND) - GetGValue(COLOR_GROUND)) * t);
    int cb = GetBValue(COLOR_GROUND) +
             (int)((GetBValue(COLOR_UNDERGROUND) - GetBValue(COLOR_GROUND)) * t);
    ex_setlinecolor(RGB(cr, cg, cb));
    ex_line(0, y, WINDOW_WIDTH, y);
  }
}

void render_player(RenderContext *ctx, Player *player) {
  float tip_x;
  float tip_y;
  float display_length;
  int ox;
  int oy;

  (void)ctx;
  if (player == NULL) {
    return;
  }

  ox = (int)player->origin_x;
  oy = (int)player->origin_y;

  ex_setfillcolor(RGB(50, 50, 200));
  ex_solidrectangle(ox - 15, oy - 45, ox + 15, oy - 15);

  ex_setfillcolor(RGB(255, 220, 180));
  draw_filled_circle(ox, oy - 55, 12);

  ex_setfillcolor(RGB(160, 82, 45));
  ex_solidrectangle(ox - 14, oy - 70, ox + 14, oy - 62);

  display_length = player->length;
  if (player->state == HOOK_SWINGING) {
    display_length = 45.0f;
  }
  if (display_length < 10.0f && player->state != HOOK_SWINGING) {
    display_length = 10.0f;
  }

  tip_x = player->origin_x + display_length * sinf(player->angle);
  tip_y = player->origin_y + display_length * cosf(player->angle);

  ex_setlinecolor(RGB(200, 200, 200));
  ex_setlinestyle(PS_SOLID, 3);
  ex_line(ox, oy, (int)tip_x, (int)tip_y);

  ex_setlinecolor(RGB(220, 50, 50));
  ex_line((int)tip_x, (int)tip_y,
       (int)(tip_x - 12.0f * cosf(player->angle + 0.6f)),
       (int)(tip_y + 12.0f * sinf(player->angle + 0.6f)));
  ex_line((int)tip_x, (int)tip_y,
       (int)(tip_x + 12.0f * cosf(player->angle - 0.6f)),
       (int)(tip_y + 12.0f * sinf(player->angle - 0.6f)));
  ex_setfillcolor(RGB(255, 80, 80));
  draw_filled_circle((int)tip_x, (int)tip_y, 4);
  ex_setlinestyle(PS_SOLID, 1);

  if (player->grabbed != NULL) {
    float grab_x;
    float grab_y;
    int rad;

    player_get_hook_tip(player, &grab_x, &grab_y);
    rad = (int)(player->grabbed->radius * 0.7f);
    if (rad < 8) {
      rad = 8;
    }

    switch (player->grabbed->type) {
    case OBJ_GOLD_SMALL:
    case OBJ_GOLD_LARGE:
      ex_setfillcolor(RGB(255, 215, 0));
      draw_filled_circle((int)grab_x, (int)grab_y, rad);
      break;
    case OBJ_DIAMOND:
      draw_diamond((int)grab_x, (int)grab_y, rad, RGB(0, 255, 255),
                   RGB(255, 255, 255));
      break;
    case OBJ_STONE:
      ex_setfillcolor(RGB(128, 128, 128));
      ex_solidrectangle((int)grab_x - rad, (int)grab_y - rad, (int)grab_x + rad,
                     (int)grab_y + rad);
      break;
    case OBJ_MYSTERY_BAG:
      ex_setfillcolor(RGB(139, 90, 43));
      draw_filled_circle((int)grab_x, (int)grab_y, rad);
      break;
    default:
      break;
    }
  }
}

void render_objects(RenderContext *ctx, LinkedList *objects) {
  Node *cur;
  (void)ctx;

  if (objects == NULL) {
    return;
  }

  cur = objects->head;
  while (cur != NULL) {
    GameObject *obj = (GameObject *)cur->data;
    if (!obj->collected) {
      render_single_object(obj);
    }
    cur = cur->next;
  }
}

void render_hud(RenderContext *ctx, int score, int target, float time_left,
                int level) {
  char buffer[128];
  (void)ctx;

  ex_setfillcolor(RGB(30, 30, 30));
  ex_solidrectangle(10, 10, 270, 90);
  ex_setlinecolor(RGB(210, 180, 60));
  ex_rectangle(10, 10, 270, 90);

  snprintf(buffer, sizeof(buffer), i18n_get("HUD_LEVEL"), level);
  draw_text_line(24, 18, 22, RGB(255, 255, 255), buffer);

  snprintf(buffer, sizeof(buffer), i18n_get("HUD_SCORE"), score, target);
  draw_text_line(24, 44, 20, RGB(255, 215, 0), buffer);

  snprintf(buffer, sizeof(buffer), i18n_get("HUD_TIME"), (int)time_left);
  draw_text_line(24, 66, 20,
                 (time_left < 10.0f) ? RGB(255, 80, 80) : RGB(255, 255, 255),
                 buffer);
}

void render_menu(RenderContext *ctx, int highscore) {
  char buffer[128];
  (void)ctx;

  render_background(ctx);

  ex_setfillcolor(RGB(0, 0, 0));
  ex_solidrectangle(150, 140, 650, 430);
  ex_setlinecolor(RGB(255, 215, 0));
  ex_rectangle(150, 140, 650, 430);

  draw_text_center(WINDOW_WIDTH / 2, 175, 42, RGB(255, 215, 0),
                   i18n_get("MENU_TITLE"));
  draw_text_center(WINDOW_WIDTH / 2, 245, 26, RGB(255, 255, 255),
                   i18n_get("MENU_START"));

  if (highscore > 0) {
    snprintf(buffer, sizeof(buffer), i18n_get("MENU_HIGH_SCORE"), highscore);
    draw_text_center(WINDOW_WIDTH / 2, 290, 24, RGB(180, 255, 180), buffer);
  }

  draw_text_center(WINDOW_WIDTH / 2, 335, 20, RGB(220, 220, 220),
                   i18n_get("MENU_INSTR_1"));
  draw_text_center(WINDOW_WIDTH / 2, 365, 20, RGB(220, 220, 220),
                   i18n_get("MENU_INSTR_2"));
  draw_text_center(WINDOW_WIDTH / 2, 395, 20, RGB(180, 220, 255),
                   i18n_get("MENU_LANG_TOGGLE"));
}

void render_shop(RenderContext *ctx, LinkedList *items, int score,
                 int selected) {
  Node *cur;
  int i;
  int y;
  char buffer[128];

  (void)ctx;
  ex_setfillcolor(RGB(28, 30, 45));
  ex_solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  draw_text_center(WINDOW_WIDTH / 2, 36, 38, RGB(255, 215, 0),
                   i18n_get("SHOP_TITLE"));
  snprintf(buffer, sizeof(buffer), i18n_get("SHOP_GOLD"), score);
  draw_text_center(WINDOW_WIDTH / 2, 84, 24, RGB(255, 255, 255), buffer);
  draw_text_center(WINDOW_WIDTH / 2, 118, 18, RGB(180, 180, 200),
                   i18n_get("SHOP_INSTR"));

  cur = items->head;
  i = 0;
  y = 180;
  while (cur != NULL) {
    ShopItem *item = (ShopItem *)cur->data;
    const char *name_key = "";
    const char *desc_key = "";
    COLORREF panel = (i == selected) ? RGB(70, 90, 130) : RGB(55, 60, 85);

    if (item->type == ITEM_STRENGTH) {
      name_key = "ITEM_STR_NAME";
      desc_key = "ITEM_STR_DESC";
    } else if (item->type == ITEM_LUCKY) {
      name_key = "ITEM_LUCKY_NAME";
      desc_key = "ITEM_LUCKY_DESC";
    } else if (item->type == ITEM_BOMB) {
      name_key = "ITEM_BOMB_NAME";
      desc_key = "ITEM_BOMB_DESC";
    }

    ex_setfillcolor(panel);
    ex_solidrectangle(120, y, 680, y + 88);
    ex_setlinecolor((i == selected) ? RGB(255, 215, 0) : RGB(120, 130, 170));
    ex_rectangle(120, y, 680, y + 88);

    draw_text_line(145, y + 14, 26, RGB(255, 255, 255), i18n_get(name_key));
    draw_text_line(145, y + 46, 18, RGB(220, 220, 220), i18n_get(desc_key));

    if (item->owned) {
      snprintf(buffer, sizeof(buffer), "[%s]", i18n_get("SHOP_OWNED"));
      draw_text_line(560, y + 28, 22, RGB(150, 255, 150), buffer);
    } else {
      snprintf(buffer, sizeof(buffer), i18n_get("SHOP_PRICE"), item->price);
      draw_text_line(585, y + 28, 22, RGB(255, 215, 0), buffer);
    }

    y += 102;
    i++;
    cur = cur->next;
  }
}

void render_level_complete(RenderContext *ctx, int score, int target,
                           int level) {
  char buffer[128];
  (void)target;

  render_background(ctx);
  ex_setfillcolor(RGB(20, 30, 20));
  ex_solidrectangle(160, 160, 640, 400);
  ex_setlinecolor(RGB(120, 255, 120));
  ex_rectangle(160, 160, 640, 400);

  draw_text_center(WINDOW_WIDTH / 2, 195, 40, RGB(120, 255, 120),
                   i18n_get("LC_TITLE"));
  snprintf(buffer, sizeof(buffer), i18n_get("LC_SUBTITLE"), level);
  draw_text_center(WINDOW_WIDTH / 2, 255, 24, RGB(255, 255, 255), buffer);
  snprintf(buffer, sizeof(buffer), i18n_get("LC_SCORE"), score, target);
  draw_text_center(WINDOW_WIDTH / 2, 295, 22, RGB(255, 215, 0), buffer);
  draw_text_center(WINDOW_WIDTH / 2, 340, 20, RGB(220, 220, 220),
                   i18n_get("LC_CONTINUE"));
}

void render_game_over(RenderContext *ctx, int score, int highscore) {
  char buffer[128];

  render_background(ctx);
  ex_setfillcolor(RGB(35, 10, 10));
  ex_solidrectangle(150, 150, 650, 420);
  ex_setlinecolor(RGB(255, 100, 100));
  ex_rectangle(150, 150, 650, 420);

  draw_text_center(WINDOW_WIDTH / 2, 185, 40, RGB(255, 100, 100),
                   i18n_get("GO_TITLE"));
  snprintf(buffer, sizeof(buffer), i18n_get("GO_FINAL"), score);
  draw_text_center(WINDOW_WIDTH / 2, 250, 24, RGB(255, 255, 255), buffer);

  if (score > highscore) {
    draw_text_center(WINDOW_WIDTH / 2, 292, 22, RGB(255, 215, 0),
                     i18n_get("GO_NEW_HIGH"));
  } else if (highscore > 0) {
    snprintf(buffer, sizeof(buffer), i18n_get("GO_HIGH"), highscore);
    draw_text_center(WINDOW_WIDTH / 2, 292, 22, RGB(255, 215, 0), buffer);
  }

  draw_text_center(WINDOW_WIDTH / 2, 345, 20, RGB(220, 220, 220),
                   i18n_get("GO_RESTART"));
}
