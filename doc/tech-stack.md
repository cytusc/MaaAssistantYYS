# 使用技术栈

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 全部 Sprint 完成，反映当前完整技术栈状态
负责人: AI Assistant
-->

## 1. 编程语言

- C++20：主项目语言，承接 MAA 的核心框架风格。
- Bash：静态扫描脚本（`scripts/static_scan.sh`）。
- Python：辅助工具（OpenCV 包解压等）。

## 2. 构建系统

- CMake 3.20+
- Ninja 生成后端
- MinGW GCC 15.2.0（MSYS2 MinGW-w64）

当前构建目标（10 个）：

| 目标名 | 类型 | 说明 |
|--------|------|------|
| `MaaYYS` | 静态库 | 核心业务库 |
| `OrochiDryRun` | 可执行文件 | 御魂任务 dry-run |
| `ExplorationDryRun` | 可执行文件 | 探索任务 dry-run |
| `Sprint5DryRun` | 可执行文件 | 悬赏/金币/签到 dry-run |
| `TestResourceLoader` | 可执行文件 | 资源加载测试 |
| `ResourceVerifier` | 可执行文件 | 资源完整性检查 |
| `YYSResourceProbe` | 可执行文件 | 资源探针 |
| `UnitTests` | 可执行文件 | 23 个单元测试 |
| `DeviceTest` | 可执行文件 | 设备连接验证 |
| `TemplateMatchVerify` | 可执行文件 | OpenCV 模板匹配验证（仅 OpenCV 模式） |

## 3. 核心底座

| 组件 | 代码位置 | Include 路径 | 链接状态 | 实际使用 |
|------|----------|-------------|----------|---------|
| MaaCore | `src/MaaCore/` | ✅ 已配置 | N/A | 仅 API 参考，不直接引用 |
| MaaUtils | `src/MaaUtils/` | ✅ 已配置 | 🟡 被注释 | 仅 meojson（JSON 解析） |

MaaYYS 通过 `AdbDirectBridge` 直接执行 adb 命令控制设备，不依赖 MaaCore 内部类。

## 4. 图像识别

| 组件 | 状态 | 说明 |
|------|------|------|
| `ITemplateResolver` | ✅ | 模板匹配抽象接口 |
| `StaticTemplateResolver` | ✅ | 静态映射测试桩 |
| `ScriptedTemplateResolver` | ✅ | 序列化测试桩 |
| `MaaTemplateResolver` | ✅ | OpenCV 真实匹配 + 降级模式 |
| `IOcrResolver` | ✅ | OCR 识别抽象接口 |
| `ScriptedOcrResolver` | ✅ | 序列化 OCR 测试桩 |

OpenCV 匹配方案：`cv::matchTemplate` + ROI 裁剪 + 阈值过滤 + 多分辨率缩放。

## 5. 设备控制

| 组件 | 状态 | 说明 |
|------|------|------|
| `IActionExecutor` | ✅ | 设备动作抽象接口 |
| `RecordingActionExecutor` | ✅ | 记录动作到向量（测试桩） |
| `MaaControllerActionExecutor` | ✅ | 回调适配器 |
| `BridgedActionExecutor` | ✅ | 桥接 MaaCoreBridge → IActionExecutor |
| `AdbDirectBridge` | ✅ | 通过 adb 命令直接控制设备 |
| `DeviceConnector` | ✅ | 一站式创建真实设备上下文 |

## 6. 第三方依赖

| 依赖 | 版本 | 用途 | 状态 |
|------|------|------|------|
| meojson | - | JSON 解析 | ✅ 已集成 |
| OpenCV | 4.13.0 | 图像匹配 | ✅ 已安装（条件编译 `MAAYYS_USE_OPENCV`） |
| MaaUtils | - | Logger/Platform/IO | 🟡 链接被注释 |

## 7. 资源格式

```text
resource/YYS/
├── template_index.json       # 模板索引
├── base/ (5 PNG + config.json)
├── daily/
│   ├── config.json           # 全局日常配置
│   ├── orochi/               # 御魂（5 PNG + config.json）
│   ├── exploration/          # 探索（7 PNG + config.json）
│   ├── wanted/               # 悬赏封印（5 PNG + config.json）
│   ├── coin_monster/         # 金币妖怪（3 PNG + config.json）
│   ├── sign_in/              # 签到（3 PNG + config.json）
│   ├── nian_beast/           # 年兽（3 PNG + config.json）
│   ├── pets/                 # 小动物（3 PNG + config.json）
│   ├── guild/                # 阴阳寮（3 PNG + config.json）
│   ├── area_boss/            # 地域鬼王（3 PNG + config.json）
│   ├── secret_dungeon/       # 秘闻（3 PNG + config.json）
│   ├── true_snake/           # 真蛇（3 PNG + config.json）
│   ├── soul/                 # 御灵（3 PNG + config.json）
│   └── awakening/            # 觉醒（3 PNG + config.json）
└── activity/
    ├── config.json           # 活动全局配置
    ├── template/common/      # 通用活动模板
    └── tower_sample/         # 样板活动（3 PNG + config.json）
```

模板 ID 命名规则：`I_` 图像模板、`O_` OCR 区域。

## 8. 测试策略

| 类型 | 状态 | 工具 |
|------|------|------|
| dry-run | ✅ | OrochiDryRun / ExplorationDryRun / Sprint5DryRun |
| 单元测试 | ✅ | UnitTests（23 个测试，内置框架） |
| 静态扫描 | ✅ | `scripts/static_scan.sh` |
| 资源验证 | ✅ | ResourceVerifier / YYSResourceProbe / TestResourceLoader |
| 设备联调 | 🟡 待设备 | DeviceTest / TemplateMatchVerify |

## 9. 编码规范

1. 业务层依赖抽象接口（IActionExecutor / ITemplateResolver / IOcrResolver），不直接依赖具体实现。
2. 日常任务使用 Flow 组件（YYSWaitFlow / YYSActionFlow / YYSRetryFlow），不手写循环。
3. 日志使用 `YYS_LOG_*` 宏，不使用 `printf`。
4. 单个任务文件超过 300 行必须拆分为子组件。
5. 资源 ID 由 config.json 驱动，不在代码中硬编码。

## 10. 已知技术债务

1. MaaUtils `target_link_libraries` 被注释（待 MaaUtils 自身编译配置完成）。
2. 占位 PNG 需替换为真实游戏截图裁剪。
3. 真实 OCR 执行器待实现（接口已就绪）。
