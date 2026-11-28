# OpenGL 贪吃蛇
面向初学者的 OpenGL 贪吃蛇小游戏，使用 OpenGL 3.3 Core、GLFW、GLAD、GLM 和 stb_image。
## 功能
- 格子移动的蛇，带贴图绘制
- 吃果实变长
- 背景贴图与网格线
- 撞到自己判定
- Game Over 画面
- 碰到边界后从对面穿出
## 操作
| 按键 | 作用 |
|------|------|
| `W` / `↑` | 向上 |
| `A` / `←` | 向左 |
| `S` / `↓` | 向下 |
| `D` / `→` | 向右 |
| `Esc` | 退出（Game Over 之后） |
## 环境要求
- CMake ≥ 3.16
- 支持 C++17 的编译器
- OpenGL 3.3 及以上
- GLFW 3
  已放在 `third_party/` 里的依赖：
- GLAD
- GLM
- stb_image
## 编译与运行
```bash
cmake -S . -B build
cmake --build build
./build/GameStart
如果用自定义构建目录（例如 CLion）：


cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
./cmake-build-debug/GameStart
编译后会把 Shaders/ 和 Assets/ 复制到可执行文件旁边。请在输出目录运行程序，或保证这两个文件夹和可执行文件在同一目录。

目录结构

.
├── Assets/              # 贴图（蛇、果实、背景、结束画面）
├── Shaders/             # GLSL 顶点 / 片元着色器
├── src/
│   ├── main.cpp         # 入口与主循环
│   ├── Snake.*          # 蛇的逻辑、移动、绘制
│   ├── Fruit.*          # 果实生成与绘制
│   ├── Game.*           # 网格生成与绘制
│   ├── backGround.*     # 背景四边形与贴图
│   ├── GameOver.*       # 结束画面
│   ├── Shader.*         # 着色器加载 / 编译 / uniform
│   ├── Texture.*        # 贴图加载（stb_image）
│   ├── Camera.h         # 相机相关
│   └── Direction.h      # 移动方向
├── third_party/         # glad、glm、stb
└── CMakeLists.txt
技术说明
窗口大小：800×600
投影：正交投影 (-8, 8) × (-6, 6)
蛇的步进间隔：Snake.h 里的 stepInterval（默认 0.2 秒）
贴图用 stb_image 加载；PNG 用 RGBA，JPEG 用 RGB
许可
仅供学习与个人使用。