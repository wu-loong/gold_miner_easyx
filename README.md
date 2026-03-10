# Gold Miner EasyX

一个基于 EasyX 的 Windows 小游戏工程，主体逻辑使用 C 实现，并通过一层很薄的 C++ 桥接接入 EasyX 图形接口。

## 项目特点

- 游戏主逻辑位于 `src/*.c`
- EasyX 适配层位于 `src/easyx_bridge.cpp`
- 已包含 Visual Studio 工程文件
- 已内置 EasyX 所需头文件和库文件

## 目录结构

```text
gold_miner_easyx/
├─ data/
│  └─ levels.txt
├─ gold_miner_easyx.sln
├─ gold_miner_easyx.vcxproj
├─ README.md
├─ src/
│  ├─ main.c
│  ├─ game.c
│  ├─ render.c
│  └─ ...
└─ third_party/
   └─ easyx/
      ├─ include/
      └─ lib/x64/
```

## 运行环境

- Windows
- Visual Studio 2022
- MSVC v143 工具链
- Windows SDK 10

当前工程已配置为 `x64`，并默认使用 C17 编译 `.c` 文件。

## 如何编译

1. 在 Windows 上打开根目录下的 `gold_miner_easyx.sln`
2. 选择 `Debug | x64` 或 `Release | x64`
3. 直接生成并运行

工程输出目录默认是：

- `dist/Debug/`
- `dist/Release/`

中间文件默认输出到：

- `dist-int/Debug/`
- `dist-int/Release/`

## 运行所需文件

程序运行时依赖以下内容：

- `data/levels.txt`：关卡配置
- `third_party/easyx/include/*`：EasyX 头文件
- `third_party/easyx/lib/x64/EasyXw.lib`：EasyX 链接库

调试工作目录已配置为仓库根目录，因此程序可以直接读取 `data/levels.txt`。

如果 `data/levels.txt` 读取失败，程序会自动使用内置默认关卡启动。

## 操作说明

- `SPACE`：发射钩子 / 商店购买 / 菜单确认
- `UP`：商店上移
- `DOWN`：商店下移；在游戏中也可触发发射
- `ENTER`：进入下一关
- `L`：切换语言
- `ESC`：退出游戏

## 代码说明

- `src/main.c`：程序入口、按键轮询、主循环
- `src/game.c`：游戏状态机与关卡流程
- `src/render.c`：渲染逻辑
- `src/player.c`：钩子和玩家行为
- `src/objects.c`：矿物/石头等对象逻辑
- `src/level.c`：关卡对象生成与判定
- `src/shop.c`：商店系统
- `src/fileio.c`：关卡与分数文件读写
- `src/i18n.c`：文案与语言切换
- `src/easyx_bridge.cpp`：C 与 EasyX 之间的桥接层

## 运行时生成文件

程序运行后可能在仓库根目录生成：

- `highscore.txt`：最高分存档

## 说明

EasyX 官方接口本身依赖 C++，所以本项目采用的是“C 主体 + C++ 桥接”的实现方式。这能在尽量保留 C 代码主体的前提下完成图形渲染与窗口交互。
