/*
 * types.h - 黄金矿工游戏 - 公共类型与常量定义
 *
 * 设计理由:
 *   所有模块共享的常量和类型集中定义在这里，避免循环依赖
 *   和重复定义。每个 .c 文件只需 #include "types.h" 即可。
 */

#ifndef TYPES_H
#define TYPES_H

/* ==================== 窗口与帧率 ==================== */
#define WINDOW_WIDTH 800  /* 游戏窗口宽度（像素） */
#define WINDOW_HEIGHT 600 /* 游戏窗口高度（像素） */
#define FPS 60            /* 目标帧率 */

/* ==================== 游戏区域布局 ==================== */
#define GROUND_Y 100      /* 地面 Y 坐标（地上/地下分界线） */
#define HOOK_ORIGIN_X 400 /* 钩子锚点 X（屏幕中央） */
#define HOOK_ORIGIN_Y 80  /* 钩子锚点 Y（地面上方） */

/* ==================== 钩子参数 ==================== */
#define HOOK_RADIUS 8             /* 钩子碰撞半径 */
#define HOOK_MAX_LENGTH 550.0f    /* 绳索最大长度 */
#define HOOK_EXTEND_SPEED 300.0f  /* 钩子伸出速度（像素/秒） */
#define HOOK_RETRACT_SPEED 200.0f /* 基础缩回速度（像素/秒） */
#define HOOK_SWING_SPEED 1.5f     /* 摆动角速度（弧度/秒） */
#define HOOK_MAX_ANGLE 1.3f       /* 最大摆动角度（约 75 度） */

/* ==================== 关卡 ==================== */
#define MAX_LEVELS 10  /* 最大关卡数 */
#define MAX_OBJECTS 20 /* 每关最大物品数 */

/* ==================== 游戏状态枚举 ==================== */
/*
 * 游戏状态机的五种状态:
 *   MENU           → 主菜单(等待开始)
 *   PLAYING        → 游戏进行中
 *   LEVEL_COMPLETE → 关卡完成过渡
 *   SHOP           → 商店(购买道具)
 *   GAME_OVER      → 游戏结束
 */
typedef enum {
  STATE_MENU,
  STATE_PLAYING,
  STATE_LEVEL_COMPLETE,
  STATE_SHOP,
  STATE_GAME_OVER
} GameState;

/* ==================== 钩子状态枚举 ==================== */
/*
 * 钩子的三种状态:
 *   SWINGING   → 左右摆动，等待发射
 *   EXTENDING  → 向目标方向伸出
 *   RETRACTING → 缩回（可能带着物品）
 */
typedef enum { HOOK_SWINGING, HOOK_EXTENDING, HOOK_RETRACTING } HookState;

/* ==================== 物品类型枚举 ==================== */
/*
 * 五种矿物/物品类型:
 *   GOLD_SMALL   → 小金块: 轻(2), 低分(50)
 *   GOLD_LARGE   → 大金块: 重(6), 高分(250)
 *   DIAMOND      → 钻石:   轻(1), 超高分(600)
 *   STONE        → 石头:   最重(8), 极低分(10)
 *   MYSTERY_BAG  → 神秘袋: 中(3), 随机分(100-500)
 */
typedef enum {
  OBJ_GOLD_SMALL,
  OBJ_GOLD_LARGE,
  OBJ_DIAMOND,
  OBJ_STONE,
  OBJ_MYSTERY_BAG,
  OBJ_TYPE_COUNT /* 类型总数，方便遍历 */
} ObjectType;

/* ==================== 商店道具类型 ==================== */
typedef enum {
  ITEM_STRENGTH, /* 力量药水 - 加快缩回速度 */
  ITEM_LUCKY,    /* 幸运草   - 额外分数加成 */
  ITEM_BOMB,     /* 炸弹     - 石头直接炸掉 */
  ITEM_TYPE_COUNT
} ItemType;

#endif /* TYPES_H */
