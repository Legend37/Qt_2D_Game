# Qt 2D Game

一个基于Qt6的2D游戏项目，包含角色、武器、装备、药品等游戏元素。

## 构建要求

- CMake 3.28 或更高版本
- Qt6 (Core, Gui, Widgets)
- C++17 支持的编译器

## 本地构建

### Windows (使用 Visual Studio)
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Linux/macOS
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

## GitHub Actions 自动构建

本项目配置了两个 GitHub Actions 工作流：

### 1. 完整构建 (`build.yml`)
- **触发条件**: 推送到 `master`/`main`/`develop` 分支，或创建 Pull Request
- **支持平台**: Windows、Linux、macOS
- **功能**: 
  - 跨平台编译
  - 上传构建产物
  - 自动发布 (当推送标签时)

### 2. 快速构建 (`quick-build.yml`)
- **触发条件**: 推送到 `master`/`main` 分支，或创建 Pull Request
- **支持平台**: 仅 Windows
- **功能**: 
  - 快速验证构建
  - 缓存 Qt 安装以加速构建
  - 上传可执行文件

## 手动触发构建

您可以在 GitHub 仓库的 "Actions" 标签页中手动触发构建：

1. 进入 Actions 页面
2. 选择相应的工作流
3. 点击 "Run workflow" 按钮

## 下载构建产物

构建完成后，您可以在 Actions 页面的相应运行中下载构建产物：

- `qt-2d-game-windows` - Windows 可执行文件
- `qt-2d-game-linux` - Linux 可执行文件
- `qt-2d-game-macos` - macOS 可执行文件

## 项目结构

```
├── src/                    # 源代码
│   ├── main.cpp           # 程序入口
│   ├── MyGame.cpp/h       # 游戏主类
│   ├── Items/             # 游戏物品
│   │   ├── Characters/    # 角色类
│   │   ├── Weapons/       # 武器类
│   │   ├── Armors/        # 护甲类
│   │   ├── Medicines/     # 药品类
│   │   └── Maps/          # 地图类
│   └── Scenes/            # 游戏场景
├── assets/                # 游戏资源
└── .github/workflows/     # GitHub Actions 配置
```

## 游戏特性

- 角色控制和移动
- 重力物理系统
- 武器系统（手枪、霰弹枪、冲锋枪、匕首、铅球）
- 药品系统（绷带、医疗包、肾上腺素）
- 装备系统（护甲、头盔、裤子）
- 平台跳跃
- 碰撞检测
- 生命值系统

## 开发说明

- 使用 Qt6 图形框架
- CMake 构建系统
- C++17 标准
- 面向对象设计模式
- 组件化架构
