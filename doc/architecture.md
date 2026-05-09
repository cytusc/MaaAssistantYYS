# 框架设计

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部 Sprint 完成，反映当前完整架构状态
负责人: AI Assistant
-->

## 1. 总体架构

MaaAssistantYYS 采用"MAA 底座 + YYS 业务层 + 资源配置层"的架构。

```text
MaaAssistantYYS
├── MaaCore / MaaUtils       # 从 MAA 接入的底座能力（已复制，meojson 间接依赖）
├── MaaYYS                   # 阴阳师业务层
│   ├── Base                 # 抽象接口和上下文
│   ├── Common               # 通用控制、识别、桥接、流程、日志、资源
│   ├── Daily                # 日常任务（13 个）
│   ├── Activity             # 活动任务（ConfigDrivenActivity + 样板）
│   └── Tools                # dry-run、验证、测试工具
├── test                     # 单元测试
├── scripts                  # 静态扫描等脚本
└── resource/YYS             # 阴阳师资源配置
```

MaaYYS 业务代码通过 `meojson`（MaaUtils 子模块）间接依赖 MaaUtils，通过 `AdbDirectBridge` 直接控制真实设备，不需要链接 MaaCore 内部类。

## 2. 分层设计

### 2.1 底座层

底座层提供设备控制和图像识别基础能力：

- 设备连接（ADB）
- 截图（`adb exec-out screencap`）
- 点击、滑动、输入（`adb shell input`）
- 模板匹配（OpenCV `cv::matchTemplate`）
- OCR（接口已就绪，真实实现待接入）

### 2.2 桥接层

| 类 | 职责 | 状态 |
|----|------|------|
| `MaaCoreBridge` | 设备控制抽象基类 | ✅ |
| `AdbDirectBridge` | 通过 adb 命令直接控制设备 | ✅ |
| `DeviceConnector` | 一站式创建真实设备上下文 | ✅ |
| `BridgedActionExecutor` | 桥接 MaaCoreBridge → IActionExecutor | ✅ |

### 2.3 适配层

| 抽象接口 | 测试桩 | 真实实现 |
|----------|--------|---------|
| `IActionExecutor` | `RecordingActionExecutor` | `BridgedActionExecutor`（ADB 真实控制）/ `MaaControllerActionExecutor`（回调） |
| `ITemplateResolver` | `StaticTemplateResolver` / `ScriptedTemplateResolver` | `MaaTemplateResolver`（OpenCV 真实匹配 + 降级） |
| `IOcrResolver` | `ScriptedOcrResolver` | 待实现 |

### 2.4 业务层

业务层分为日常和活动两条线：

```text
Daily（13 个任务）
├── YYSDailyTask              # 日常任务基类
├── YYSDailyManager           # 管理器（按优先级排序执行）
└── Tasks
    ├── YYSSignInTask (80)           # 签到
    ├── YYSCoinMonsterTask (70)      # 金币妖怪
    ├── YYSWantedTask (60)           # 悬赏封印（含 OCR）
    │   ├── WantedNavigator
    │   ├── WantedTaskRecognizer
    │   └── WantedRewardHandler
    ├── YYSOrochiTask (50)           # 御魂
    │   ├── OrochiNavigator
    │   ├── OrochiLayerSelector
    │   ├── OrochiBattleRunner
    │   └── RewardHandler
    ├── YYSAreaBossTask (45)         # 地域鬼王
    ├── YYSTrueSnakeTask (45)        # 真蛇
    ├── YYSSoulTask (40)             # 御灵
    ├── YYSAwakeningTask (40)        # 觉醒
    ├── YYSExplorationTask (40)      # 探索
    │   ├── ExplorationNavigator
    │   ├── ChapterSelector
    │   ├── ExplorationRunner
    │   └── ExplorationRewardHandler
    ├── YYSNianBeastTask (35)        # 年兽
    ├── YYSSecretDungeonTask (35)    # 秘闻
    ├── YYSPetsTask (30)             # 小动物
    └── YYSGuildTask (25)            # 阴阳寮

Activity
├── YYSBaseActivity           # 活动基类
├── YYSActivityManager        # 管理器（启停控制 + 自动发现注册）
├── ConfigDrivenActivity      # 配置驱动活动（读 config.json 自动执行步骤）
└── tower_sample              # 样板活动（爬塔）
```

### 2.5 流程层

| Flow 类 | 职责 | 使用状态 |
|---------|------|----------|
| `YYSWaitFlow` | 等待模板出现 | ✅ 全部日常任务使用 |
| `YYSActionFlow` | 执行点击/滑动/长按 | ✅ 全部日常任务使用 |
| `YYSRetryFlow` | 装饰器：重试内部 Flow | ✅ 全部日常任务使用 |
| `YYSLoopFlow` | 循环执行链式 Flow | ✅ 单元测试覆盖 |

## 3. 关键抽象

### 3.1 IActionExecutor

```cpp
virtual bool screencap() = 0;
virtual bool click(Point point) = 0;
virtual bool swipe(Point from, Point to) = 0;
virtual bool long_click(Point point, int duration_ms = 800);
```

### 3.2 ITemplateResolver

```cpp
virtual std::optional<Rect> find_template(const std::string& name) const = 0;
```

### 3.3 IOcrResolver

```cpp
virtual std::optional<std::string> recognize(const std::string& ocr_id) const = 0;
```

### 3.4 YYSContext

```cpp
std::shared_ptr<IActionExecutor> executor() const;
std::shared_ptr<ITemplateResolver> resolver() const;
std::shared_ptr<IOcrResolver> ocr_resolver() const;
std::shared_ptr<YYSConfig> config() const;
```

### 3.5 YYSConfig

完整的配置类，支持 JSON 加载、KV 存储、任务启停和优先级设置。

## 4. 日常与活动解耦

### 4.1 日常模块原则

- 生命周期长，逻辑稳定。
- `YYSDailyManager` 按 `priority()` 降序执行。
- 每个任务继承 `YYSDailyTask`，覆写 `_run()`。

### 4.2 活动模块原则

- 生命周期短，允许快速新增和删除。
- `ConfigDrivenActivity` 读取 config.json 自动执行步骤序列（wait/click/wait_click）。
- `YYSActivityManager::discover_and_register()` 自动扫描活动目录。
- 新增活动不修改 Daily 和 Base。

## 5. 工具链

| 工具 | 说明 |
|------|------|
| `OrochiDryRun` | 御魂任务 dry-run |
| `ExplorationDryRun` | 探索任务 dry-run |
| `Sprint5DryRun` | 悬赏/金币/签到 dry-run |
| `UnitTests` | 23 个单元测试 |
| `DeviceTest` | 设备连接验证 |
| `TemplateMatchVerify` | OpenCV 模板匹配验证 |
| `ResourceVerifier` | 资源完整性检查 |
| `YYSResourceProbe` | 资源探针 |
| `TestResourceLoader` | 资源加载测试 |
| `scripts/static_scan.sh` | 静态扫描（检查伪依赖） |
