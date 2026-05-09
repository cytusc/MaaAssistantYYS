# 框架设计

<!--
文档版本: 2.1
最后更新: 2026-05-09
更新内容: Sprint 1 完成 - MaaTemplateResolver OpenCV 条件编译实现，match_template_opencv() 真实匹配，
          set_image/set_image_from_raw 截图输入，TemplateMatchVerify 验证工具
负责人: AI Assistant
-->

## 1. 总体架构

MaaAssistantYYS 采用"MAA 底座 + YYS 业务层 + 资源配置层"的架构。

```text
MaaAssistantYYS
├── MaaCore / MaaUtils       # 从 MAA 接入的底座能力（已复制，未实际桥接）
├── MaaYYS                   # 阴阳师业务层
│   ├── Base                 # 抽象接口和上下文
│   ├── Common               # 通用控制、识别、流程工具
│   ├── Daily                # 日常任务
│   ├── Activity             # 活动任务
│   └── Tools                # dry-run 和开发工具
└── resource/YYS             # 阴阳师资源配置
```

**重要说明**：虽然 `src/MaaCore/` 和 `src/MaaUtils/` 已复制到项目中，且 include 路径已配置，但 MaaYYS 业务代码 **没有任何 #include 引用 MaaCore 或 MaaUtils 的头文件**。MaaUtils 的 `target_link_libraries` 在 CMakeLists.txt 中被注释掉。当前 MaaYYS 是完全独立的代码，仅通过 `meojson`（MaaUtils 子模块）间接依赖 MaaUtils。

## 2. 分层设计

### 2.1 底座层

底座层来自 MAA，主要包括：

- 设备连接
- 截图
- 点击、滑动、输入
- 模板匹配
- OCR
- 日志与通用工具

底座层不应混入阴阳师业务概念。

**当前状态**：底座层代码存在于 `src/MaaCore/` 和 `src/MaaUtils/`，但 MaaYYS 未直接使用。MaaYYS 通过自建的适配层（`MaaControllerActionExecutor`、`MaaTemplateResolver`）间接对接，适配层当前使用 `std::function` 回调而非直接调用 MaaCore API。

### 2.2 适配层

适配层负责把 MAA 的具体能力封装到 MaaYYS 的抽象接口中。当前核心抽象包括：

- `IActionExecutor`：执行点击、滑动、长按、截图。
- `ITemplateResolver`：根据模板名称查找匹配区域。
- `YYSContext`：任务运行时上下文，持有执行器、模板解析器和配置对象。

业务代码只依赖这些抽象，不直接依赖具体 MAA Controller 或 Vision 类。

**当前状态**：

| 抽象接口 | 测试桩实现 | 真实适配实现 | 桥接 MaaCore |
|----------|-----------|-------------|-------------|
| `IActionExecutor` | `RecordingActionExecutor` ✅ | `MaaControllerActionExecutor` ✅（回调适配器） | ❌ 未绑定 |
| `ITemplateResolver` | `StaticTemplateResolver` ✅ / `ScriptedTemplateResolver` ✅ | `MaaTemplateResolver` ✅（OpenCV 条件编译，降级模式可用） | ❌ 未绑定 |

### 2.3 业务层

业务层分为日常和活动两条线。

```text
Daily
├── YYSDailyTask              # 日常任务基类（继承 YYSModule + IYYSDaily）
├── YYSDailyManager           # 日常任务管理器
└── Tasks
    └── YYSOrochiTask         # 御魂任务（唯一已实现的日常任务）
        ├── OrochiNavigator   # 御魂界面导航
        ├── OrochiLayerSelector # 层数选择器
        ├── OrochiBattleRunner  # 战斗流程管理
        └── RewardHandler     # 奖励处理

Activity
├── YYSBaseActivity           # 活动基类（继承 YYSModule + IYYSActivity）
├── YYSActivityManager        # 活动管理器
└── Activities                # （空，无具体活动实现）
```

日常任务用于稳定功能，活动任务用于短周期变化功能。

**注意**：`YYSBaseActivity` 继承了 `IYYSActivity`，但未实现 `activity_id()` 和 `activity_name()` 纯虚函数，子类必须自行实现。

### 2.4 流程层

流程层封装常见控制结构：

| Flow 类 | 职责 | 实现状态 | 使用状态 |
|---------|------|----------|----------|
| `YYSWaitFlow` | 等待模板出现 | ✅ 已实现 | ❌ 未被业务代码使用 |
| `YYSActionFlow` | 执行点击、滑动等动作 | ✅ 已实现 | ❌ 未被业务代码使用 |
| `YYSRetryFlow` | 重试某段流程 | ✅ 已实现 | ❌ 未被业务代码使用 |
| `YYSLoopFlow` | 循环执行子流程 | ✅ 已实现 | ❌ 未被业务代码使用 |

流程层同样依赖 `YYSContext` 中的抽象接口，而不是直接依赖底层控制器。

**当前问题**：业务代码（Orochi 系列）直接在组件中手写等待循环和重试逻辑，未复用 Flow 组件。`Common/Flow/CMakeLists.txt` 定义了独立的 `YYSFlow` 目标，但未被主 CMakeLists.txt 引用，且存在循环依赖隐患（YYSFlow 链接 MaaYYS，而 MaaYYS 已包含 Flow 源文件）。

## 3. 关键抽象

### 3.1 IActionExecutor

职责：封装设备动作。

```cpp
virtual bool screencap() = 0;
virtual bool click(Point point) = 0;
virtual bool swipe(Point from, Point to) = 0;
virtual bool long_click(Point point, int duration_ms = 800);  // 有默认实现，降级为 click
```

当前已有测试实现：`RecordingActionExecutor`（记录动作到向量）。

当前已有适配实现：`MaaControllerActionExecutor`（基于 `std::function` 回调，**未绑定 MaaCore Controller**）。

后续需要：绑定 MaaCore Controller 的真实实现。

### 3.2 ITemplateResolver

职责：封装模板定位。

```cpp
virtual std::optional<Rect> find_template(const std::string& name) const = 0;
```

当前已有测试实现：

- `StaticTemplateResolver`：静态映射，直接查表返回。
- `ScriptedTemplateResolver`：按序返回预设值，支持状态变化模拟。

当前已有适配实现：`MaaTemplateResolver`（三级降级：自定义匹配函数 → `match_template_opencv()`（OpenCV 条件编译）→ 预定义 ROI）。

- **启用 OpenCV 时**（`-DMAAYYS_USE_OPENCV=ON`）：`match_template_opencv()` 使用 `cv::matchTemplate` 进行真实图像匹配，支持 ROI 裁剪、多分辨率缩放、阈值过滤。
- **未启用 OpenCV 时**：`match_with_maa()` 返回 `nullopt`，降级到预定义 ROI。

截图输入接口：
- `set_image(const cv::Mat&)`：直接设置 cv::Mat 截图（OpenCV 模式）。
- `set_image_from_raw(const void*, int, int, int)`：从原始像素数据创建截图（OpenCV 模式）。
- `set_image(const void*, int, int, int)`：存储截图指针（降级模式）。
- `clear_image()`：清除当前截图。

后续需要：绑定 MaaCore Controller 的真实实现。

### 3.3 YYSContext

职责：承载任务执行依赖。

```cpp
std::shared_ptr<IActionExecutor> executor() const;
std::shared_ptr<ITemplateResolver> resolver() const;
std::shared_ptr<YYSConfig> config() const;  // ⚠️ YYSConfig 未定义
```

所有任务通过 `YYSContext` 获取依赖，避免构造函数参数过长，也避免直接访问全局单例。

**已知问题**：`YYSConfig` 仅有前向声明，项目中不存在 `YYSConfig.h` 或 `YYSConfig.cpp`。调用 `ctx->config()` 并解引用将导致编译/链接错误。

### 3.4 YYSModule

职责：模块基类，提供 `init()` 和 `m_ctx`。

```cpp
virtual std::string name() const = 0;  // 纯虚
virtual bool init(std::shared_ptr<YYSContext> ctx);  // 存储 ctx
```

### 3.5 IYYSDaily / IYYSActivity

职责：日常任务和活动任务的接口。

```cpp
// IYYSDaily
virtual std::string task_id() const = 0;
virtual int priority() const;  // 默认 0
virtual bool run() = 0;
virtual bool check_enable() const;  // 默认 true

// IYYSActivity
virtual std::string activity_id() const = 0;
virtual std::string activity_name() const = 0;
virtual bool run() = 0;
virtual std::vector<std::string> depends() const;  // 默认 {}
```

## 4. 日常与活动解耦

### 4.1 日常模块原则

- 生命周期长。
- 逻辑稳定。
- 配置变化少。
- 只处理长期存在的玩法。

### 4.2 活动模块原则

- 生命周期短。
- 允许快速新增和删除。
- 每期活动独立目录。
- 不修改 Daily 和 Base。

推荐活动目录：

```text
resource/YYS/activity/<activity_id>/
├── config.json
└── template/
```

推荐活动代码：

```text
src/MaaYYS/Activity/Activities/<ActivityName>Activity.h
src/MaaYYS/Activity/Activities/<ActivityName>Activity.cpp
```

## 5. 文件粒度约束

为避免高耦合低内聚，约束如下：

1. 单个任务文件超过约 300 行时必须拆分。
2. 任务内部超过 3 个明显阶段时，应拆成小类。
3. 活动专属逻辑不得放入公共模块。
4. 公共模块不得知道具体活动名称。
5. 资源 ID 必须由配置驱动，不在多个文件硬编码扩散。

## 6. 推荐拆分类名（已实现）

以御魂为例，已拆分为：

```text
YYSOrochiTask             # 编排入口（主任务） ✅
OrochiNavigator           # 进入御魂和导航 ✅
OrochiLayerSelector       # 层数选择 ✅
OrochiBattleRunner        # 开战和循环 ✅
RewardHandler             # 奖励处理 ✅
```

**注意**：以上组件虽然实现了完整逻辑，但内部使用手写的等待/重试循环，未复用 `YYSWaitFlow`/`YYSRetryFlow` 等 Flow 组件。后续重构时应统一使用 Flow 体系。

## 7. 当前已落地闭环

当前已完成：

- 抽象接口层（`IActionExecutor`、`ITemplateResolver`、`YYSContext`、`IYYSActivity`、`IYYSDaily`、`YYSModule`）。
- 测试桩（`RecordingActionExecutor`、`StaticTemplateResolver`、`ScriptedTemplateResolver`）。
- Flow 流程引擎（`YYSWaitFlow`、`YYSActionFlow`、`YYSRetryFlow`、`YYSLoopFlow`）— 已实现但未被业务代码使用。
- `MaaControllerActionExecutor` 回调适配器（未绑定 MaaCore Controller）。
- `MaaTemplateResolver` 接口和 OpenCV 条件编译实现（`match_template_opencv()` 真实匹配 / 降级到预定义 ROI）。
- `YYSResourceLoader` JSON 资源加载器。
- `YYSOrochiTask` 模块化拆分（4 个子组件）。
- `OrochiDryRun` dry-run 程序（测试通过）。
- `TestResourceLoader` 资源加载测试程序。
- `TemplateMatchVerify` 截图模板匹配验证工具（OpenCV 模式专用）。
- CMake 构建系统（编译通过，3+1 个目标：`MaaYYS` 静态库、`OrochiDryRun`、`TestResourceLoader`、`TemplateMatchVerify`（OpenCV 模式））。

当前未完成：

- `YYSConfig` 类定义（当前为幽灵类）。
- `MaaControllerActionExecutor` 绑定 MaaCore Controller。
- MaaUtils 依赖重新启用和 Logger 接入。
- Flow 体系在业务代码中的实际使用。
- 从 OAS 迁移御魂以外任务的资源。
- 真实设备联调验证。
- 日常任务扩展（探索、悬赏封印等）。
- 活动模块具体实现。
- OCR 执行器实现。
