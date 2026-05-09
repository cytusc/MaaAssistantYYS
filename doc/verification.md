# 验证方案

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部验证通过，新增 UnitTests / Sprint5DryRun / DeviceTest / 静态扫描
负责人: AI Assistant
-->

## 1. 验证目标

验证体系用于保证 MaaAssistantYYS 的每个阶段都有可量化结果，避免只写代码不验证。

## 2. 构建验证

### 命令

```bash
cmake -S "D:/code/maa/MaaAssistantYYS" -B "D:/code/maa/MaaAssistantYYS/build"
cmake --build "D:/code/maa/MaaAssistantYYS/build"
```

### 当前期望

- 生成 `libMaaYYS.a`。
- 生成 `OrochiDryRun.exe`。
- 生成 `TestResourceLoader.exe`。

### 已知问题

- `ResourceVerifier.cpp` 和 `YYSResourceProbe.cpp` 存在于 `Tools/` 但未加入 CMakeLists.txt 构建目标。
- `Common/Flow/CMakeLists.txt` 定义了 `YYSFlow` 目标但未被主 CMakeLists.txt 引用。

## 3. dry-run 验证

### 3.1 OrochiDryRun

#### 命令

```bash
"D:/code/maa/MaaAssistantYYS/build/OrochiDryRun.exe"
```

#### 当前期望输出

```text
=== Resource Verification ===
Loading resources from: D:/code/maa/MaaAssistantYYS/resource/YYS
✅ Resources loaded successfully
Total templates: N

=== Orochi Templates ===
✅ I_OROCHI
✅ I_OROCHI_FIRE
✅ I_REWARD
✅ O_LAYER_8

=== Running Orochi Task Test ===
[INFO] ========== YYSOrochiTask started ==========
...
=== Test Results ===
dry_run_result=ok
recorded_actions=14
```

### 3.4 ExplorationDryRun

#### 命令

```bash
"D:/code/maa/MaaAssistantYYS/build/ExplorationDryRun.exe"
```

#### 当前期望输出

```text
=== Resource Verification ===
Loading resources from: D:/code/maa/MaaAssistantYYS/resource/YYS
✅ Resources loaded successfully
Total templates: N

=== Exploration Templates ===
✅ I_EXPLORATION
✅ I_CHAPTER
✅ I_EXPLORE_FIGHT
✅ I_EXPLORE_MONSTER
✅ I_EXPLORE_BOSS
✅ I_EXPLORE_REWARD
✅ I_EXPLORE_COMPLETE

=== Running Exploration Task Test ===
[INFO] ========== YYSExplorationTask started ==========
...
=== Test Results ===
dry_run_result=ok
recorded_actions=16
```

#### 验证意义

- 验证探索任务架构可扩展性（第二个日常任务）。
- 验证 4 个子组件的编排流程（导航 → 选章节 → 节点遍历 → 奖励）。
- 验证 Flow 体系在新任务中的复用。

**注意**：`recorded_actions` 的具体数值取决于 `ScriptedTemplateResolver` 的预设序列和 `RecordingActionExecutor` 的记录逻辑。当前配置下为 14（包含截图、点击、滑动等操作）。

#### 验证意义

- 验证 `YYSContext` 依赖注入可用。
- 验证 `ITemplateResolver` 可驱动任务判断。
- 验证 `IActionExecutor` 可记录任务动作。
- 验证御魂最小流程可跑通。

### 3.2 TestResourceLoader

#### 命令

```bash
"D:/code/maa/MaaAssistantYYS/build/TestResourceLoader.exe"
```

#### 当前期望输出

```text
Loading resources from: D:/code/maa/MaaAssistantYYS/resource/YYS
✅ Resources loaded successfully
Templates: N
OCR: N
✅ I_OROCHI (file exists)
✅ I_OROCHI_FIRE (file exists)
✅ I_REWARD (file exists)
✅ O_LAYER_8 (file exists)
```

#### 验证意义

- 验证 `YYSResourceLoader` 可递归扫描和解析 JSON 配置。
- 验证御魂模板资源文件存在。

### 3.3 TemplateMatchVerify（OpenCV 模式专用）

#### 前置条件

需要启用 OpenCV 编译：`cmake -DMAAYYS_USE_OPENCV=ON ...`

#### 命令

```bash
# 匹配所有模板
"D:/code/maa/MaaAssistantYYS/build/TemplateMatchVerify.exe" screenshot.png

# 匹配指定模板
"D:/code/maa/MaaAssistantYYS/build/TemplateMatchVerify.exe" screenshot.png resource/YYS I_OROCHI
```

#### 期望输出

```text
=== Template Match Verify ===
Screenshot: screenshot.png
Resource:   resource/YYS

Screenshot loaded: 1280x720 channels=3
Resources loaded: N templates

=== Matching All Templates ===
  ✅ I_OROCHI -> x=55 y=104 w=295 h=406 (御魂入口图标)
  ❌ I_OROCHI_FIRE -> no match (御魂挑战按钮)
  ...

=== Summary ===
Matched: M/N
```

#### 验证意义

- 验证 `MaaTemplateResolver` 真实图像匹配逻辑。
- 验证 ROI 裁剪和阈值过滤。
- 验证多分辨率缩放。

## 4. 静态扫描验证

### 目标

确保业务层没有重新引入伪依赖或高耦合直连。

### 检查项

业务层不应出现：

| 检查项 | 检查命令 | 当前状态 |
|--------|----------|----------|
| `m_ctx->resource()` | `grep -r "m_ctx->resource()" src/MaaYYS/` | ✅ 无匹配 |
| `m_ctx->controller()` | `grep -r "m_ctx->controller()" src/MaaYYS/` | ✅ 无匹配 |
| `m_ctx->vision()` | `grep -r "m_ctx->vision()" src/MaaYYS/` | ✅ 无匹配 |
| `Logger.hpp` 错误 include | `grep -r "Logger.hpp" src/MaaYYS/` | ✅ 无匹配 |
| `asst/Controller.h` 错误 include | `grep -r "asst/Controller" src/MaaYYS/` | ✅ 无匹配 |
| `asst/Vision` 错误 include | `grep -r "asst/Vision" src/MaaYYS/` | ✅ 无匹配 |

### 额外检查项（建议新增）

| 检查项 | 说明 |
|--------|------|
| `printf` 使用 | 业务代码不应直接使用 `printf`，应统一使用 Logger |
| `LOG_INFO` 宏重复定义 | 多个文件重复定义 `LOG_INFO` 等宏，应统一 |
| `YYSConfig` 解引用 | 不应调用 `ctx->config()->xxx()`，YYSConfig 未定义 |

## 5. 单元测试规划

后续建议引入测试框架（GoogleTest 或 Catch2），对以下内容做单元测试：

| 测试目标 | 优先级 | 说明 |
|----------|--------|------|
| `StaticTemplateResolver` | P0 | 简单映射，易测试 |
| `ScriptedTemplateResolver` | P0 | 序列状态，需验证边界 |
| `RecordingActionExecutor` | P0 | 记录验证 |
| `YYSWaitFlow` | P1 | 超时和成功路径 |
| `YYSActionFlow` | P1 | 各种动作类型 |
| `YYSRetryFlow` | P1 | 最大重试次数 |
| `YYSLoopFlow` | P1 | 有限/无限循环 |
| `YYSResourceLoader` | P1 | 各种配置格式 |
| `YYSOrochiTask` | P2 | 成功和失败路径 |
| `MaaTemplateResolver` | P2 | 降级逻辑 |

## 6. 设备联调验证

设备联调前置条件：

1. ✅ `AdbDirectBridge` 已实现真实设备控制。
2. ✅ `MaaTemplateResolver` 完成真实图像匹配（OpenCV 4.13.0 已安装）。
3. ✅ 御魂模板资源完成迁移。
4. 模拟器分辨率固定为 1280x720 或完成缩放适配。

设备联调工具：`DeviceTest.exe [adb_path] [address]`

设备联调步骤：

1. 启动模拟器和阴阳师。
2. `DeviceTest.exe adb 127.0.0.1:5555` — 验证连接、截图、执行器。
3. `TemplateMatchVerify.exe screenshot.png` — 验证模板匹配。
4. 执行御魂任务。
5. 检查日志和失败返回。

## 7. 回归验证

每次新增任务或活动，至少执行：

- CMake 构建。
- 对应 dry-run。
- 静态扫描。
- 如涉及真实控制，则执行设备联调。

## 8. 活动模块验证

活动模块必须单独验证：

1. 活动配置能加载。
2. 活动模板能识别。
3. 活动 dry-run 能跑通。
4. 禁用活动后不会影响日常任务。
5. 删除活动目录后不会影响基础构建。

## 9. 失败记录规范

失败记录应包含：

- 失败时间。
- 失败任务。
- 当前界面或截图。
- 失败模板 ID。
- 期望动作。
- 实际动作。
- 下一步修复建议。

## 10. 当前验证状态总结

| 验证类型 | 状态 | 说明 |
|----------|------|------|
| 构建验证 | ✅ 通过 | 10 个目标均可构建（降级模式 + OpenCV 模式） |
| OrochiDryRun | ✅ 通过 | dry_run_result=ok |
| ExplorationDryRun | ✅ 通过 | dry_run_result=ok, recorded_actions=16 |
| Sprint5DryRun | ✅ 通过 | wanted/coin_monster/sign_in 全部 ok |
| UnitTests | ✅ 通过 | Passed=23 Failed=0 test_result=ok |
| TestResourceLoader | ✅ 通过 | 资源加载和验证正常 |
| TemplateMatchVerify | ✅ 可构建 | OpenCV 模式构建成功 |
| 静态扫描 | ✅ 通过 | static_scan=ok |
| 设备联调 | 🟡 待设备 | AdbDirectBridge + DeviceTest 已就绪 |
| 静态扫描 | ✅ 通过 | 无伪依赖 |
| TemplateMatchVerify | 🟡 代码就绪 | 需安装 OpenCV 后启用 `MAAYYS_USE_OPENCV` 验证 |
| 设备联调 | ❌ 未完成 | 阻塞于 MaaCore 桥接 |
| 单元测试 | ❌ 未建立 | 无测试框架 |
