# MaaAssistantYYS 文档索引

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部 Sprint 完成，文档版本全面同步至 3.1
负责人: AI Assistant
-->

## 文档目标

本目录用于沉淀 MaaAssistantYYS 的完整项目资料，包括框架设计、需求说明、开发计划、技术栈、移植参考和待办事项。所有后续开发应优先查阅本目录，避免需求散落在对话和临时代码里。

## 文档清单

| 文档 | 说明 | 版本 |
|------|------|------|
| [需求说明](requirements.md) | 项目目标、功能边界、模块拆分和验收标准 | 3.1 |
| [框架设计](architecture.md) | 整体架构、分层设计、核心抽象和日常/活动解耦策略 | 3.1 |
| [技术栈](tech-stack.md) | 语言、构建系统、图像识别、控制协议和测试策略 | 3.1 |
| [移植参考](migration-reference.md) | 从 MAA 和 OAS 需要复用、改造、删除或重写的内容 | 3.1 |
| [开发计划](development-plan.md) | 阶段拆解、里程碑、交付物和验证方式 | 3.1 |
| [待办事项](todo.md) | 按优先级维护的开发任务池 | 3.1 |
| [资源规范](resource-spec.md) | 模板、OCR、活动资源、日常资源的目录和配置规范 | 3.1 |
| [验证方案](verification.md) | 构建验证、dry-run、单元测试、设备联调和回归测试策略 | 3.1 |
| [迭代总结](iteration-summary.md) | 迭代周期内完成的任务、指标和下一步计划 | 3.1 |

## 当前项目状态

截至 2026-05-09，全部 8 个 Sprint 和 5 个开发阶段已完成。

### 项目统计

| 指标 | 数值 |
|------|------|
| 源文件 | 55 .h + 56 .cpp + 1 测试 + 1 脚本 = 113 |
| 资源文件 | 57 PNG + 18 JSON |
| 构建目标 | 1 静态库 + 9 可执行文件（含 OpenCV 条件编译的 TemplateMatchVerify） |
| 日常任务 | 13 个（全部 dry-run 通过） |
| 活动样板 | 1 个（ConfigDrivenActivity + tower_sample） |
| 单元测试 | 24 个断言 |
| DryRun 程序 | 3 个（OrochiDryRun / ExplorationDryRun / Sprint5DryRun） |

### 核心能力

| 能力 | 状态 |
|------|------|
| 抽象接口 | ✅ IActionExecutor / ITemplateResolver / IOcrResolver / IYYSDaily / IYYSActivity |
| 设备控制 | ✅ AdbDirectBridge（adb 直连）+ BridgedActionExecutor + DeviceConnector |
| 图像匹配 | ✅ MaaTemplateResolver（OpenCV 4.13.0 条件编译，降级模式可用） |
| OCR | ✅ IOcrResolver 接口 + ScriptedOcrResolver 测试桩（真实执行器待实现） |
| 流程引擎 | ✅ YYSWaitFlow / YYSActionFlow / YYSRetryFlow / YYSLoopFlow（22 个业务文件使用） |
| 任务调度 | ✅ YYSDailyManager 按优先级降序执行 |
| 活动框架 | ✅ ConfigDrivenActivity 配置驱动 + YYSActivityManager 自动发现注册 |
| 配置管理 | ✅ YYSConfig（JSON 加载 / 任务启停 / 优先级） |
| 日志系统 | ✅ YYSLogger（时间戳 + 文件名 + 行号 + 函数名 + 级别过滤） |

### 遗留项

1. MaaUtils `target_link_libraries` 被注释（待 MaaUtils 自身编译配置完成）。
2. 占位 PNG 需替换为真实游戏截图裁剪（除御魂外的 52 个 PNG 为 1x1 占位）。
3. 真实 OCR 执行器待实现。
4. 真实设备联调待模拟器接入。

## 文档维护原则

1. 文档必须与代码同步更新。
2. 新增模块前，先在 [开发计划](development-plan.md) 或 [待办事项](todo.md) 中登记。
3. 活动模块必须独立记录资源、流程和特殊逻辑，禁止污染日常模块。
4. 日常模块应保持稳定，活动更新优先通过 `resource/YYS/activity/<activity_id>` 和活动类扩展解决。
5. 文档中记录的是设计意图和协作边界，具体实现以源码和构建验证为准。
6. 每次文档更新须在文件头部版本控制元数据中记录更新内容、时间和负责人。
7. 文档描述必须与实际代码一致，不得描述尚未实现的功能为已完成。
