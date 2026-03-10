/*
 * fileio.h - 文件读写模块
 *
 * 处理关卡配置加载和最高分持久化。
 */

#ifndef FILEIO_H
#define FILEIO_H

#include "level.h"
#include "types.h"

/* 从文件加载关卡配置，返回实际加载的关卡数 */
int fileio_load_levels(const char *filename, LevelConfig *configs,
                       int max_levels);

/* 保存最高分到文件，成功返回 1 */
int fileio_save_highscore(const char *filename, int score);

/* 从文件读取最高分，文件不存在返回 0 */
int fileio_load_highscore(const char *filename);

#endif /* FILEIO_H */
