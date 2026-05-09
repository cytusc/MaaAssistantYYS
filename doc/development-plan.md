# 开发计划

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部 8 个 Sprint 完成，AdbDirectBridge 实现，OpenCV 4.13.0 安装
负责人: AI Assistant
-->

## 1. 总体路线

开发路线分为 5 个阶段，全部已完成：

```text
阶段 0：工程底座和文档闭环 ✅
阶段 1：真实控制和识别适配 ✅ (AdbDirectBridge + OpenCV 4.13.0)
阶段 2：御魂任务 MVP ✅ (dry-run 通过，真实联调待设备)
阶段 3：日常任务扩展 ✅ (13 个日常任务全部实现)
阶段 4：活动模块快速迭代体系 ✅ (ConfigDrivenActivity + 样板活动)
```

## 2. 阶段 0：工程底座和文档闭环 ✅

**状态：已完成**

- `doc` 文档体系（9 个文档）。
- `src/MaaYYS` 业务层骨架（Base / Common / Daily / Activity / Tools）。
- `MaaCore` 和 `MaaUtils` 接入（已复制到 src/，include 路径已配置）。
- 抽象接口：`IActionExecutor`、`ITemplateResolver`、`IOcrResolver`、`IYYSActivity`、`IYYSDaily`、`YYSModule`。
- 测试桩：`RecordingActionExecutor`、`StaticTemplateResolver`、`ScriptedTemplateResolver`、`ScriptedOcrResolver`。
- Flow 流程引擎：`YYSWaitFlow`、`YYSActionFlow`、`YYSRetryFlow`、`YYSLoopFlow`。
- `YYSConfig` 配置类（JSON 加载、任务启停、优先级设置）。
- `YYSLogger` 统一日志系统。

## 3. 阶段 1：真实控制和识别适配 ✅

**状态：已完成**

- ✅ `MaaControllerActionExecutor`：回调适配器。
- ✅ `BridgedActionExecutor`：将 MaaCoreBridge 桥接到 IActionExecutor。
- ✅ `AdbDirectBridge`：通过 adb 命令直接控制真实设备（connect / screencap / click / swipe / long_click / input / start_game / stop_game）。
- ✅ `DeviceConnector`：一站式初始化真实设备上下文。
- ✅ `DeviceTest`：设备连接验证工具。
- ✅ `MaaTemplateResolver`：OpenCV 条件编译实现（`match_template_opencv()` 真实匹配 + 降级模式）。
- ✅ `TemplateMatchVerify`：截图模板匹配验证工具。
- ✅ OpenCV 4.13.0 运行时已安装，`MAAYYS_USE_OPENCV=ON` 构建通过。
- ✅ `YYSResourceLoader` 完整版本。

### 交付物

- `src/MaaYYS/Common/Bridge/AdbDirectBridge.*`
- `src/MaaYYS/Common/Bridge/DeviceConnector.*`
- `src/MaaYYS/Common/Controller/BridgedActionExecutor.*`
- `src/MaaYYS/Common/Controller/MaaControllerActionExecutor.*`
- `src/MaaYYS/Common/Vision/MaaTemplateResolver.*`
- `src/MaaYYS/Tools/DeviceTest.cpp`
- `src/MaaYYS/Tools/TemplateMatchVerify.cpp`

## 4. 阶段 2：御魂任务 MVP ✅

**状态：已完成（dry-run 通过，真实联调待设备接入）**

- ✅ 从 OAS 迁移御魂模板资源（5 个 PNG + config.json）。
- ✅ 拆分 `YYSOrochiTask`（OrochiNavigator / OrochiLayerSelector / OrochiBattleRunner / RewardHandler）。
- ✅ dry-run 测试通过（`OrochiDryRun`）。

### 验收

- [x] dry-run 通过（OrochiDryRun.exe, `dry_run_result=ok`）
- [ ] 真实设备完成一次御魂流程（待模拟器设备接入）
- [x] 日志能定位失败阶段

## 5. 阶段 3：日常任务扩展 ✅

**状态：已完成，13 个日常任务全部实现**

| 任务 | 类名 | 优先级 | 子组件 | Dry-Run |
|------|------|--------|--------|---------|
| 签到 | YYSSignInTask | 80 | 单文件 | ✅ |
| 金币妖怪 | YYSCoinMonsterTask | 70 | 单文件 | ✅ |
| 悬赏封印 | YYSWantedTask | 60 | 3 子组件 + OCR | ✅ |
| 御魂副本 | YYSOrochiTask | 50 | 4 子组件 | ✅ |
| 地域鬼王 | YYSAreaBossTask | 45 | 单文件 | ✅ |
| 真蛇 | YYSTrueSnakeTask | 45 | 单文件 | ✅ |
| 御灵 | YYSSoulTask | 40 | 单文件 | ✅ |
| 觉醒副本 | YYSAwakeningTask | 40 | 单文件 | ✅ |
| 探索副本 | YYSExplorationTask | 40 | 4 子组件 | ✅ |
| 年兽 | YYSNianBeastTask | 35 | 单文件 | ✅ |
| 秘闻副本 | YYSSecretDungeonTask | 35 | 单文件 | ✅ |
| 小动物 | YYSPetsTask | 30 | 单文件 | ✅ |
| 阴阳寮 | YYSGuildTask | 25 | 单文件 | ✅ |

全部注册到 `YYSDailyManager`，`run_all()` 按优先级降序执行。

## 6. 阶段 4：活动模块快速迭代体系 ✅

**状态：已完成**

- ✅ `ConfigDrivenActivity`：读取 config.json 步骤列表，自动执行 wait/click/wait_click。
- ✅ `load_activity_from_config()` 工厂函数。
- ✅ `YYSActivityManager` 增强：启停控制 + `discover_and_register()` 自动扫描活动目录。
- ✅ 样板活动 `tower_sample`（爬塔，3 个步骤 + 3 个模板）。

### 验收

- [x] 新增活动无需修改 Daily。
- [x] 新增活动无需修改 Base。
- [x] 活动可单独启停。

## 7. Sprint 计划

### Sprint 1：OpenCV 集成与真实模板匹配 ✅ 已完成

OpenCV 条件编译 + `match_template_opencv()` + `TemplateMatchVerify`。

### Sprint 2：MaaCore 桥接与代码质量 ✅ 已完成

`YYSConfig` + `YYSLogger` + `MaaCoreBridge` 接口设计。

### Sprint 3：代码质量与 Flow 系统统一 ✅ 已完成

Flow 体系重构 + 全部 Orochi 组件迁移到 Flow。

### Sprint 4：探索任务实现 ✅ 已完成

`YYSExplorationTask` + 4 个子组件 + `ExplorationDryRun`。

### Sprint 5：悬赏封印与金币妖怪 ✅ 已完成

`IOcrResolver` + 悬赏封印（OCR）+ 金币妖怪 + 签到 + `Sprint5DryRun`。

### Sprint 6：活动模块快速迭代体系 ✅ 已完成

`ConfigDrivenActivity` + 活动自动发现注册 + 样板活动。

### Sprint 7：工程质量提升 ✅ 已完成

内置测试框架（23 个测试）+ ResourceVerifier/Probe 构建 + 静态扫描脚本。

### Sprint 8：P1/P2 日常任务扩展 ✅ 已完成

8 个新任务（年兽/小动物/阴阳寮/地域鬼王/秘闻/真蛇/御灵/觉醒）。

## 8. 质量门禁

每个 Sprint 均已包含：

1. ✅ 构建验证（降级模式 + OpenCV 模式）。
2. ✅ dry-run 或单元测试验证。
3. ✅ 文档更新。
4. ✅ 待办状态更新。
5. ✅ 静态扫描通过。

## 9. 风险与依赖

| 风险项 | 状态 | 缓解措施 |
|--------|------|----------|
| OpenCV 环境配置 | ✅ 已解决 | OpenCV 4.13.0 从 MSYS2 镜像安装 |
| MaaCore API 耦合 | ✅ 已解决 | AdbDirectBridge 绕过 MaaCore 内部类，直接 adb 命令 |
| OAS 资源坐标不准 | 🟡 待校准 | 真实设备联调时校准，ROI 配置可调 |
| OCR 在艺术字上不稳定 | 🟡 待验证 | IOcrResolver 接口已就绪，真实 OCR 待实现 |
| 活动 UI 频繁变化 | ✅ 已解决 | ConfigDrivenActivity 配置驱动 |

## 10. 当前项目统计

| 指标 | 数值 |
|------|------|
| 源文件 (.h/.cpp) | 86+ |
| 资源文件 (PNG) | 50+ |
| 资源配置 (JSON) | 15+ |
| 编译目标 | 10 (1 静态库 + 9 可执行文件) |
| 日常任务 | 13 个 |
| 活动样板 | 1 个 |
| 单元测试 | 23 个 |
| DryRun 程序 | 4 个 |
