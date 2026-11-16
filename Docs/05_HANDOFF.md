# LOKI 交接说明（Claude Code → Codex）

> 写于 2026-11-15。读者：接手的 AI Agent。用户（导演/验收者）几乎不懂 UE，用中文沟通，**每次只做一个最小步骤，等用户明确说"通过，继续"才进入下一步**。
> 先读：`CLAUDE.md`、`Docs/LOKI_UE5_Claude_MCP_项目总设计与分步验收_v1.0.md`（含 10 条规则、S00–S15 路线图、附录 A 剧情；剧情部分只读，只能提案）、`Docs/01_DEV_LOG.md`、`Docs/02_ACCEPTANCE_LOG.md`。

## 0. 最新（2026-11-16，以 Docs/STATE.md 为准）
- 本文第 1 节是早期交接（S12 前），已过时；当前进度、用户决定、下一步一律看 `Docs/STATE.md` 和 `01_DEV_LOG.md` 最后几节。
- 序章按设计文档的牢房方案；断崖方案作废。
- 下一位 Agent 首先：确认用户是否同意安装 git-lfs；然后等用户下令 C2。

## 1. 当前状态
- S00–S11 全部**用户验收通过**（S08/S10/S11 经返工）。S11 返工由 Codex 完成（死亡布娃娃碰撞 + 追击距离），用户确认通过。
- **下一步：S12（玩家死亡/失败后的检查点重试）**，用户尚未说"继续 S12"，不要自行开始。
- 玩家目前**没有生命值**，守卫**不会攻击**（没有任何敌方伤害）。S12 需要先决定"失败条件"（守卫抓到/碰到玩家？受伤？）——这是设计选择，应先问用户。
- 工程：`/Users/rain/DevGame/Loki/LOKI.uproject`，UE 5.8.4，Mac 开发；Git 仓库已建（Codex 建的，分支 main），Git 提交需用户批准。
- 所有测试关卡：`/Game/LOKI/Levels/Lvl_S04_Collision`（**不是默认地图**，默认地图仍是 `/Game/ThirdPerson/Lvl_ThirdPerson`；该模板关卡从未被修改）。

## 2. 操作（键位）
| 键 | 功能 |
|---|---|
| WASD / 鼠标 | 移动 / 视角（模板） |
| 空格 | 跳（模板） |
| 左 Shift | 奔跑（速度 600→900，S04） |
| E | 靠近门/NPC 时交互（开门 / 对话 / 关闭对话） |
| 1 / 2 | NPC 提问时选"好 / 不用"（S07，临时的屏幕文字方案，不是 UMG） |
| Q | 幻术：在原位留下不透明"残影"，自己半透明隐身 4 秒，8 秒冷却（S10） |
| 鼠标左键 | 近战：前方 120cm、半径 180，伤害 34，冷却 0.6 秒；隐身时=背刺伤害 100（S11） |

## 3. 已建立的资产与职责（均在 `/Game/LOKI/` 下，另有对模板角色的修改）
- `Gameplay/BP_Door`：门。根组件是 DoorMesh（Cube）；InteractBox 触发盒；按 E（K2Node_InputKey + EnableInput）开门，用 MoveComponentTo 上升 300cm；自定义事件 `OpenDoor` 可被外部调用；`bOpen` 只开一次。
- `Gameplay/BP_NPC_Test`：NPC（Manny，Idle）。进入 TalkBox → "Press E to talk"；E 开始对话（PrintString，Key=NpcDialog），锁移动；1/2 选择，写 `BP_LokiGameInstance.StoryFlag_AcceptedHelp`；选 1 调用 `Door.OpenDoor`；E 关闭并解锁。**对白全是占位句，不是剧情**。
- `Gameplay/BP_LokiGameInstance`：GameInstance，公开变量 `StoryFlag_AcceptedHelp`；已在 `Config/DefaultEngine.ini` 的 `[/Script/EngineSettings.GameMapsSettings] GameInstanceClass` 设置。
- `Gameplay/BP_Guard`：守卫（Character，Quinn + ABP_Unarmed；`NavMovementProperties.bUseAccelerationForPaths=true`，否则走路动画不播）。行为：
  - 巡逻：BeginPlay 记 A，B=A+`PatrolOffset`（默认 0,700,0）；`Patrol` 事件循环（MoveToLocation → 翻转 → Delay 5 → 再 Patrol）；追击/分心/死亡时循环改为每秒轮询。
  - 感知：`PawnSensing`（视距 1200，PeripheralVisionAngle 90=前方 180°，0.2 秒）+ `ProximityLoop`（每 0.2 秒，`ProximityRadius`=300cm 内且 `LineOfSightTo` 通过则发现）。两者都走同一个闸门：`bChasing OR bDistracted OR 玩家隐身(bCloaked)` 时忽略。
  - 追击：`ChaseLoop`（Codex 已改为每 0.15 秒重发 `MoveToActor`，接受半径 25）；超过 `LoseTargetSeconds`(4) 没看到，或玩家隐身 → 放弃，AlertMark("!") 隐藏，速度回 200。
  - 诱饵：`DecoyAttract(Where)` → 放弃追击、bDistracted、速度 300、走向诱饵、4 秒后恢复。
  - 受伤：`ReceiveAnyDamage`：`Health`(100) 减伤害，屏幕显示血量，闪 M_Ghost 0.15 秒，被打后 SpotPlayer（受闸门约束）；<=0 → 死亡（bDead、停止、碰撞/布娃娃，Codex 已修成自然倒地）→ 5 秒后销毁。
- `Gameplay/BP_Decoy`：残影（Quinn + MM_Idle，无碰撞，寿命 5 秒）；BeginPlay 用 `K2_ProjectPointToNavigation`（范围 800,800,500）吸附到导航网格（失败则自毁），然后通知 ≤`AttractRadius`(1500) 的守卫。
- `Materials/M_Ghost`：半透明幻影材质（Translucent/Unlit/双面，参数 GhostColor、GhostOpacity；**已勾 bUsedWithSkeletalMesh**，否则游戏里退回默认材质）。
- `Levels/Lvl_S04_Collision`：灰盒测试关卡。含 Floor、四周墙（Wall_Front/Left/Right）、Block_Low、Wall_Hide（遮挡墙）、Door_01（450,0,150；缩放 0.2×19.5×3）、NPC_Test_01、Guard_01（-700,-600）、NavBounds_S04（NavMeshBoundsVolume）、PlayerStart(-1000,0,100)、Sun、SkyLight。
- 对模板的改动（只**新增**，未改原逻辑）：
  - `Input/Actions/IA_Sprint` + `IMC_Default` 里加 LeftShift 映射；
  - `ThirdPerson/Blueprints/BP_ThirdPersonCharacter`：IA_Sprint → MaxWalkSpeed 900/600；Q 键（幻术：Sequence 分冷却链与隐身链）；左键攻击；变量 bIllusionReady、IllusionCooldown(8)、bCloaked、CloakSeconds(4)、bCanAttack、AttackDamage(34)、BackstabDamage(100)。
- 配置：`Config/DefaultEngine.ini` 增加 `GameInstanceClass`（改前备份：`Docs/_backup_pre_enginewire/DefaultEngine.ini.pre_S07`）。

## 4. MCP / 工具现状与经验（重要）
- `unreal` MCP：`http://127.0.0.1:8765/mcp`，工程根目录 `.mcp.json`（Claude Code）和 `.codex/config.toml`（Codex）都已指向它，头由 `Tools/mcp_headers.sh` 提供（令牌开启）。该服务是 EngineWire 插件（`Plugins/EngineWire`，仅 Editor，TargetAllowList=Editor）。官方 MCP 的 `bAutoStartServer=False`。**两者不能同时用 8765**。
- 工具是单个 `unreal`，四步：search → describe → execute →（读 warnings）。失败会返回 `nextCall`。`blueprint.edit_graph` 的 `batch` + `$id` 引用最好用；`dryRun` 能校验函数/变量，但**不知道同一批里新建的自定义事件**（要先建事件再建调用）。
- 经验：
  1. **中文本地化引脚名**：如 `AsAI控制器`（Cast 输出）要原样使用。
  2. **PIE 与蓝图编辑冲突**：PIE 运行时不能改 SCS（组件）；每次编辑前先 `control_editor.play` 的 `control:stop`。用户的试玩会留下 PIE，常需先停。
  3. **PIE 内修改只在 PIE 内有效**，结束即丢弃；`inspect.set_property` 对资产是永久的。
  4. 运行时直接写 `PawnSensing.PeripheralVisionAngle` **不会更新**内部视角余弦，测试视野要改资产再重启 PIE。
  5. 新建材质要用于骨骼网格，必须 `bUsedWithSkeletalMesh=true`（`inspect.set_property` 可设），否则 PIE 日志会有 "missing usage flag" 警告且实际用默认材质。
  6. 材质创建/节点需要 `consent`（先 describe 取 nonce，用一次作废）。
  7. 守卫每 5 秒会掉头（Patrol 循环），做传送/感知测试时有时序干扰；冻结守卫的办法：PIE 内对 `CharMoveComp.MaxWalkSpeed` 设 0。
  8. 读 PIE 运行时对象：`inspect.get_property` 的 `objectPath` 可用 `PlayerPawn`、`GameInstance` 等；`control_actor.get_transform` 的 `readMode:motion` 可模拟按键（`inputs`）并采样属性。
  9. 项目里 `InputKey` 节点必须用 `set_node_property InputKey=<键名>` 设键（One、Two、Q、E、LeftMouseButton）。
  10. `MoveToLocation/MoveToActor` 不是延迟节点；`MoveToLocation` 的 `bProject...` 默认不吸附导航网格。
- `ABP_Unarmed` 的 ShouldMove 依赖 `GetCurrentAcceleration`，AI 角色必须 `bUseAccelerationForPaths`。

11. **中文编辑器下 `write_graph_from_code` 的坑**（2026-11-15）：`(if ...)`、`(not ...)` 等简写会被译成英文节点名（Utilities|FlowControl|Branch）而失败；事件名要用已存在的节点（BeginPlay 需先用 edit_graph create_node Event ReceiveBeginPlay 建出来）；组件变量 getter 是 `变量|默认|获取HealthComp`。绑定 C++ 委托用 edit_graph `nodeType: BindEvent`（会自动建好签名正确的自定义事件）。复杂改线用 edit_graph batch（先 dryRun）。
13. **改 C++ 头文件后要接蓝图，先重启编辑器**：热重载后新旧类并存，蓝图连线会报"X 与 X 不兼容"。流程：用户关编辑器 → `Build.sh` → 重开。只改函数体可继续用 `Module Recompile LOKI`。
14. **PIE 里 F1 = UE 自带线框模式**（F3 恢复），和本项目 F1 调试开关同时触发。
12. **Claude 桌面 App 会把会话开在 `.claude/worktrees/` 里**：编辑器用的是主目录 `/Users/rain/DevGame/Loki`，所有改动要落在主目录 main 上（用户已批准）。

## 5. 已知问题 / 技术债（未处理，已知）
- 编译提示：`BP_Guard` 的 `DecoyAttract` 在 Delay 后读 `bChasing`（可能被后续运行覆盖）。冷却 8 秒内不会触发，暂不影响。
- 对话/选项界面是 PrintString，临时方案；S13 前后需要 UMG（用户之前希望做按钮）。
- 门的缩放/触发盒大小互相补偿，换缩放要同步改；蓝图结构可重构为 DefaultSceneRoot + 子网格。
- 诱饵吸附后 z=10（导航面高度）。
- 无攻击动画（挥击/受击），也没有玩家生命值/死亡。
- 守卫"看到后永久追"受 4 秒丢失约束；追击期间守卫始终知道玩家真实位置（没改成"追最后看到的位置"）。
- 全是灰盒 + 模板人偶，网格材质默认；没有声音。
- Windows：**完全未验证**（`Docs/03_WINDOWS_COMPAT.md` 需在新增插件/C++ 时更新）。C++ 模块是最小占位（Source/LOKI），Windows 需要 VS 工具链。
- 素材：用户已在 Fab 领取 **Last Wizard FREE**（在 `/Users/Shared/UnrealEngine/Launcher/VaultCache/LastWiza8ac318b6d2ecV1/data/Content/LastWizard`：城堡房间构件、带 Idle/Walk/Attack/Death 的骑士、门、火炬、桌子等，文档称支持 UE 5.8，约 205MB）。**尚未导入 LOKI**，`Docs/04_ASSET_LICENSES.md` **尚未创建**。导入前先问用户；导入后必须登记来源与许可。用户的计划：灰盒阶段用免费素材，最终版再换高清付费素材。

## 6. 工作规则摘要（来自 CLAUDE.md，必须遵守）
1. 一次一个最小步骤；做完停下等用户"通过，继续"；"未通过"只修当前步骤。
2. 回复模板：步骤编号 / 本步目标 / 实际完成 / 修改的文件 / 实际执行的测试 / 已知风险 / 请你人工验收 / 回退方式 / 状态。
3. 修改前说明、修改后说明；不删资产、不覆盖保存现有关卡（新建关卡/资产可以）；不确定先问。
4. 不要自行编造剧情，不要把"待定"写成"已定"。
5. 每步更新 `Docs/01_DEV_LOG.md`、`Docs/02_ACCEPTANCE_LOG.md`；Windows 风险记入 `Docs/03_WINDOWS_COMPAT.md`。
6. MCP 插件仅 Editor；Git 提交需用户批准；不提交 Saved/、Intermediate/、DerivedDataCache/、Binaries/。
7. 不能把 Agent 自测当作用户验收；PIE 数据证据 ≠ 肉眼观感。
8. 用户偏好：中文、简洁；会在试玩后给具体观感反馈，请据此只修反馈指出的问题。

## 7. 路线图（总设计 §8）
S12 玩家死亡/失败后的检查点重试 → S13 短 Sequencer 实时镜头并返回控制 → S14 串成 2–3 分钟灰盒流程 → S15+ 逐项替换写实资产（需用户批准，登记授权）。

## 7.5 C++ 系统（正式玩法代码）
- `ULokiHealthComponent`（C1，`Source/LOKI/LokiHealthComponent.*`）：玩家 `BP_ThirdPersonCharacter.HealthComp` 已使用。自动接收 ApplyDamage；`bInvulnerable` 由开场镜头 `BP_CutsceneTrigger` 设置；蓝图通过 OnHealthChanged / OnDeath 做提示与重载。守卫 BP_Guard 仍用自己的 Health 变量（C2 再迁），但攻击前用 `IsActorDead` 检查目标。`SetDebugDisplay` 由 F1 调试开关驱动，显示 HP 与无敌状态。守卫 CharacterMesh0 对 Camera 通道 Ignore（防闪黑）。

## 8. C++ 迭代链路（2026-11-15 实测，Mac）
- 改 `Source/LOKI/*.cpp/.h` 后，在编辑器里执行控制台命令 `Module Recompile LOKI`（可用 MCP `system_control.console_command`），约 8–10 秒热重载，**不用退出编辑器**；Mac 没有 Live Coding。
- 新建 C++ 类用 MCP `system_control.create_cpp_class`（不能带 subfolder，LOKI 模块没有 Public/Private）；首次编译约 35 秒。
- 命令行构建：`Engine/Build/BatchFiles/Mac/Build.sh LOKIEditor Mac Development -Project="/Users/rain/DevGame/Loki/LOKI.uproject"`（约 10 秒增量），之后在编辑器里 `Module Reload LOKI`。MCP 的 `run_build kind=ubt` 路径有 bug，别用。
- 测试类 `LokiCompileTest` 仍在 Source/LOKI，可用 MCP `remove_cpp_class`（需用户同意）或手动删除。
- 头文件布局变化（改 UPROPERTY 等）的热重载稳定性、Windows 上的 Live Coding 均未验证。
