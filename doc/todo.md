# 待办事项

<!--
文档版本: 2.1
最后更新: 2026-05-09
更新内容: Sprint 1 完成，更新待办状态
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

## Sprint 2：MaaCore 桥接与真实设备联调

- [ ] 桥接 MaaCore Controller 到 `MaaControllerActionExecutor`
- [ ] 重新启用 MaaUtils 依赖（取消 CMakeLists.txt 中的注释）
- [ ] 替换 `printf` 日志为 MaaUtils Logger
- [ ] 真实设备联调御魂流程
- [ ] 验证：御魂任务在真实模拟器上完成至少一次完整流程

## Sprint 3：代码质量与 Flow 系统统一

- [ ] 实现 `YYSConfig` 类定义（消除幽灵类）
- [ ] 统一日志系统（移除所有 `LOG_INFO` 等 `printf` 宏）
- [ ] Flow 系统重构：Orochi 组件改用 `YYSWaitFlow`/`YYSActionFlow`/`YYSRetryFlow`
- [ ] 删除或修复 `Common/Flow/CMakeLists.txt`（当前未被引用且有循环依赖）
- [ ] YYSContext 增强支持配置驱动初始化
- [ ] 验证：重构后 dry-run 和真实设备联调均通过

## Sprint 4：探索任务实现

- [ ] 迁移探索模板资源（补充 `resource/YYS/daily/exploration/template/` PNG 文件）
- [ ] 实现 `YYSExplorationTask` 及子组件（Navigator / ChapterSelector / BattleRunner）
- [ ] 创建 `ExplorationDryRun`
- [ ] 验证：探索任务 dry-run 通过

## Sprint 5：悬赏封印与金币妖怪

- [ ] 迁移悬赏封印模板资源（补充 `resource/YYS/daily/wanted/template/` PNG 文件）
- [ ] 实现悬赏封印任务（含 OCR 识别场景）
- [ ] 实现 OCR 执行器接口
- [ ] 创建 `resource/YYS/daily/coin_monster/` 资源目录和配置
- [ ] 实现金币妖怪任务
- [ ] 创建 `resource/YYS/daily/sign_in/` 资源目录和配置
- [ ] 实现签到任务
- [ ] 验证：三个任务 dry-run 通过

## Sprint 6：活动模块快速迭代体系

- [ ] 设计活动配置 JSON Schema
- [ ] 实现 `ConfigDrivenActivity`
- [ ] 完善 `YYSActivityManager` 的注册和发现机制
- [ ] 实现一个样板活动
- [ ] 验证：新增活动无需修改 Daily/Base，活动可单独启停

## Sprint 7：工程质量提升

- [ ] 引入单元测试框架（GoogleTest 或 Catch2）
- [ ] 为 Flow 组件编写单元测试
- [ ] 为 YYSResourceLoader 编写单元测试
- [ ] 为 YYSOrochiTask 成功/失败路径编写单元测试
- [ ] 完善 `ResourceVerifier` 工具（加入 CMakeLists.txt 构建目标）
- [ ] 将 `YYSResourceProbe` 加入 CMakeLists.txt 构建目标
- [ ] 实现静态扫描脚本
- [ ] 验证：核心组件有测试覆盖

## Sprint 8：P1/P2 日常任务扩展

- [ ] 年兽任务
- [ ] 小动物任务
- [ ] 阴阳寮任务
- [ ] 地域鬼王任务
- [ ] 秘闻副本
- [ ] 真蛇
- [ ] 御灵
- [ ] 觉醒副本

## 已知问题（需在对应 Sprint 中解决）

- [ ] YYSConfig 幽灵类：`YYSContext` 中前向声明但无定义 → Sprint 3 解决
- [ ] Flow 体系未使用：业务代码手写循环 → Sprint 3 解决
- [ ] 日志不统一：`printf` 宏重复定义 → Sprint 2/3 解决
- [ ] Tools 未全部构建：`ResourceVerifier` 和 `YYSResourceProbe` 不在 CMakeLists.txt → Sprint 7 解决
- [ ] Flow 子 CMakeLists.txt 未引用且有循环依赖 → Sprint 3 解决
- [ ] 模板 PNG 缺失：exploration 和 wanted 的 config.json 有定义但无 PNG → Sprint 4/5 解决
- [ ] YYSBaseActivity 未实现 `activity_id()`/`activity_name()` → Sprint 6 解决
- [ ] OpenCV 运行时环境未安装：代码已就绪，需安装 OpenCV 后启用 `MAAYYS_USE_OPENCV` 验证 → Sprint 2 解决

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
- [x] 从 OAS 迁移御魂真实资源
- [x] 拆分 `YYSOrochiTask` 为 4 个子组件
- [x] 实现 Flow 流程引擎（YYSWaitFlow / YYSActionFlow / YYSRetryFlow / YYSLoopFlow）
- [x] 实现 YYSBaseActivity 和 YYSActivityManager 骨架
- [x] 实现 YYSDailyManager 和 register_default_tasks
- [x] 建立 base / daily / activity 三层资源目录结构
- [x] 创建 exploration 和 wanted 的 config.json 骨架
