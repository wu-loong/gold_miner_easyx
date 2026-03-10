/*
 * render.h - 渲染模块
 *
 * 所有 EasyX 绘制集中在此模块，实现视图与逻辑分离。
 */

#ifndef RENDER_H
#define RENDER_H

#include "list.h"
#include "player.h"
#include "types.h"

/* ==================== 渲染上下文 ==================== */
typedef struct {
  int initialized;
} RenderContext;

/* ==================== 公开接口 ==================== */

int render_init(RenderContext *ctx);
void render_cleanup(RenderContext *ctx);
void render_clear(RenderContext *ctx);
void render_present(RenderContext *ctx);

void render_background(RenderContext *ctx);
void render_player(RenderContext *ctx, Player *player);
void render_objects(RenderContext *ctx, LinkedList *objects);
void render_hud(RenderContext *ctx, int score, int target, float time_left,
                int level);
void render_menu(RenderContext *ctx, int highscore);
void render_shop(RenderContext *ctx, LinkedList *items, int score,
                 int selected);
void render_level_complete(RenderContext *ctx, int score, int target,
                           int level);
void render_game_over(RenderContext *ctx, int score, int highscore);

#endif /* RENDER_H */
