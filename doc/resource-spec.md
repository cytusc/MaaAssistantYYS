# 资源规范

<!--
文档版本: 2.0
最后更新: 2026-05-09
更新内容: 补充实际资源清单，标注缺失的 PNG 文件，增加 template_index.json 说明
负责人: AI Assistant
-->

## 1. 资源目录总览

资源统一放在：

```text
resource/YYS/
├── base/              # 全局通用资源
├── daily/             # 日常任务资源
│   ├── orochi/        # 御魂
│   ├── exploration/   # 探索（PNG 缺失）
│   └── wanted/        # 悬赏封印（PNG 缺失）
├── activity/          # 活动资源
└── template_index.json # 模板索引文件
```

## 2. base 目录

`base` 存放全局通用资源。

**当前实际内容**：

```text
resource/YYS/base/
├── config.json        # 游戏基础配置（包名、分辨率）
└── template/
    ├── I_CONFIRM.png  # 确认按钮
    ├── I_CANCEL.png   # 取消按钮
    ├── I_CLOSE.png    # 关闭按钮
    ├── I_BACK.png     # 返回按钮
    └── I_START.png    # 开始按钮
```

`config.json` 内容：

```json
{
    "package_name_official": "com.netease.onmyoji",
    "package_name_bili": "com.netease.onmyoji.bili",
    "default_resolution": [1280, 720]
}
```

## 3. daily 目录

`daily` 存放长期稳定的日常任务资源。

### 3.1 orochi（御魂）✅ 完整

```text
resource/YYS/daily/orochi/
├── config.json
└── template/
    ├── I_OROCHI.png        # 御魂入口图标
    ├── I_OROCHI_FIRE.png   # 挑战/开启战斗按钮
    ├── I_LAYER_LIST.png    # 层数列表
    ├── I_REWARD.png        # 奖励弹窗确认按钮
    └── O_LAYER_8.png       # 第8层选择器
```

`config.json` 定义的模板：

| ID | ROI | threshold | 描述 |
|----|-----|-----------|------|
| I_OROCHI | [55, 104, 295, 406] | 0.8 | 御魂入口图标 |
| I_OROCHI_FIRE | [1095, 577, 131, 124] | 0.6 | 挑战按钮 |
| I_REWARD | [500, 500, 120, 80] | 0.7 | 奖励确认 |
| O_LAYER_8 | [138, 130, 45, 88] | 0.8 | 第8层 |

`config.json` 定义的 OCR：

| ID | ROI | mode |
|----|-----|------|
| O_LAYER | [138, 130, 359, 500] | digit |

### 3.2 exploration（探索）🟡 仅骨架

```text
resource/YYS/daily/exploration/
└── config.json        # 模板 ID 已定义，但 PNG 文件缺失
```

`config.json` 定义的模板（PNG 缺失）：

| ID | ROI | threshold | 状态 |
|----|-----|-----------|------|
| I_EXPLORATION | [0, 0, 1280, 720] | 0.8 | ❌ PNG 缺失 |
| I_CHAPTER | [0, 0, 1280, 720] | 0.7 | ❌ PNG 缺失 |

### 3.3 wanted（悬赏封印）🟡 仅骨架

```text
resource/YYS/daily/wanted/
└── config.json        # 模板 ID 已定义，但 PNG 文件缺失
```

`config.json` 定义的模板（PNG 缺失）：

| ID | ROI | threshold | 状态 |
|----|-----|-----------|------|
| I_WANTED | [0, 0, 1280, 720] | 0.8 | ❌ PNG 缺失 |

`config.json` 定义的 OCR：

| ID | ROI | mode | 状态 |
|----|-----|------|------|
| O_TASK | [0, 0, 1280, 720] | full | 🟡 定义存在，无执行器 |

### 3.4 日常总配置

`resource/YYS/daily/config.json` 列出所有日常任务：

```json
{
    "tasks": [
        {"id": "orochi", "enabled": true},
        {"id": "exploration", "enabled": true},
        {"id": "wanted", "enabled": true},
        {"id": "gold_youkai", "enabled": true},
        {"id": "pets", "enabled": true}
    ]
}
```

## 4. activity 目录

`activity` 存放每期活动资源。活动必须独立目录。

**当前实际内容**：

```text
resource/YYS/activity/
├── config.json                    # 活动列表配置
└── template/
    └── common/
        ├── I_ACTIVITY_ENTER.png   # 活动入口
        └── I_CHALLENGE.png        # 挑战按钮
```

`config.json` 内容：

```json
{
    "activities": [
        {"id": "tower", "enabled": false},
        {"id": "demon", "enabled": false}
    ]
}
```

活动更新原则：

1. 新活动新目录。
2. 不覆盖旧活动资源。
3. 不修改 daily 目录。
4. 活动结束后可以禁用，不必立即删除。

## 5. template_index.json

`resource/YYS/template_index.json` 是模板索引文件，列出所有模板 PNG 的相对路径。当前索引内容：

| 分类 | 模板 ID | 路径 |
|------|---------|------|
| base | I_CONFIRM | base/template/I_CONFIRM.png |
| base | I_CANCEL | base/template/I_CANCEL.png |
| base | I_CLOSE | base/template/I_CLOSE.png |
| base | I_BACK | base/template/I_BACK.png |
| base | I_START | base/template/I_START.png |
| daily.orochi | I_OROCHI | daily/orochi/template/I_OROCHI.png |
| daily.orochi | I_OROCHI_FIRE | daily/orochi/template/I_OROCHI_FIRE.png |
| daily.orochi | I_LAYER_LIST | daily/orochi/template/I_LAYER_LIST.png |
| daily.exploration | (空) | — |
| daily.wanted | (空) | — |
| activity.common | I_ACTIVITY_ENTER | activity/template/common/I_ACTIVITY_ENTER.png |
| activity.common | I_CHALLENGE | activity/template/common/I_CHALLENGE.png |

## 6. 命名规范

### 6.1 模板 ID

- `I_`：图像模板，例如 `I_OROCHI_FIRE`。
- `O_`：OCR 区域，例如 `O_LAYER_8`。
- `L_`：列表识别，例如 `L_LAYER_LIST`。
- `A_`：动作配置，可选。

### 6.2 文件命名

建议文件名与模板 ID 保持一致：

```text
I_OROCHI_FIRE.png
O_LAYER_8.png
```

## 7. 配置字段规范

模板配置建议包含：

```json
{
    "id": "I_OROCHI_FIRE",
    "file": "template/I_OROCHI_FIRE.png",
    "roi": [1095, 577, 131, 124],
    "threshold": 0.6,
    "method": "TemplateMatch",
    "description": "御魂挑战按钮"
}
```

字段说明：

- `id`：资源唯一标识。
- `file`：模板文件路径（相对于 config.json 所在目录）。
- `roi`：识别区域，格式为 `[x, y, width, height]`。
- `threshold`：匹配阈值。
- `method`：识别方式。
- `description`：中文说明。

## 8. 坐标规范

默认坐标系：

- 宽度：1280
- 高度：720
- 原点：左上角
- ROI 格式：`x, y, width, height`

后续如果支持多分辨率，应由适配层统一缩放，不在业务任务中手写缩放。

## 9. OCR 规范

OCR 配置建议包含：

```json
{
    "id": "O_LAYER",
    "roi": [138, 130, 359, 500],
    "mode": "digit",
    "keywords": ["壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾"]
}
```

## 10. 资源校验

后续应提供资源校验工具，检查：

- 配置 JSON 是否可解析。
- 模板文件是否存在。
- ROI 是否越界。
- 模板 ID 是否重复。
- 活动资源是否污染 daily/base。

**当前状态**：`ResourceVerifier.cpp` 和 `YYSResourceProbe.cpp` 已实现但未加入 CMakeLists.txt 构建目标。

## 11. 当前资源完整性总结

| 资源分类 | config.json | PNG 文件 | 完整性 |
|----------|-------------|----------|--------|
| base | ✅ | ✅ 5/5 | 完整 |
| daily/orochi | ✅ | ✅ 5/5 | 完整 |
| daily/exploration | ✅ | ❌ 0/2 | PNG 缺失 |
| daily/wanted | ✅ | ❌ 0/1 | PNG 缺失 |
| activity | ✅ | ✅ 2/2 | 样板完整 |
