# 开发计划

<!--
文档版本: 2.2
最后更新: 2026-05-09
更新内容: Sprint 2 完成 - YYSConfig 实现，YYSLogger 统一日志，MaaCoreBridge 桥接接口
负责人: AI Assistant
-->

## 1. 总体路线

开发路线分为 5 个阶段：

```text
阶段 0：工程底座和文档闭环 ✅
阶段 1：真实控制和识别适配 🔄 (约 70%，OpenCV 条件编译已实现)
阶段 2：御魂任务 MVP 🔄 (约 85%，dry-run 通过，真实联调未完成)
阶段 3：日常任务扩展 ⏳
阶段 4：活动模块快速迭代体系 ⏳
```

## 2. 阶段 0：工程底座和文档闭环 ✅

**状态：已完成**

### 目标

形成可编译、可 dry-run、可继续协作的项目基础。

### 已完成

- `doc` 文档体系。
- `src/MaaYYS` 业务层骨架（Base / Common / Daily / Activity / Tools）。
- `MaaCore` 和 `MaaUtils` 接入（已复制到 src/，include 路径已配置，但未实际桥接）。
- 抽象接口：`IActionExecutor`、`ITemplateResolver`、`IYYSActivity`、`IYYSDaily`、`YYSModule`。
- 测试桩：`RecordingActionExecutor`、`StaticTemplateResolver`、`ScriptedTemplateResolver`。
- Flow 流程引擎：`YYSWaitFlow`、`YYSActionFlow`、`YYSRetryFlow`、`YYSLoopFlow`（已实现，未被业务代码使用）。
- `OrochiDryRun` dry-run 验证。
- `TestResourceLoader` 资源加载验证。

### 验收

- [x] CMake 配置成功。
- [x] `MaaYYS` 静态库构建成功。
- [x] `OrochiDryRun` 输出 `dry_run_result=ok`。
- [x] `TestResourceLoader` 可加载并验证资源。

## 3. 阶段 1：真实控制和识别适配 🔄

### 目标

将抽象接口接到 MAA 真实底座。

### 已完成

- ✅ `MaaControllerActionExecutor`：基于 `std::function` 回调的控制器适配器（**注意：未绑定 MaaCore Controller，当前仅作为回调转发层**）。
- ✅ `YYSResourceLoader` 完整版本（支持模板和 OCR 配置，递归扫描 config.json）。
- ✅ `MaaTemplateResolver` 接口设计和降级策略（自定义匹配 → `match_template_opencv()` → 预定义 ROI）。
- ✅ `YYSOrochiTask` 拆分完成（OrochiNavigator / OrochiLayerSelector / OrochiBattleRunner / RewardHandler）。
- ✅ 御魂资源配置结构完成。
- ✅ OpenCV 条件编译集成（`-DMAAYYS_USE_OPENCV=ON` 启用真实匹配，默认 OFF 降级模式）。
- ✅ `MaaTemplateResolver::match_template_opencv()` 真实图像匹配实现（`cv::matchTemplate` + ROI 裁剪 + 阈值过滤 + 多分辨率缩放）。
- ✅ `MaaTemplateResolver::set_image()` 截图输入实现（支持 `cv::Mat` 和原始像素数据）。
- ✅ `TemplateMatchVerify` 截图模板匹配验证工具。

### 进行中 / 阻塞

- 🔄 OpenCV 运行时环境安装（代码已就绪，需在目标机器安装 OpenCV 库才能启用 `MAAYYS_USE_OPENCV`）。

### 未完成

- ⏳ OpenCV 运行时环境安装和验证。
- ⏳ `MaaControllerActionExecutor` 绑定 MaaCore Controller（当前仅回调转发）。
- ⏳ MaaUtils 依赖重新启用（CMakeLists.txt 中被注释）。
- ⏳ 日志系统统一（当前使用 `printf` 宏，未接入 MaaUtils Logger）。
- ⏳ `YYSConfig` 类定义（当前为幽灵类，仅有前向声明）。

### 交付物

- ✅ `src/MaaYYS/Common/Controller/MaaControllerActionExecutor.*`
- ✅ `src/MaaYYS/Common/Vision/MaaTemplateResolver.*`（OpenCV 条件编译实现，降级模式可用）
- ✅ `src/MaaYYS/Common/Resource/YYSResourceLoader.*`
- ✅ `src/MaaYYS/Tools/TemplateMatchVerify.cpp`（OpenCV 模式专用）

### 验收

- ✅ CMake 配置成功（降级模式和 OpenCV 模式均可配置）。
- ✅ 降级模式构建和 dry-run 通过。
- 🟡 给定截图和模板，能输出匹配区域（代码已实现，需安装 OpenCV 后启用 `MAAYYS_USE_OPENCV` 验证）。
- 🟡 给定坐标，能通过 MaaControllerActionExecutor 执行点击（回调模式可用，但未绑定 MaaCore）。

## 4. 阶段 2：御魂任务 MVP 🔄

### 目标

完成御魂任务在真实设备上的最小可用流程。

### 已完成

- ✅ 从 OAS 迁移御魂模板资源（5 个 PNG + config.json）。
- ✅ 拆分 `YYSOrochiTask`（4 个子组件）。
- ✅ 支持进入御魂、选择层数、点击挑战、等待结算、处理奖励。
- ✅ 支持失败重试和超时返回。
- ✅ dry-run 测试通过（`OrochiDryRun`）。
- ✅ 日志能定位失败阶段（组件化日志输出，使用 `printf` 宏）。

### 未完成

- ⏳ 真实设备完成一次御魂流程（**阻塞于 OpenCV 集成和 MaaCore 桥接**）。

### 交付物

- ✅ `src/MaaYYS/Daily/Tasks/Orochi/OrochiNavigator.*`
- ✅ `src/MaaYYS/Daily/Tasks/Orochi/OrochiLayerSelector.*`
- ✅ `src/MaaYYS/Daily/Tasks/Orochi/OrochiBattleRunner.*`
- ✅ `src/MaaYYS/Daily/Tasks/Orochi/RewardHandler.*`
- ✅ `resource/YYS/daily/orochi/config.json`
- ✅ `resource/YYS/daily/orochi/template/*`

### 验收

- [x] dry-run 通过（OrochiDryRun.exe）
- [ ] 真实设备完成一次御魂流程（待设备联调）
- [x] 日志能定位失败阶段

## 5. 阶段 3：日常任务扩展 ⏳

### 目标

扩展稳定日常任务。

### 优先级

P0：

- 探索（config.json 骨架已存在，模板 PNG 缺失）
- 悬赏封印（config.json 骨架已存在，模板 PNG 缺失）
- 金币妖怪
- 签到

P1：

- 年兽
- 小动物
- 阴阳寮
- 地域鬼王

P2：

- 秘闻
- 真蛇
- 御灵
- 觉醒

## 6. 阶段 4：活动模块快速迭代体系 ⏳

### 目标

形成每期活动只改活动目录和活动类的迭代机制。

### 任务

1. 实现 `ConfigDrivenActivity`。
2. 约定 `resource/YYS/activity/<activity_id>/config.json`。
3. 建立活动注册机制。
4. 迁移一个简单活动作为样板。

### 验收

- 新增活动无需修改 Daily。
- 新增活动无需修改 Base。
- 活动可单独启停。

## 7. Sprint 计划

### Sprint 1：OpenCV 集成与真实模板匹配 ✅ 已完成

**目标**：解除当前最大阻塞项，使项目能进行真实图像匹配。

1. 配置 OpenCV 依赖（CMakeLists.txt 添加 `find_package` + `MAAYYS_USE_OPENCV` 选项）。
2. 实现 `MaaTemplateResolver::match_template_opencv()` 真实匹配逻辑（`cv::matchTemplate` + ROI + 阈值）。
3. 实现 `MaaTemplateResolver::set_image()` 截图数据转 `cv::Mat`（支持多分辨率缩放和通道转换）。
4. 构建截图模板匹配验证工具（`TemplateMatchVerify`）。
5. 验证：降级模式构建和 dry-run 通过。

### Sprint 2：MaaCore 桥接与真实设备联调

**目标**：将抽象接口真正连接到 MaaCore，完成端到端闭环。

1. 桥接 MaaCore Controller 到 `MaaControllerActionExecutor`。
2. 重新启用 MaaUtils 依赖，替换 `printf` 日志为 MaaUtils Logger。
3. 真实设备联调御魂流程。
4. 验证：御魂任务在真实模拟器上完成至少一次完整流程。

### Sprint 3：代码质量与 Flow 系统统一

**目标**：在扩展新任务前，统一代码风格和基础设施。

1. 统一日志系统（移除 `printf` 宏，接入 MaaUtils Logger）。
2. Flow 系统重构（让 Orochi 组件使用 `YYSWaitFlow`/`YYSActionFlow`/`YYSRetryFlow`）。
3. 实现 `YYSConfig` 类（消除幽灵类）。
4. YYSContext 增强支持配置驱动初始化。
5. 验证：重构后 dry-run 和真实设备联调均通过。

### Sprint 4：探索任务实现

**目标**：第一个御魂之外的新日常任务，验证架构可扩展性。

1. 迁移探索模板资源（补充 PNG 文件）。
2. 实现 `YYSExplorationTask` 及子组件。
3. 创建 `ExplorationDryRun`。
4. 验证：探索任务 dry-run 通过。

### Sprint 5：悬赏封印与金币妖怪

**目标**：扩展日常任务覆盖面，引入 OCR 识别场景。

1. 悬赏封印任务（首个 OCR 场景）。
2. 金币妖怪任务。
3. 签到任务。
4. 验证：三个任务 dry-run 通过。

### Sprint 6：活动模块快速迭代体系

**目标**：建立活动快速开发框架。

1. 实现 `ConfigDrivenActivity`。
2. 完善活动注册机制。
3. 实现一个样板活动。
4. 验证：新增活动无需修改 Daily/Base，活动可单独启停。

### Sprint 7：工程质量提升

**目标**：补齐工程基础设施。

1. 引入单元测试框架（GoogleTest 或 Catch2）。
2. 完善资源校验工具。
3. 实现静态扫描脚本。
4. 验证：核心组件有测试覆盖。

### Sprint 8：P1/P2 日常任务扩展

**目标**：覆盖更多日常任务。

1. P1 任务：年兽、小动物、阴阳寮、地域鬼王。
2. P2 任务：秘闻、真蛇、御灵、觉醒。
3. 验证：每个 P1 任务 dry-run 通过。

## 8. 质量门禁

每个 Sprint 必须包含：

1. 构建验证。
2. dry-run 或设备联调验证。
3. 文档更新。
4. 待办状态更新。
5. 失败场景记录。

## 9. 风险与依赖

| 风险项 | 影响 | 缓解措施 |
|--------|------|----------|
| OpenCV 环境配置困难 | 阻塞 Sprint 1 | 优先使用 vcpkg 或 conan 管理依赖 |
| MaaCore API 不稳定 | 阻塞 Sprint 2 | 通过适配层隔离，不直接暴露 MaaCore 类型 |
| OAS 资源坐标不准 | 影响所有任务 | 真实设备联调时校准，保留 ROI 配置可调 |
| OCR 在艺术字上不稳定 | 影响悬赏封印等 | 保留模板匹配兜底，不纯依赖 OCR |
| 活动 UI 频繁变化 | 影响活动模块 | ConfigDrivenActivity 设计为配置驱动 |
| YYSConfig 幽灵类 | 影响配置驱动 | Sprint 3 中优先实现 YYSConfig 定义 |
