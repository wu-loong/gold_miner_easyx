/*
 * fileio.c - 文件读写模块实现
 *
 * 关卡文件格式:
 *   LEVEL <编号> <目标分数> <时间秒>
 *   OBJ <类型名> <x> <y> <半径>
 *   ...
 *   END
 *
 * 最高分文件格式: 单行整数
 */

#include "fileio.h"
#include <stdio.h>
#include <string.h>

/* ---- 辅助: 类型名称 → 枚举 ---- */
static ObjectType parse_object_type(const char *name) {
  if (strcmp(name, "GOLD_SMALL") == 0)
    return OBJ_GOLD_SMALL;
  if (strcmp(name, "GOLD_LARGE") == 0)
    return OBJ_GOLD_LARGE;
  if (strcmp(name, "DIAMOND") == 0)
    return OBJ_DIAMOND;
  if (strcmp(name, "STONE") == 0)
    return OBJ_STONE;
  if (strcmp(name, "MYSTERY_BAG") == 0)
    return OBJ_MYSTERY_BAG;
  return OBJ_STONE; /* 默认 */
}

/* ---- 加载关卡 ---- */
int fileio_load_levels(const char *filename, LevelConfig *configs,
                       int max_levels) {
  FILE *fp;
  char line[256];
  int level_count = 0;
  LevelConfig *cur = NULL;

  if (filename == NULL || configs == NULL) {
    return 0;
  }

  fp = fopen(filename, "r");
  if (fp == NULL) {
    return 0;
  }

  while (fgets(line, sizeof(line), fp) != NULL && level_count < max_levels) {
    char keyword[32];

    /* 跳过注释和空行 */
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
      continue;
    }

    if (sscanf(line, "%31s", keyword) != 1) {
      continue;
    }

    if (strcmp(keyword, "LEVEL") == 0) {
      cur = &configs[level_count];
      cur->object_count = 0;
      sscanf(line, "LEVEL %d %d %d", &cur->level_num, &cur->target_score,
             &cur->time_limit);
    } else if (strcmp(keyword, "OBJ") == 0 && cur != NULL) {
      if (cur->object_count < MAX_OBJECTS) {
        char type_name[32];
        float x, y, radius;
        sscanf(line, "OBJ %31s %f %f %f", type_name, &x, &y, &radius);
        cur->object_configs[cur->object_count].type =
            parse_object_type(type_name);
        cur->object_configs[cur->object_count].x = x;
        cur->object_configs[cur->object_count].y = y;
        cur->object_configs[cur->object_count].radius = radius;
        cur->object_count++;
      }
    } else if (strcmp(keyword, "END") == 0 && cur != NULL) {
      level_count++;
      cur = NULL;
    }
  }

  fclose(fp);
  return level_count;
}

/* ---- 保存最高分 ---- */
int fileio_save_highscore(const char *filename, int score) {
  FILE *fp;

  if (filename == NULL) {
    return 0;
  }

  fp = fopen(filename, "w");
  if (fp == NULL) {
    return 0;
  }

  fprintf(fp, "%d\n", score);
  fclose(fp);
  return 1;
}

/* ---- 读取最高分 ---- */
int fileio_load_highscore(const char *filename) {
  FILE *fp;
  int score = 0;

  if (filename == NULL) {
    return 0;
  }

  fp = fopen(filename, "r");
  if (fp == NULL) {
    return 0;
  }

  fscanf(fp, "%d", &score);
  fclose(fp);
  return score;
}
