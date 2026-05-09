# MaaYYS

基于 MaaAssistantArknights (MAA) 框架开发的阴阳师自动化助手。

## 项目简介

MaaYYS 是一个模块化的阴阳师游戏自动化框架，继承自 MAA 的核心架构，专门针对阴阳师游戏进行适配和扩展。

## 目录结构

```
MaaAssistantYYS/
├── src/
│   ├── MaaCore/                    # MAA 核心模块
│   ├── MaaYYS/                     # 阴阳师模块
│   │   ├── Base/                   # 模块基类
│   │   ├── Activity/               # 活动任务
│   │   ├── Daily/                  # 日常任务
│   │   └── Common/                 # 公共组件
│   └── MaaGui/                     # GUI 界面
├── resource/
│   └── YYS/                        # 阴阳师资源
│       ├── base/                   # 基础资源
│       ├── daily/                  # 日常资源
│       └── activity/               # 活动资源
├── CMakeLists.txt
└── README.md
```

## 核心模块

### 1. Base 模块
- `YYSModule` - 所有模块的基类
- `YYSContext` - 全局上下文，管理 Controller、Vision 等
- `IYYSActivity` - 活动任务接口
- `IYYSDaily` - 日常任务接口

### 2. Activity 模块
- `YYSActivityManager` - 活动任务管理器
- `YYSBaseActivity` - 活动任务基类

### 3. Daily 模块
- `YYSDailyManager` - 日常任务管理器
- `YYSDailyTask` - 日常任务基类

## 编译

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 系统要求

- C++20
- CMake 3.20+
- OpenCV 4.0+
- Windows 10/11

## 开发计划

- [ ] Sprint 1: 项目骨架搭建
- [ ] Sprint 2: 核心模块实现
- [ ] Sprint 3: 日常任务适配
- [ ] Sprint 4: 活动任务适配

## 许可证

继承 MAA 许可证
