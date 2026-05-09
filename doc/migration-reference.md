# 移植参考

<!--
文档版本: 2.1
最后更新: 2026-05-09
更新内容: Sprint 1 完成 - 更新 MaaTemplateResolver 移植状态
负责人: AI Assistant
-->

## 1. 移植目标

MaaAssistantYYS 需要同时参考两个项目：

1. MAA：提供跨平台自动化底座，包括设备控制、截图、图像识别、OCR、任务框架和构建体系。
2. OAS：提供阴阳师业务经验，包括任务流程、资源定义、模板坐标、OCR 区域和异常处理经验。

移植原则不是机械复制，而是拆分为可复用、需改造、需删除、需重写四类。

## 2. 从 MAA 参考和移植的内容

### 2.1 可直接参考

- Controller 抽象
- ADB、minitouch、maatouch 控制方式
- 截图和缓存机制
- Vision Helper
- Matcher / MultiMatcher / OCRer
- CMake 组织方式
- 资源加载思想
- 日志和错误回调机制

**当前状态**：以上内容代码已存在于 `src/MaaCore/`，但 MaaYYS **未实际引用**任何 MaaCore 头文件。MaaYYS 自建了 `IActionExecutor`/`ITemplateResolver` 抽象层，通过 `MaaControllerActionExecutor`（回调适配器）和 `MaaTemplateResolver`（降级到预定义 ROI）间接对接。后续需要真正桥接 MaaCore 的 Controller 和 Vision。

### 2.2 需要适配

- 任务体系：MAA 原任务以明日方舟为中心，需要替换为 YYS Daily / Activity。
- 资源目录：MAA 原资源以 Arknights 为中心，需要改为 `resource/YYS`。
- 客户端配置：包名、启动方式、分辨率默认值需要适配阴阳师。
- OCR 词库：需要加入阴阳师常见文本。

**当前状态**：资源目录已改为 `resource/YYS`（base/daily/activity 三层），客户端配置已在 `resource/YYS/base/config.json` 中定义（包名 `com.netease.onmyoji`）。任务体系已自建（`YYSDailyTask`/`YYSBaseActivity`），未复用 MAA Task 框架。

### 2.3 应删除或隔离

- 明日方舟关卡导航逻辑。
- 明日方舟基建、公开招募、肉鸽、保全等业务任务。
- 明日方舟专属资源和任务配置。
- 与明日方舟掉落上传相关的逻辑。

隔离策略：先保留 MaaCore 底座代码，业务层不依赖明日方舟任务；后续再逐步裁剪。

**当前状态**：MaaCore 代码完整保留在 `src/MaaCore/`，MaaYYS 业务层未引用任何 MaaCore 业务代码，隔离已实现。

## 3. 从 OAS 参考和移植的内容

### 3.1 可参考的核心能力

OAS 中值得迁移的不是 Python 代码本身，而是以下业务知识：

- 阴阳师界面流转路径。
- 各任务的入口、按钮、结算界面。
- 模板 ROI 坐标。
- OCR 区域和识别关键词。
- 点击、等待、重试、异常弹窗处理。
- 活动任务快速迭代方式。

### 3.2 OAS Rule 映射

| OAS 概念 | MaaAssistantYYS 对应设计 | 实现状态 |
| --- | --- | --- |
| `RuleImage` | `ITemplateResolver` + MAA Matcher | 🟡 接口完成，真实匹配待 OpenCV |
| `RuleClick` | `IActionExecutor::click` | ✅ 接口完成 |
| `RuleLongClick` | `IActionExecutor::long_click` | ✅ 接口完成 |
| `RuleSwipe` | `IActionExecutor::swipe` | ✅ 接口完成 |
| `appear` | `YYSWaitFlow` 或任务内 `wait_with_timeout` | 🟡 Flow 已实现但未使用 |
| `appear_then_click` | `YYSWaitFlow` + `YYSActionFlow` | 🟡 Flow 已实现但未使用 |
| `ui_click` | `YYSLoopFlow` + `YYSActionFlow` | 🟡 Flow 已实现但未使用 |
| `interval_timer` | 后续统一进入 Flow 或 Scheduler | ❌ 未实现 |

### 3.3 优先参考的 OAS 模块

优先级 P0：

- `tasks/Orochi` — ✅ 已迁移资源和流程
- `tasks/Exploration` — 🟡 config.json 骨架已创建，资源和代码待迁移
- `tasks/WantedQuests` — 🟡 config.json 骨架已创建，资源和代码待迁移
- `tasks/GlobalGame` — ❌ 未迁移
- `tasks/Component` — ❌ 未迁移
- `module/atom/image.py` — ✅ 对应 `ITemplateResolver`
- `module/atom/click.py` — ✅ 对应 `IActionExecutor`
- `module/device/control.py` — 🟡 对应 `MaaControllerActionExecutor`（回调适配器）

优先级 P1：

- 活动任务目录 — ❌ 未迁移
- 阴阳寮任务 — ❌ 未迁移
- 秘闻、真蛇、御灵、觉醒等副本 — ❌ 未迁移
- OCR 和列表识别相关模块 — ❌ 未迁移

## 4. 移植顺序建议

### 阶段一：资源和动作抽象 ✅（大部分完成）

- ✅ 完成 `IActionExecutor` 到回调适配器的实现。
- 🔄 完成 `ITemplateResolver` 到 MAA Matcher 的真实适配（OpenCV 条件编译已实现，`match_template_opencv()` 真实匹配可用）。
- ✅ 完成 JSON 资源加载（`YYSResourceLoader`）。

### 阶段二：御魂任务 ✅（dry-run 完成，设备联调未完成）

- ✅ 迁移 OAS 御魂资源。
- ✅ 拆分御魂任务为导航、选层、战斗、领奖。
- ✅ 完成 dry-run。
- ⏳ 完成设备联调（阻塞于 OpenCV 和 MaaCore 桥接）。

### 阶段三：日常任务扩展 ⏳

- 探索
- 悬赏
- 金币妖怪
- 签到
- 小动物

### 阶段四：活动模块 ⏳

- 建立活动模板。
- 迁移一个简单活动作为样板。
- 建立活动配置快速迭代规范。

## 5. 移植风险

1. OAS 坐标和模板基于 1280x720，其他分辨率需适配。
2. OAS Python 动态能力强，迁移到 C++ 时需要显式类型和接口。
3. 活动 UI 更新频繁，不能写入核心模块。
4. OCR 在阴阳师艺术字上可能不稳定，需要保留模板兜底。
5. 直接复制 OAS 任务流程可能导致 C++ 文件过大，应拆成小类。
6. MaaCore 的 Controller/Vision API 可能与当前适配层设计不匹配，桥接时可能需要调整适配层接口。

## 6. 当前移植状态

已完成：

- ✅ 御魂任务最小流程骨架（4 个子组件）。
- ✅ 抽象执行器和模板解析器接口。
- ✅ dry-run 验证链路。
- ✅ JSON 资源自动加载（`YYSResourceLoader` 递归扫描 config.json）。
- ✅ 御魂模板资源迁移（5 个 PNG + config.json）。
- ✅ 基础通用按钮资源（5 个 PNG）。
- ✅ 活动样板资源（2 个 PNG）。
- ✅ 探索和悬赏封印的 config.json 骨架。

未完成：

- ⏳ OAS 御魂以外任务的资源迁移。
- ✅ MaaCore Vision 真实匹配代码（`match_template_opencv()` 已实现，需 `-DMAAYYS_USE_OPENCV=ON` 启用）。
- ⏳ MaaCore Controller 真实执行（`MaaControllerActionExecutor` 未绑定 MaaCore）。
- ⏳ MaaUtils 依赖重新启用。
- ⏳ 设备联调。
- ⏳ OCR 执行器实现。
