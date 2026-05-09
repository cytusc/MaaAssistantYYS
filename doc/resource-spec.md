# 资源规范

<!--
文档版本: 3.1
最后更新: 2026-05-09
更新内容: 补全 13 个日常任务 + 活动样板的资源清单，更新资源完整性统计
负责人: AI Assistant
-->

## 1. 资源目录总览

资源统一放在：

```text
resource/YYS/
├── template_index.json    # 模板索引文件
├── base/                  # 全局通用资源（5 PNG + config.json）
├── daily/                 # 日常任务资源（13 个子目录）
│   ├── config.json        # 日常总配置
│   ├── orochi/            # 御魂（5 PNG）
│   ├── exploration/       # 探索（7 PNG）
│   ├── wanted/            # 悬赏封印（5 PNG）
│   ├── coin_monster/      # 金币妖怪（3 PNG）
│   ├── sign_in/           # 签到（3 PNG）
│   ├── area_boss/         # 地域鬼王（3 PNG）
│   ├── true_snake/        # 真蛇（3 PNG）
│   ├── soul/              # 御灵（3 PNG）
│   ├── awakening/         # 觉醒（3 PNG）
│   ├── nian_beast/        # 年兽（3 PNG）
│   ├── secret_dungeon/    # 秘闻（3 PNG）
│   ├── pets/              # 小动物（3 PNG）
│   └── guild/             # 阴阳寮（3 PNG）
└── activity/              # 活动资源
    ├── config.json        # 活动列表配置
    ├── template/common/   # 通用活动模板（2 PNG）
    └── tower_sample/      # 样板活动（3 PNG + config.json）
```

**统计**：57 PNG + 18 JSON。

## 2. base 目录

`base` 存放全局通用资源。

```text
resource/YYS/base/
├── config.json
└── template/
    ├── I_CONFIRM.png
    ├── I_CANCEL.png
    ├── I_CLOSE.png
    ├── I_BACK.png
    └── I_START.png
```

## 3. daily 目录

`daily` 存放长期稳定的日常任务资源。每个任务一个子目录，包含 `config.json` 和 `template/` PNG 文件。

### 3.1 资源清单

| 任务 | task_id | PNG 数 | OCR | 真实截图 |
|------|---------|--------|-----|---------|
| 御魂 | orochi | 5 | O_LAYER (digit) | ✅ 真实 |
| 探索 | exploration | 7 | — | 占位 |
| 悬赏封印 | wanted | 5 | O_TASK (full) | 占位 |
| 金币妖怪 | coin_monster | 3 | — | 占位 |
| 签到 | sign_in | 3 | — | 占位 |
| 地域鬼王 | area_boss | 3 | — | 占位 |
| 真蛇 | true_snake | 3 | — | 占位 |
| 御灵 | soul | 3 | — | 占位 |
| 觉醒 | awakening | 3 | — | 占位 |
| 年兽 | nian_beast | 3 | — | 占位 |
| 秘闻 | secret_dungeon | 3 | — | 占位 |
| 小动物 | pets | 3 | — | 占位 |
| 阴阳寮 | guild | 3 | — | 占位 |

### 3.2 日常总配置

`resource/YYS/daily/config.json` 列出可调度的日常任务。当前内容仅列部分任务，实际注册在 `YYSDailyManager::register_default_tasks()` 中完成（13 个任务全部硬编码注册）。

## 4. activity 目录

`activity` 存放每期活动资源。

### 4.1 当前内容

```text
resource/YYS/activity/
├── config.json                    # 活动列表
├── template/common/
│   ├── I_ACTIVITY_ENTER.png       # 活动入口
│   └── I_CHALLENGE.png            # 挑战按钮
└── tower_sample/                  # 样板活动
    ├── config.json                # 活动配置（activity_id, steps, template）
    └── template/
        ├── I_TOWER_ENTER.png
        ├── I_TOWER_FLOOR.png
        └── I_TOWER_REWARD.png
```

### 4.2 活动更新原则

1. 新活动新目录（`resource/YYS/activity/<activity_id>/`）。
2. 不覆盖旧活动资源。
3. 不修改 daily 目录。
4. 活动结束后可以禁用，不必立即删除。
5. `ConfigDrivenActivity` 可通过 `load_activity_from_config()` 从 config.json 自动加载。

## 5. 命名规范

### 5.1 模板 ID

- `I_`：图像模板，例如 `I_OROCHI_FIRE`。
- `O_`：OCR 区域，例如 `O_LAYER`。

### 5.2 文件命名

文件名与模板 ID 保持一致：`I_OROCHI_FIRE.png`。

## 6. 配置字段规范

模板配置字段：

```json
{
    "id": "I_OROCHI_FIRE",
    "file": "I_OROCHI_FIRE.png",
    "roi": [1095, 577, 131, 124],
    "threshold": 0.6,
    "desc": "御魂挑战按钮"
}
```

OCR 配置字段：

```json
{
    "id": "O_LAYER",
    "roi": [138, 130, 359, 500],
    "mode": "digit",
    "keywords": ["壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾"]
}
```

## 7. 坐标规范

- 默认分辨率：1280×720，原点左上角。
- ROI 格式：`[x, y, width, height]`。
- 多分辨率适配由 `MaaTemplateResolver` 的缩放逻辑处理。

## 8. 资源校验工具

| 工具 | 说明 | 构建目标 |
|------|------|----------|
| `ResourceVerifier` | 列出全部模板和 OCR 资源 | ✅ 已加入 CMakeLists.txt |
| `YYSResourceProbe` | 检查关键资源是否存在 | ✅ 已加入 CMakeLists.txt |
| `TestResourceLoader` | 验证资源加载和文件存在性 | ✅ 已加入 CMakeLists.txt |

## 9. 资源完整性总结

| 资源分类 | config.json | PNG 文件 | 完整性 |
|----------|-------------|----------|--------|
| base | ✅ | ✅ 5/5 | 完整（真实截图） |
| daily/orochi | ✅ | ✅ 5/5 | 完整（真实截图） |
| daily/exploration | ✅ | ✅ 7/7 | 占位 PNG |
| daily/wanted | ✅ | ✅ 5/5 | 占位 PNG |
| daily/coin_monster | ✅ | ✅ 3/3 | 占位 PNG |
| daily/sign_in | ✅ | ✅ 3/3 | 占位 PNG |
| daily/area_boss | ✅ | ✅ 3/3 | 占位 PNG |
| daily/true_snake | ✅ | ✅ 3/3 | 占位 PNG |
| daily/soul | ✅ | ✅ 3/3 | 占位 PNG |
| daily/awakening | ✅ | ✅ 3/3 | 占位 PNG |
| daily/nian_beast | ✅ | ✅ 3/3 | 占位 PNG |
| daily/secret_dungeon | ✅ | ✅ 3/3 | 占位 PNG |
| daily/pets | ✅ | ✅ 3/3 | 占位 PNG |
| daily/guild | ✅ | ✅ 3/3 | 占位 PNG |
| activity/common | ✅ | ✅ 2/2 | 占位 PNG |
| activity/tower_sample | ✅ | ✅ 3/3 | 占位 PNG |

**说明**：除 base 和 orochi 外，其余 PNG 为 1×1 占位文件，需从游戏截图裁剪替换后才能用于真实设备联调。
