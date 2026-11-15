# Windows 兼容性记录

| 项目 | Mac Editor | Win Editor | Win Runtime | 备注 |
|---|---|---|---|---|
| ModelContextProtocol（官方，实验性） | 已验证（5.8.4） | 未验证 | 不应依赖 | uproject 中 TargetAllowList=Editor，不进入打包游戏 |
| MCPClientToolset | 已验证（加载） | 未验证 | 不应依赖 | 同上 |
| AllToolsets | 未验证（LOKI 内） | 未验证 | 不应依赖 | 同上；是否必需待 S02 验证后评估 |

- 工程为纯 Blueprint 模板（TP_ThirdPersonBP），暂无 C++ 模块，无双编译器风险。
- 工程路径与资产名使用英文与数字。
- Windows 打包、运行均未验证；需要真实 Windows 环境单独测试。

- 2026-11-15（C1）：LOKI 模块开始有正式 C++ 玩法代码（`ULokiHealthComponent`）。Windows 需 VS 2022 工具链编译 LOKI 模块；代码只用引擎标准 API（OnTakeAnyDamage 动态委托、FMath），无平台相关代码。未在 Windows 编译验证。
