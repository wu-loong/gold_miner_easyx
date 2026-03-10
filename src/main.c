/*
 * main.c - 黄金矿工游戏入口（Win32 + EasyX 纯 C 版本）
 */

#include "game.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

static InputKey poll_input_edge(void) {
  static SHORT prev_space = 0;
  static SHORT prev_up = 0;
  static SHORT prev_down = 0;
  static SHORT prev_enter = 0;
  static SHORT prev_l = 0;
  SHORT cur;

  cur = GetAsyncKeyState(VK_SPACE);
  if ((cur & 0x8000) && !(prev_space & 0x8000)) {
    prev_space = cur;
    return KEY_SPACE;
  }
  prev_space = cur;

  cur = GetAsyncKeyState(VK_UP);
  if ((cur & 0x8000) && !(prev_up & 0x8000)) {
    prev_up = cur;
    return KEY_UP;
  }
  prev_up = cur;

  cur = GetAsyncKeyState(VK_DOWN);
  if ((cur & 0x8000) && !(prev_down & 0x8000)) {
    prev_down = cur;
    return KEY_DOWN;
  }
  prev_down = cur;

  cur = GetAsyncKeyState(VK_RETURN);
  if ((cur & 0x8000) && !(prev_enter & 0x8000)) {
    prev_enter = cur;
    return KEY_ENTER;
  }
  prev_enter = cur;

  cur = GetAsyncKeyState('L');
  if ((cur & 0x8000) && !(prev_l & 0x8000)) {
    prev_l = cur;
    return KEY_L;
  }
  prev_l = cur;

  return KEY_NONE;
}

int main(void) {
  Game *game;
  ULONGLONG prev_ticks;

  printf("=== Gold Miner (EasyX C) ===\n");
  printf("Controls: SPACE = Shoot/Buy, UP/DOWN = Shop, ENTER = Next, L = Lang, ESC = Quit\n\n");

  game = (Game *)calloc(1, sizeof(Game));
  if (game == NULL) {
    printf("Error: Failed to allocate game memory\n");
    return 1;
  }

  if (!game_init(game)) {
    printf("Error: Failed to initialize game\n");
    free(game);
    return 1;
  }

  printf("Game initialized. %d levels loaded.\n", game->total_levels);

  prev_ticks = GetTickCount64();
  while (1) {
    ULONGLONG now_ticks;
    float dt;
    InputKey key;

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
      break;
    }

    now_ticks = GetTickCount64();
    dt = (float)(now_ticks - prev_ticks) / 1000.0f;
    prev_ticks = now_ticks;

    if (dt > 0.05f) {
      dt = 0.05f;
    }

    key = poll_input_edge();
    game_handle_input(game, key);
    game_update(game, dt);
    game_render(game);

    Sleep(1000 / FPS);
  }

  game_cleanup(game);
  free(game);
  return 0;
}
