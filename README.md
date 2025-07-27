# Qt 2D Game

一个基于Qt6的2D游戏项目，包含角色、武器、装备、药品等游戏元素。
## [游戏文档]("https://github.com/Legend37/Qt_2D_Game/wiki")

## 本地构建

### Windows 

自行安装**Qt 6.5.3**和**CMake**后编译。

### macOS

首先检查是否正确安装`homebrew`，如果没有安装，则运行

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

安装Qt

```bash
brew install qt
```

安装CMake

```
brew install cmake
```

下载完整sourcecode并解压，进入到文件夹后用终端执行

```bash
mkdir build
cd build
cmake ..
make
```

可以看到已经生成了可执行文件**qt_2025_game**
