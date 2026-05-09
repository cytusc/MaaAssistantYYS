# MaaAssistantYYS 文档索引

<!--
文档版本: 2.3
最后更新: 2026-05-09
更新内容: Sprint 3 完成 - Flow 体系重构，Orochi 组件使用 Flow，YYSContext 配置驱动初始化
负责人: AI Assistant
-->

## 文档目标

本目录用于沉淀 MaaAssistantYYS 的完整项目资料，包括框架设计、需求说明、开发计划、技术栈、移植参考和待办事项。所有后续开发应优先查阅本目录，避免需求散落在对话和临时代码里。

## 文档清单

| 文档 | 说明 | 版本 | 最后更新 |
|------|------|------|----------|
| [需求说明](requirements.md) | 项目目标、功能边界、模块拆分和验收标准 | 2.0 | 2026-05-09 |
| [框架设计](architecture.md) | 整体架构、分层设计、核心抽象和日常/活动解耦策略 | 2.0 | 2026-05-09 |
| [技术栈](tech-stack.md) | 语言、构建系统、图像识别、控制协议和测试策略 | 2.0 | 2026-05-09 |
| [移植参考](migration-reference.md) | 从 MAA 和 OAS 需要复用、改造、删除或重写的内容 | 2.0 | 2026-05-09 |
| [开发计划](development-plan.md) | 阶段拆解、里程碑、交付物和验证方式 | 2.0 | 2026-05-09 |
| [待办事项](todo.md) | 按优先级维护的开发任务池 | 2.0 | 2026-05-09 |
| [资源规范](resource-spec.md) | 模板、OCR、活动资源、日常资源的目录和配置规范 | 2.0 | 2026-05-09 |
| [验证方案](verification.md) | 构建验证、dry-run、单元测试、设备联调和回归测试策略 | 2.0 | 2026-05-09 |
| [迭代总结](iteration-summary.md) | 迭代周期内完成的任务、指标和下一步计划 | 2.0 | 2026-05-09 |

## 当前项目状态

截至 2026-05-09，项目已完成以下基础闭环：

### 已完成

1. `src/MaaYYS` 业务层骨架（Base / Common / Daily / Activity / Tools 五个子模块）。
2. `src/MaaCore` 与 `src/MaaUtils` 已复制到项目中，include 路径已配置，但 **MaaYYS 代码未实际依赖 MaaCore/MaaUtils**（MaaUtils 链接被注释，MaaCore 无任何 #include 引用）。
3. 抽象接口层：`IActionExecutor`、`ITemplateResolver`、`YYSContext`、`IYYSActivity`、`IYYSDaily`、`YYSModule`。
4. 测试桩：`RecordingActionExecutor`、`StaticTemplateResolver`、`ScriptedTemplateResolver`。
5. Flow 流程引擎：`YYSWaitFlow`、`YYSActionFlow`、`YYSRetryFlow`、`YYSLoopFlow`（**已实现但未被业务代码使用**）。
6. `MaaControllerActionExecutor`：基于 `std::function` 回调的控制器适配器（**未绑定 MaaCore Controller**）。
7. `MaaTemplateResolver`：接口和 OpenCV 条件编译实现（`match_template_opencv()` 真实匹配 / 降级到预定义 ROI，通过 `-DMAAYYS_USE_OPENCV=ON` 启用）。
8. `YYSResourceLoader`：JSON 资源加载器，支持模板和 OCR 配置。
9. `YYSOrochiTask` 模块化拆分：`OrochiNavigator` / `OrochiLayerSelector` / `OrochiBattleRunner` / `RewardHandler`。
10. 御魂资源配置（5 个 PNG + config.json）+ 基础通用按钮（5 个 PNG）+ 活动样板（2 个 PNG）。
11. `OrochiDryRun` 和 `TestResourceLoader` 可构建、可执行。
12. `TemplateMatchVerify` 截图模板匹配验证工具（OpenCV 模式专用）。

### 已知问题

1. ~~YYSConfig 幽灵类~~ → ✅ Sprint 2 已解决。
2. ~~Flow 体系未使用~~ → ✅ Sprint 3 已解决。Orochi 组件已重构使用 `YYSWaitFlow`/`YYSActionFlow`/`YYSRetryFlow`。
3. ~~日志系统不统一~~ → ✅ Sprint 2 已解决。
4. Tools 未全部构建：`ResourceVerifier.cpp` 和 `YYSResourceProbe.cpp` 不在 CMakeLists.txt → Sprint 7 解决。
5. ~~Flow 子 CMakeLists.txt 未引用且有循环依赖~~ → ✅ Sprint 3 已删除。
6. 模板 PNG 缺失：exploration 和 wanted 的 config.json 有定义但无 PNG → Sprint 4/5 解决。
7. MaaCore 桥接接口已设计（`MaaCoreBridge` + `NullMaaCoreBridge`），真实实现待 MaaCore 编译集成。
8. OpenCV 运行时环境未安装：代码已就绪，需安装 OpenCV 后启用 `MAAYYS_USE_OPENCV` 验证。

## 文档维护原则

1. 文档必须与代码同步更新。
2. 新增模块前，先在 [开发计划](development-plan.md) 或 [待办事项](todo.md) 中登记。
3. 活动模块必须独立记录资源、流程和特殊逻辑，禁止污染日常模块。
4. 日常模块应保持稳定，活动更新优先通过 `resource/YYS/activity/<activity_id>` 和活动类扩展解决。
5. 文档中记录的是设计意图和协作边界，具体实现以源码和构建验证为准。
6. 每次文档更新须在文件头部版本控制元数据中记录更新内容、时间和负责人。
7. 文档描述必须与实际代码一致，不得描述尚未实现的功能为已完成。
