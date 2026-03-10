/*
 * game.c - 游戏状态机实现
 *
 * 协调所有模块: player, objects, level, shop, render, fileio。
 * 管理 MENU → PLAYING → LEVEL_COMPLETE → SHOP → PLAYING → ... → GAME_OVER
 * 流程。
 */

#include "game.h"
#include "fileio.h"
#include "i18n.h"
#include "objects.h"
#include "shop.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HIGHSCORE_FILE "highscore.txt"
#define LEVELS_FILE "data/levels.txt"

/* 释放 GameObject 的回调 */
static void free_object(void *data, void *ctx) {
  (void)ctx;
  object_destroy((GameObject *)data);
}

/* 释放 ShopItem 的回调 */
static void free_shop_item(void *data, void *ctx) {
  (void)ctx;
  free(data);
}

/* ---- 启动一个关卡 ---- */
static void start_level(Game *game) {
  LevelConfig *cfg;

  list_clear(&game->objects, free_object);

  cfg = &game->levels[game->current_level];
  level_create_objects(cfg, &game->objects);
  game->time_left = (float)cfg->time_limit;

  game->player.state = HOOK_SWINGING;
  game->player.angle = 0.0f;
  game->player.length = 0.0f;
  game->player.grabbed = NULL;
  game->player.swing_dir = 1.0f;

  shop_apply_effects(&game->shop_items, &game->player);

  game->state = STATE_PLAYING;
}

int game_init(Game *game) {
  if (game == NULL) {
    return 0;
  }

  srand((unsigned int)time(NULL));
  i18n_init();

  if (!render_init(&game->render_ctx)) {
    return 0;
  }

  player_init(&game->player);
  list_init(&game->objects);
  shop_init(&game->shop_items);

  game->total_levels =
      fileio_load_levels(LEVELS_FILE, game->levels, MAX_LEVELS);
  if (game->total_levels == 0) {
    printf("Warning: No levels loaded, using default level\n");
    game->total_levels = 1;
    game->levels[0].level_num = 1;
    game->levels[0].target_score = 500;
    game->levels[0].time_limit = 60;
    game->levels[0].object_count = 5;
    game->levels[0].object_configs[0].type   = OBJ_GOLD_SMALL;
    game->levels[0].object_configs[0].x      = 200;
    game->levels[0].object_configs[0].y      = 300;
    game->levels[0].object_configs[0].radius = 25;
    game->levels[0].object_configs[1].type   = OBJ_GOLD_LARGE;
    game->levels[0].object_configs[1].x      = 400;
    game->levels[0].object_configs[1].y      = 400;
    game->levels[0].object_configs[1].radius = 45;
    game->levels[0].object_configs[2].type   = OBJ_DIAMOND;
    game->levels[0].object_configs[2].x      = 600;
    game->levels[0].object_configs[2].y      = 350;
    game->levels[0].object_configs[2].radius = 15;
    game->levels[0].object_configs[3].type   = OBJ_STONE;
    game->levels[0].object_configs[3].x      = 300;
    game->levels[0].object_configs[3].y      = 250;
    game->levels[0].object_configs[3].radius = 35;
    game->levels[0].object_configs[4].type   = OBJ_GOLD_SMALL;
    game->levels[0].object_configs[4].x      = 550;
    game->levels[0].object_configs[4].y      = 500;
    game->levels[0].object_configs[4].radius = 25;
  }

  game->current_level = 0;
  game->highscore = fileio_load_highscore(HIGHSCORE_FILE);
  game->shop_selected = 0;
  game->victory = 0;
  game->state = STATE_MENU;

  return 1;
}

void game_cleanup(Game *game) {
  if (game == NULL) {
    return;
  }

  if (game->player.score > game->highscore) {
    fileio_save_highscore(HIGHSCORE_FILE, game->player.score);
  }

  list_clear(&game->objects, free_object);
  list_clear(&game->shop_items, free_shop_item);
  render_cleanup(&game->render_ctx);
}

void game_handle_input(Game *game, InputKey key) {
  if (game == NULL || key == KEY_NONE) {
    return;
  }

  switch (game->state) {
  case STATE_MENU:
    if (key == KEY_SPACE) {
      player_init(&game->player);
      game->current_level = 0;
      game->victory = 0;
      shop_reset_owned(&game->shop_items);
      shop_reset_effects(&game->player);
      start_level(game);
    } else if (key == KEY_L) {
      i18n_toggle_language();
    }
    break;

  case STATE_PLAYING:
    if (key == KEY_SPACE || key == KEY_DOWN) {
      player_shoot(&game->player);
    }
    break;

  case STATE_LEVEL_COMPLETE:
    if (key == KEY_SPACE) {
      if (game->current_level + 1 < game->total_levels) {
        game->shop_selected = 0;
        shop_reset_effects(&game->player);
        shop_reset_owned(&game->shop_items);
        game->state = STATE_SHOP;
      } else {
        game->victory = 1;
        if (game->player.score > game->highscore) {
          game->highscore = game->player.score;
          fileio_save_highscore(HIGHSCORE_FILE, game->highscore);
        }
        game->state = STATE_GAME_OVER;
      }
    }
    break;

  case STATE_SHOP:
    if (key == KEY_UP) {
      game->shop_selected--;
      if (game->shop_selected < 0) {
        game->shop_selected = list_count(&game->shop_items) - 1;
      }
    } else if (key == KEY_DOWN) {
      game->shop_selected++;
      if (game->shop_selected >= list_count(&game->shop_items)) {
        game->shop_selected = 0;
      }
    } else if (key == KEY_SPACE) {
      Node *cur = game->shop_items.head;
      int i = 0;
      while (cur != NULL && i < game->shop_selected) {
        cur = cur->next;
        i++;
      }
      if (cur != NULL) {
        ShopItem *item = (ShopItem *)cur->data;
        shop_buy(&game->shop_items, item->type, &game->player.score);
      }
    } else if (key == KEY_ENTER) {
      game->current_level++;
      start_level(game);
    }
    break;

  case STATE_GAME_OVER:
    if (key == KEY_SPACE) {
      game->state = STATE_MENU;
    }
    break;
  }
}

void game_update(Game *game, float dt) {
  GameObject *hit;
  int time_up;
  int no_valuables;
  int hook_idle;

  if (game == NULL || game->state != STATE_PLAYING) {
    return;
  }

  game->time_left -= dt;

  time_up = (game->time_left <= 0.0f);
  no_valuables = !level_has_valuable_objects(&game->objects);
  hook_idle = (game->player.state == HOOK_SWINGING);

  if (time_up || (no_valuables && hook_idle)) {
    if (time_up) {
      game->time_left = 0.0f;
    }

    if (level_is_complete(game->player.score,
                          game->levels[game->current_level].target_score)) {
      game->state = STATE_LEVEL_COMPLETE;
    } else {
      if (game->player.score > game->highscore) {
        game->highscore = game->player.score;
        fileio_save_highscore(HIGHSCORE_FILE, game->highscore);
      }
      game->state = STATE_GAME_OVER;
    }
    return;
  }

  player_update(&game->player, dt);

  hit = player_check_collision(&game->player, &game->objects);
  (void)hit;
}

void game_render(Game *game) {
  render_clear(&game->render_ctx);

  switch (game->state) {
  case STATE_MENU:
    render_menu(&game->render_ctx, game->highscore);
    break;

  case STATE_PLAYING:
    render_background(&game->render_ctx);
    render_objects(&game->render_ctx, &game->objects);
    render_player(&game->render_ctx, &game->player);
    render_hud(&game->render_ctx, game->player.score,
               game->levels[game->current_level].target_score, game->time_left,
               game->levels[game->current_level].level_num);
    break;

  case STATE_LEVEL_COMPLETE:
    render_level_complete(&game->render_ctx, game->player.score,
                          game->levels[game->current_level].target_score,
                          game->levels[game->current_level].level_num);
    break;

  case STATE_SHOP:
    render_shop(&game->render_ctx, &game->shop_items, game->player.score,
                game->shop_selected);
    break;

  case STATE_GAME_OVER:
    render_game_over(&game->render_ctx, game->player.score, game->highscore);
    break;
  }

  render_present(&game->render_ctx);
}
