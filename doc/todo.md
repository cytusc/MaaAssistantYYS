# 待办事项

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部 Sprint 完成 + MaaCore 桥接 + OpenCV 运行时安装
负责人: AI Assistant
-->

## Sprint 1：OpenCV 集成与真实模板匹配 ✅ 已完成

- [x] 配置 OpenCV 依赖（CMakeLists.txt 添加 `MAAYYS_USE_OPENCV` 选项 + `find_package`）
- [x] 实现 `MaaTemplateResolver::match_template_opencv()` 真实图像匹配
- [x] 实现 `MaaTemplateResolver::set_image()` 截图数据转 `cv::Mat`
- [x] 实现 `MaaTemplateResolver::set_image_from_raw()` 原始像素数据输入
- [x] 实现 `MaaTemplateResolver::clear_image()` 清除截图
- [x] 构建截图模板匹配验证工具（`TemplateMatchVerify`）
- [x] 验证：降级模式构建和 dry-run 通过

## Sprint 2：MaaCore 桥接与代码质量 ✅ 已完成

- [x] 实现 `YYSConfig` 类定义（消除幽灵类，支持 JSON 配置加载、任务启停、优先级设置）
- [x] 创建 `YYSLogger` 统一日志系统（带时间戳、文件名、行号、函数名）
- [x] 替换所有业务代码中的 `printf` 日志为 `YYS_LOG_*` 宏
- [x] 设计 `MaaCoreBridge` 桥接接口（`NullMaaCoreBridge` 空实现）
- [x] 删除有循环依赖的 `Common/Flow/CMakeLists.txt`
- [x] 增强 Flow 组件（`YYSWaitFlow` 返回 `found_rect`、`YYSLoopFlow` null check、日志）
- [x] 重构所有 Orochi 组件使用 Flow 组件
- [x] YYSContext 增强支持配置驱动初始化
- [x] 验证：构建和 dry-run 通过

## Sprint 3：代码质量与 Flow 系统统一 ✅ 已完成

- [x] 删除有循环依赖的 `Common/Flow/CMakeLists.txt`
- [x] 增强 Flow 组件（`YYSWaitFlow` 返回 `found_rect`、`YYSLoopFlow` null check、日志）
- [x] 重构 `OrochiNavigator` 使用 `YYSWaitFlow` + `YYSRetryFlow` + `YYSActionFlow`
- [x] 重构 `OrochiBattleRunner` 使用 `YYSWaitFlow` + `YYSRetryFlow` + `YYSActionFlow`
- [x] 重构 `OrochiLayerSelector` 使用 `YYSWaitFlow` + `YYSActionFlow`
- [x] 重构 `RewardHandler` 使用 `YYSWaitFlow` + `YYSActionFlow`
- [x] `YYSContext` 增强支持配置驱动初始化（`create_with_defaults()` / `create_from_config()`）
- [x] 验证：重构后 dry-run 通过

## Sprint 4：探索任务实现 ✅ 已完成

- [x] 更新探索资源配置（`config.json` 补充 7 个模板定义）
- [x] 创建探索模板占位 PNG（`resource/YYS/daily/exploration/template/` 7 个文件）
- [x] 实现 `ExplorationNavigator`（进入探索界面）
- [x] 实现 `ChapterSelector`（章节选择，支持滑动查找）
- [x] 实现 `ExplorationRunner`（地图节点遍历 + 战斗，BOSS 优先）
- [x] 实现 `ExplorationRewardHandler`（奖励弹窗处理）
- [x] 实现 `YYSExplorationTask` 主入口（编排 4 个子组件）
- [x] 注册到 `YYSDailyManager::register_default_tasks()`
- [x] 更新 CMakeLists.txt（Exploration glob + ExplorationDryRun 目标）
- [x] 创建 `ExplorationDryRun`（资源验证 + 任务模拟）
- [x] 验证：构建通过，ExplorationDryRun `dry_run_result=ok`，OrochiDryRun 回归通过

## Sprint 5：悬赏封印与金币妖怪 ✅ 已完成

- [x] 实现 `IOcrResolver` 接口（OCR 识别抽象）
- [x] 实现 `ScriptedOcrResolver` 测试桩（按序列返回预设文本）
- [x] `YYSContext` 添加 OCR resolver 插槽（`ocr_resolver()` / `set_ocr_resolver()`）
- [x] 迁移悬赏封印模板资源（`resource/YYS/daily/wanted/template/` 5 个 PNG + config.json）
- [x] 实现悬赏封印任务（`YYSWantedTask` + WantedNavigator / WantedTaskRecognizer / WantedRewardHandler）
- [x] WantedTaskRecognizer 使用 OCR 识别悬赏目标
- [x] 创建 `resource/YYS/daily/coin_monster/` 资源目录和配置（3 个 PNG + config.json）
- [x] 实现金币妖怪任务（`YYSCoinMonsterTask`）
- [x] 创建 `resource/YYS/daily/sign_in/` 资源目录和配置（3 个 PNG + config.json）
- [x] 实现签到任务（`YYSSignInTask`）
- [x] 创建 `Sprint5DryRun`（测试 3 个任务 + OCR 验证）
- [x] 验证：三个任务 dry-run 通过，回归通过

## Sprint 6：活动模块快速迭代体系 ✅ 已完成

- [x] 实现 `ConfigDrivenActivity`（读取 config.json 步骤列表，自动执行 wait/click/wait_click）
- [x] 实现 `load_activity_from_config()` 工厂函数
- [x] 增强 `YYSActivityManager`（启停控制 + `discover_and_register()` 自动扫描注册）
- [x] 实现样板活动（`tower_sample` 爬塔活动，3 个步骤 + 3 个模板）
- [x] 验证：新增活动无需修改 Daily/Base，活动可单独启停

## Sprint 7：工程质量提升 ✅ 已完成

- [x] 实现内置测试框架（无外部依赖，`yys_test` 命名空间）
- [x] 为 StaticTemplateResolver 编写单元测试（3 个 case）
- [x] 为 ScriptedTemplateResolver 编写单元测试（5 个 case）
- [x] 为 ScriptedOcrResolver 编写单元测试（4 个 case）
- [x] 为 RecordingActionExecutor 编写单元测试（1 个 case）
- [x] 为 YYSWaitFlow 编写单元测试（成功 + 超时路径）
- [x] 为 YYSRetryFlow 编写单元测试（成功路径）
- [x] 为 YYSLoopFlow 编写单元测试（有限循环）
- [x] 为 YYSResourceLoader 编写单元测试（加载 + 模板 + OCR 验证）
- [x] 完善 `ResourceVerifier` 工具（加入 CMakeLists.txt 构建目标）
- [x] 将 `YYSResourceProbe` 加入 CMakeLists.txt 构建目标
- [x] 实现静态扫描脚本（`scripts/static_scan.sh`）
- [x] 验证：23 个测试全部通过，静态扫描通过

## Sprint 8：P1/P2 日常任务扩展 ✅ 已完成

- [x] 年兽任务（`YYSNianBeastTask`，priority 35）
- [x] 小动物任务（`YYSPetsTask`，priority 30）
- [x] 阴阳寮任务（`YYSGuildTask`，priority 25）
- [x] 地域鬼王任务（`YYSAreaBossTask`，priority 45）
- [x] 秘闻副本（`YYSSecretDungeonTask`，priority 35）
- [x] 真蛇（`YYSTrueSnakeTask`，priority 45）
- [x] 御灵（`YYSSoulTask`，priority 40）
- [x] 觉醒副本（`YYSAwakeningTask`，priority 40）
- [x] 全部注册到 `YYSDailyManager`，按优先级排序执行
- [x] 验证：编译通过，回归测试通过

## 已知问题（需在对应 Sprint 中解决）

- [x] ~~YYSConfig 幽灵类~~ → Sprint 2 已解决
- [x] ~~Flow 体系未使用~~ → Sprint 3 已解决
- [x] ~~日志不统一~~ → Sprint 2 已解决（YYSLogger）
- [x] ~~Tools 未全部构建~~ → Sprint 7 已解决，ResourceVerifier 和 YYSResourceProbe 已加入 CMakeLists.txt
- [x] ~~Flow 子 CMakeLists.txt 未引用且有循环依赖~~ → Sprint 3 已删除
- [x] ~~模板 PNG 缺失~~ → Sprint 4/5 已解决，所有任务均有占位 PNG
- [x] ~~YYSBaseActivity 未实现纯虚函数~~ → Sprint 6 已解决（ConfigDrivenActivity 实现了 activity_id/activity_name）
- [x] ~~OpenCV 运行时环境未安装~~ → OpenCV 4.13.0 已安装，`MAAYYS_USE_OPENCV=ON` 构建通过
- [x] ~~MaaCore 真实桥接未实现~~ → AdbDirectBridge 已实现，通过 adb 命令直接控制设备
- [ ] MaaUtils 依赖未重新启用 → 待 MaaCore 编译环境搭建后启用

## 已完成（历史记录）

- [x] 创建 `doc` 文档目录
- [x] 建立项目需求、架构、技术栈、移植参考和开发计划文档
- [x] 接入 `MaaCore` 和 `MaaUtils`（已复制到 src/，include 路径已配置）
- [x] 建立 `IActionExecutor`、`ITemplateResolver`、`YYSContext`
- [x] 建立 `OrochiDryRun` 验证程序
- [x] 实现 `MaaControllerActionExecutor`（回调适配器）
- [x] 实现 `YYSResourceLoader` 完整版本
- [x] 实现 `MaaTemplateResolver` 接口和降级策略
- [x] OpenCV 条件编译集成（`MAAYYS_USE_OPENCV` 选项）
- [x] 实现 `MaaTemplateResolver::match_template_opencv()` 真实图像匹配
- [x] 实现 `MaaTemplateResolver::set_image()` / `set_image_from_raw()` / `clear_image()`
- [x] 创建 `TemplateMatchVerify` 验证工具
- [x] 实现 `YYSConfig` 类定义（消除幽灵类）
- [x] 创建 `YYSLogger` 统一日志系统
- [x] 替换所有业务代码中的 `printf` 日志为 `YYS_LOG_*` 宏
- [x] 设计 `MaaCoreBridge` 桥接接口（`NullMaaCoreBridge` 空实现）
- [x] 从 OAS 迁移御魂真实资源
- [x] 拆分 `YYSOrochiTask` 为 4 个子组件
- [x] 实现 Flow 流程引擎（YYSWaitFlow / YYSActionFlow / YYSRetryFlow / YYSLoopFlow）
- [x] 实现 YYSBaseActivity 和 YYSActivityManager 骨架
- [x] 实现 YYSDailyManager 和 register_default_tasks
- [x] 建立 base / daily / activity 三层资源目录结构
- [x] 创建 exploration 和 wanted 的 config.json 骨架
