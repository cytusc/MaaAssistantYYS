# 使用技术栈

<!--
文档版本: 2.1
最后更新: 2026-05-09
更新内容: Sprint 1 完成 - OpenCV 条件编译集成，MaaTemplateResolver 真实匹配实现
负责人: AI Assistant
-->

## 1. 编程语言

- C++20：主项目语言，承接 MAA 的核心框架风格。
- Python：仅作为辅助脚本候选，用于资源转换、模板提取、批量校验等开发工具。

## 2. 构建系统

- CMake 3.20+
- Ninja / Make / Visual Studio Generator 均可作为生成后端

当前构建目标：

| 目标名 | 类型 | 状态 |
|--------|------|------|
| `MaaYYS` | 静态库 | ✅ 可构建 |
| `OrochiDryRun` | 可执行文件 | ✅ 可构建 |
| `TestResourceLoader` | 可执行文件 | ✅ 可构建 |

**未纳入构建的 Tools**：`ResourceVerifier.cpp`、`YYSResourceProbe.cpp`（存在于源码但未加入 CMakeLists.txt）。

**构建问题**：`Common/Flow/CMakeLists.txt` 定义了 `YYSFlow` 目标但未被主 CMakeLists.txt 引用，且存在循环依赖隐患。

## 3. 核心底座

来自 MAA 的能力：

- `MaaCore`：Controller、Vision、Task、Config、Utils 等模块
- `MaaUtils`：Buffer、IOStream、Logger、Platform 等基础能力

**当前实际状态**：

| 组件 | 代码存在 | Include 路径 | 链接状态 | MaaYYS 实际引用 |
|------|----------|-------------|----------|----------------|
| MaaCore | ✅ `src/MaaCore/` | ✅ 已配置 | N/A | ❌ 无任何 #include |
| MaaUtils | ✅ `src/MaaUtils/` | ✅ 已配置 | ❌ 被注释掉 | ❌ 无任何 #include |

MaaYYS 当前是完全独立的代码，仅通过 `meojson`（MaaUtils 子模块，通过 include 路径间接引用）有间接依赖。后续 Sprint 2 需要重新启用 MaaUtils 链接并桥接 MaaCore。

## 4. 图像识别

### 规划使用

- OpenCV 模板匹配（**待集成**）
- MAA Matcher / MultiMatcher / OCRer（**待桥接**）
- 后续按需使用 ONNX Runtime 或 PaddleOCR 相关能力

### 当前实现

| 组件 | 类型 | 状态 | 说明 |
|------|------|------|------|
| `ITemplateResolver` | 接口 | ✅ | `find_template(name) -> optional<Rect>` |
| `StaticTemplateResolver` | 测试桩 | ✅ | 静态映射，直接查表 |
| `ScriptedTemplateResolver` | 测试桩 | ✅ | 按序返回预设值，模拟状态变化 |
| `MaaTemplateResolver` | 适配器 | ✅ | 三级降级：自定义匹配 → `match_template_opencv()`（OpenCV 条件编译）→ 预定义 ROI |
| `MaaTemplateResolver::match_template_opencv()` | 真实匹配 | ✅ | `cv::matchTemplate` + ROI + 阈值（需 `-DMAAYYS_USE_OPENCV=ON`） |
| `MaaTemplateResolver::set_image()` | 截图输入 | ✅ | 支持 `cv::Mat` 和原始像素数据，多分辨率缩放 |

### OCR

| 组件 | 类型 | 状态 | 说明 |
|------|------|------|------|
| `OcrResource` | 数据结构 | ✅ | ROI、关键词、模式 |
| OCR 执行器 | 接口+实现 | ❌ | 未实现 |

## 5. 设备控制

### 规划支持

- ADB
- minitouch
- maatouch
- Windows 窗口控制

### 当前实现

| 组件 | 类型 | 状态 | 说明 |
|------|------|------|------|
| `IActionExecutor` | 接口 | ✅ | screencap/click/swipe/long_click |
| `RecordingActionExecutor` | 测试桩 | ✅ | 记录动作到向量 |
| `MaaControllerActionExecutor` | 适配器 | 🟡 | 基于 `std::function` 回调，**未绑定 MaaCore Controller** |

## 6. 第三方依赖

| 依赖 | 版本 | 用途 | 集成方式 | 状态 |
|------|------|------|----------|------|
| meojson | - | JSON 解析 | MaaUtils 子模块，include 路径引用 | ✅ 已集成 |
| OpenCV | 4.x（待定） | 图像匹配 | `find_package`（条件编译 `MAAYYS_USE_OPENCV`） | ✅ 代码已集成，运行时待安装 |
| MaaUtils | - | Logger/Platform/IO | `target_link_libraries`（被注释） | ❌ 未链接 |
| GoogleTest/Catch2 | - | 单元测试 | `find_package`（待配置） | ❌ 未集成 |

## 7. 资源格式

资源目录：

```text
resource/YYS/
├── base/          # 通用按钮（5 个 PNG + config.json）
├── daily/         # 日常任务
│   ├── orochi/    # 御魂（5 个 PNG + config.json）
│   ├── exploration/ # 探索（config.json 仅骨架，PNG 缺失）
│   └── wanted/    # 悬赏封印（config.json 仅骨架，PNG 缺失）
└── activity/      # 活动（2 个 PNG + config.json）
```

资源配置使用 JSON。模板 ID 命名规则：

- `I_`：图像模板，例如 `I_OROCHI_FIRE`
- `O_`：OCR 区域，例如 `O_LAYER_8`
- `L_`：列表识别，例如 `L_LAYER_LIST`
- `A_`：动作或活动入口，可选

## 8. 测试策略

### 8.1 dry-run ✅

不连接设备，不加载真实图片，通过测试桩模拟识别和动作。

适用场景：

- 任务流程验证
- 活动流程验证
- 奖励弹窗消失等状态变化验证

当前已有：`OrochiDryRun`、`TestResourceLoader`。

### 8.2 单元测试 ❌

后续可引入测试框架，例如 Catch2 或 GoogleTest。当前先使用可执行工具验证关键链路。

### 8.3 设备联调 ❌

真实模拟器联调时需要验证：

- 截图是否成功
- 模板匹配是否准确
- 坐标缩放是否正确
- 点击和滑动是否生效
- 任务失败是否可恢复

**前置条件**：OpenCV 集成 + MaaCore Controller 桥接。

## 9. 编码规范

1. 业务层依赖抽象接口，不直接依赖 MAA 具体实现。
2. 活动模块和日常模块分目录管理。
3. 单个类保持单一职责。
4. 新增能力必须提供最小验证方式。
5. 资源 ID 应集中维护，避免散落硬编码。

## 10. 已知技术债务

1. **日志系统**：使用 `printf` 宏（`LOG_INFO` 等），多个文件重复定义，未接入 MaaUtils Logger。
2. **YYSConfig 幽灵类**：`YYSContext` 中前向声明但无定义。
3. **Flow 未使用**：`YYSWaitFlow` 等已实现但业务代码手写循环。
4. **MaaUtils 链接被注释**：Logger、Platform 等能力未接入。
5. **Flow 子 CMakeLists.txt**：定义了 `YYSFlow` 目标但未被引用，且有循环依赖。
