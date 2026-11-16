# LOKI 开发日志

## 2026-11-15
### S00 环境盘点（只读）
- macOS / Apple M4 Max；UE 5.8.4（++UE5+Release-5.8，已发布版），安装于 /Users/Shared/Epic Games/UE_5.8
- 官方 Unreal MCP 插件（ModelContextProtocol）在 UE58_MCP_Test 上实测可用

### S01 MCP 创建/保存/重读（测试工程 UE58_MCP_Test）
- 通过 MCP 创建 Cube、材质、Blueprint；保存资产；关卡 L_S01 由用户手动保存（MCP 无“新建/另存关卡”工具）
- 重新加载关卡后 Actor 仍在；PIE 日志验证 BeginPlay 触发
- 状态：用户已手动保存，等待 S01 验收

### S02 建立 LOKI 工程（进行中）
- 来源：UE_5.8/Templates/TP_ThirdPersonBP + TemplateResources/High 的 Characters、Input、LevelPrototyping（无 Variant）
- 位置：/Users/rain/DevGame/Loki/LOKI.uproject，EngineAssociation=5.8，GameName=LOKI
- 已启用插件（仅 Editor）：ModelContextProtocol、MCPClientToolset、AllToolsets
- 未做：Git 初始化、.gitignore、任何玩法修改
- 未测试：尚未打开 LOKI 工程（等待用户首次打开）
- 2026-11-15 18:5x：用户已打开 LOKI 并试玩（WASD/鼠标/跳跃正常，无报错、无卡顿）。
- MCP 未连上：设置类 UModelContextProtocolSettings 为 config=EditorPerProjectUserSettings（按工程保存），默认端口 8000、bAutoStartServer=false。
- 已在 Config/DefaultEditorPerProjectUserSettings.ini 写入 ServerUrlPath=/mcp、ServerPortNumber=8765、bAutoStartServer=True；需重启 LOKI 编辑器后生效（未验证）。

### MCP-EVAL 社区 MCP（ChiR24/Unreal_mcp）只读评估 — 2026-11-15
- 仓库已克隆到临时目录做只读检查，未安装进任何工程。MIT 许可。版本 0.6.1-beta-b（beta），插件已更名 EngineWire（旧名 McpAutomationBridge），最新提交 2026-11-10。
- 声明支持 UE 5.0–5.8、Win64/Mac/Linux 编辑器；仅编辑器插件，不进入打包游戏。
- Mac 需 Xcode（已发现 /Applications/Xcode.app）；工程必须含 C++ 代码才能从源码编译，预编译包与引擎小版本和平台绑定，未找到 5.8 Mac 预编译包的确认。
- 原生 HTTP 服务默认端口 3000，默认带 capability token（存于 Saved/MCP/）。
- 5.8.4 + Mac 的实际可用性：未验证。
- 用户决定：隔离试用，不影响 LOKI 与官方 MCP。S02 已记为通过。

### MCP-TRIAL-1 EngineWire 隔离试用工程搭建 — 2026-11-15
- 位置：Loki/_Trial_EngineWire/（临时，试用结束后删除）；EWTrial.uproject，最小 C++ 模块（由 TP_Blank 重命名，BuildSettings V7 / Unreal5_8）。
- 插件：Plugins/EngineWire（来自 ChiR24/Unreal_mcp，0.6.1-beta-b，MIT），TargetAllowList=Editor。
- 配置：Config/DefaultGame.ini → EngineWireSettings：NativeMCPPort=8765、bRequireCapabilityToken=False（仅试用，回环地址；若迁移需重新开启令牌）。
- 未测试：尚未编译/打开。注意：与 LOKI 的官方 MCP 同用 8765，二者不能同时打开。
[2026-11-15] 新增 CLAUDE.md（供 Code 会话读取）

### MCP-MIGRATE 迁移到 EngineWire — 2026-11-15
- 试用结论：EngineWire 在 5.8.4/Mac 编译通过；建关卡、保存、放置物体全部成功，0 步人工操作。用户批准迁移。
- LOKI 变更：新增 Plugins/EngineWire（源码，Editor 专用）、Source/LOKI 最小 C++ 模块 + Target.cs、LOKI.uproject 增加 Modules/EngineWire、DefaultGame.ini 增加 EngineWireSettings（端口 8765，令牌开启）、官方 MCP bAutoStartServer 改为 False、新增 .mcp.json、Tools/mcp_headers.sh、.gitignore、CLAUDE.md。
- 备份：Docs/_backup_pre_enginewire/（改动前的 uproject 与两份 ini）。
- 未验证：LOKI 尚未首次编译；Windows 未验证（EngineWire 非运行时依赖）；C++ 模块使 Windows 需要 VS 工具链。
[2026-11-15] 总设计文件已放入 Docs/，CLAUDE.md 已指向它

### S03 第三人称移动/镜头基线 — 2026-11-15
- 结论：沿用 UE 第三人称模板，未修改任何资产/关卡/配置（零写入）。
- 核对：GlobalDefaultGameMode=BP_ThirdPersonGameMode；BP_ThirdPersonCharacter 含 CameraBoom(SpringArm)+FollowCamera，Move/Aim 函数，输入资产 Content/Input（IMC_Default、IMC_MouseLook、IA_Move/Look/Jump/MouseLook）；两个 BP 均编译无错。
- MCP-MIGRATE 只读验收：Lvl_ThirdPerson 摘要 73 Actor（list 可见 64，另 9 个被排除）；编辑器关卡内无 ThirdPersonCharacter，仅 PlayerStart，PIE 启动后由 GameMode 生成 BP_ThirdPersonCharacter_C_0 —— 这解释了 find_actors 编辑器态找不到它。
- Agent PIE 实测（模拟按键，PIE 后已停止）：W 前进最高速度 600cm/s，约 0.5s 达 90%；D 转向右移；SpaceBar 起跳（升高约 120cm 后落地）；各键响应延迟 1 帧内。镜头抖动 peakJerk 308（出现在从出生高台走下边缘时）。
- 未验证：鼠标看四周（脚本无法模拟鼠标）；手感与镜头观感需人工；Windows 未验证。

### S04 奔跑 + 简单碰撞区域 — 2026-11-15
- 新增资产：/Game/Input/Actions/IA_Sprint（Digital）；IMC_Default 增加 LeftShift→IA_Sprint。
- 修改资产：BP_ThirdPersonCharacter EventGraph 新增 IA_Sprint 的 Started→MaxWalkSpeed=900、Completed→MaxWalkSpeed=600（其余图表未动），编译 0 错误 0 警告。
- 新增关卡：/Game/LOKI/Levels/Lvl_S04_Collision（空白新建；Sun、SkyLight、PlayerStart、Floor、Wall_Front/Left/Right、Block_Low，均为 UE BasicShapes Cube，默认网格材质）。Lvl_ThirdPerson 未修改；默认地图未改。
- Agent PIE 实测：W 走 600cm/s；按住 LeftShift 升至 900cm/s，松开回 600；全速冲向 Wall_Front（前表面 x=775）后停在 x=739.9 并保持不动（胶囊半径约 35cm），无穿透。
- 未验证：手柄奔跑（仅映射键盘 LeftShift）；奔跑动画（沿用模板，未加专用动画）；鼠标；Windows。
- 注意：奔跑速度写死在蓝图（600/900），后续如需统一管理再重构。

### S05 唯一可交互物：门 — 2026-11-15
- 新增资产：/Game/LOKI/Gameplay/BP_Door（Actor）。组件：DoorMesh（根，引擎 Cube）、InteractBox（BoxComponent，子于 DoorMesh）；变量 bOpen。
- 逻辑（全在 BP_Door 内，未改角色蓝图）：InteractBox 开始重叠且为 Character → EnableInput(PlayerController 0) + 屏幕提示"Press E to open door"；结束重叠 → DisableInput；按键 E → 若 bOpen=false 则置 true，MoveComponentTo 把门沿 +Z 升 300cm，用时 1 秒。门只能开一次（不会再关）。
- 关卡：Lvl_S04_Collision 新增 Door_01（位置 450,0,150；缩放 0.2×19.5×3，两端嵌入左右墙形成封闭挡板）；已保存。其他关卡未改。
- 注意：根组件的缩放由关卡里放置的 Actor 缩放决定（蓝图里设的缩放被覆盖），所以门的尺寸在关卡实例上设置；触发盒的 BoxExtent 已按父级缩放反向补偿（本地 1000×67×33），换缩放时需同步调整。后续可重构为 DefaultSceneRoot + 子网格。
- Agent PIE 实测：按住 W 走向门，停在 x=405（被门挡住）；3.5s 按 E，门升到 z=450；随后继续前进至 x=739.9（被前墙挡住），门位置读回 (450,0,450)。
- 未验证：屏幕提示文字是否显示（只确认节点编译，未截图）；手柄无交互键；鼠标；Windows。

### S06 固定位置 NPC + 一句对白 — 2026-11-15
- 新增资产：/Game/LOKI/Gameplay/BP_NPC_Test（Actor）。组件：Capsule（根）、NpcMesh（SKM_Manny_Simple，单节点动画 MM_Idle 循环）、TalkBox（BoxComponent，触发范围）；变量 bTalking。
- 逻辑（全在 BP_NPC_Test 内，未改角色蓝图）：进入 TalkBox 且为 Character → EnableInput + 屏幕提示"Press E to talk"；离开 → DisableInput；按 E：未对话则 bTalking=true、SetIgnoreMoveInput(true)、屏幕显示占位字幕（Key=NpcDialog）；再按 E：bTalking=false、SetIgnoreMoveInput(false)、清除字幕。
- 对白为占位："NPC：你好，旅人。（占位对白，非正式剧情）[E 关闭]"，不是正式剧情，未采用附录 A 内容。
- 字幕使用 PrintString 屏幕文字（临时方案），S07 需要两选项对话时再做正式 UMG 对话 UI。
- 关卡：Lvl_S04_Collision 新增 NPC_Test_01（-300,200,90，朝 -Y）；已保存。其他关卡未改。
- Agent PIE 实测：W 走到 x=-364.4 停下；2.0s 按 E 后，2.6–3.6s 按住 W 不移动（位置不变）；4.0s 再按 E，4.6s 起 W 恢复移动（速度 600）。
- 注意：实测前编辑器里有用户遗留的 PIE 会话，已停止后才能编辑蓝图。
- 未验证：字幕在屏幕上的实际显示效果（未截图）；鼠标仍可转视角（只锁了移动）；NPC 动画是否在播放（未截图）；Windows。
- 素材备忘：发现 Fab 库里有 Last Wizard FREE（/Users/Shared/UnrealEngine/Launcher/VaultCache/LastWiza...，含骑士+4动画、城堡房间构件、门、火炬等，文档称 UE 5.8，仅在 Vault 缓存，未导入 LOKI）。待用户决定是否导入并登记 04_ASSET_LICENSES.md。

### S07 两选项对话 + 一个剧情布尔值 — 2026-11-15
- 新增资产：/Game/LOKI/Gameplay/BP_LokiGameInstance（GameInstance，公开变量 StoryFlag_AcceptedHelp:bool，默认 false）。
- 配置修改：Config/DefaultEngine.ini → [/Script/EngineSettings.GameMapsSettings] GameInstanceClass=/Game/LOKI/Gameplay/BP_LokiGameInstance.BP_LokiGameInstance_C（改前备份：Docs/_backup_pre_enginewire/DefaultEngine.ini.pre_S07）。
- BP_Door：新增自定义事件 OpenDoor（若未开则 bOpen=true 并升 300cm/1s），原有"按 E 开门"保留。
- BP_NPC_Test：对话改为提问"要我帮你把门打开吗？（占位对白，非正式剧情）[1]好 [2]不用 [E]关闭"，新增 bAwaitChoice；按 1 → StoryFlag_AcceptedHelp=true、字幕"好，我来帮你开门"、并调用关卡中 BP_Door 的 OpenDoor；按 2 → StoryFlag_AcceptedHelp=false、字幕"那你自己想办法吧"、门不动；E 关闭对话并恢复移动。选择只在提问状态下生效一次。
- 选项界面：仍为 PrintString 屏幕文字 + 键盘 1/2（临时方案，不是 UMG 按钮）。
- Agent PIE 实测（两次独立 PIE）：选 1 → 门读回 (450,0,450)，GameInstance.StoryFlag_AcceptedHelp=true；选 2 → 门读回 (450,0,150) 保持关闭，标记=false（注意 false 同时也是默认值，该分支"写入 false"本身无法与默认区分）。
- 未验证：字幕/选项实际画面；对话中不选直接按 E 关闭（逻辑存在，未单独测）；标记在换关/存档后的保留（本步不做存档）；Windows。
- 对白均为占位，非剧情；未采用附录 A。

### S08 单个守卫两点巡逻 — 2026-11-15
- 新增资产：/Game/LOKI/Gameplay/BP_Guard（Character；骨骼网格 SKM_Quinn_Simple，动画蓝图 ABP_Unarmed，MaxWalkSpeed=200，AutoPossessAI=PlacedInWorldOrSpawned）。变量：PointA、PointB（运行时）、PatrolOffset（公开，默认 0,700,0）、bGoToB。
- 逻辑：BeginPlay 记录当前位置为 A，B=A+PatrolOffset，开始 Patrol 自定义事件循环：取 AIController → 按 bGoToB 用 AIController.MoveToLocation(AcceptanceRadius=50) 走向 B 或 A → 翻转 bGoToB → Delay 5 秒 → 再次 Patrol。（MoveToLocation 非延迟节点，所以用固定 5 秒周期，不是"到达后再回头"；S09 做警觉/追击时需要改成状态机/行为树。）
- 关卡 Lvl_S04_Collision：新增 NavMeshBoundsVolume（NavBounds_S04，位置 0,0,150，半尺寸 1950×950×300）并重建导航网格（自动生成 RecastNavMesh/NavigationData）；新增 Guard_01（-700,-600,92，朝 +Y）。已保存。
- Agent PIE 实测（14 秒，每秒采样）：守卫在 y≈-513 与 y≈+13 之间往返，速度 200cm/s，方向切换时朝向翻转，两端各停约 1 秒，连续 3 个半周期（走完 2 次折返）；x、z 不变，无穿模/卡住迹象。
- 备注：端点比设定点短约 87cm（AcceptanceRadius 50 + 胶囊半径约 34，MoveToLocation 默认 bStopOnOverlap），实际巡逻距离约 530cm 而非 700cm，属预期。
- 未验证：走路动画画面（只确认属性已设置，未截图）；守卫与玩家/NPC/门碰撞时的表现；守卫朝向是否平滑；Windows。
- 注意：测试时用的是 PIE 内读取位置，PIE 已停止。NavMesh 属于关卡内数据，若移动/新增大型障碍需重建。

### S08 返工：守卫滑行（走路动画不播）— 2026-11-15
- 用户反馈：守卫脚不动，在滑行。
- 原因：模板 ABP_Unarmed 的 ShouldMove 由 GetCurrentAcceleration 决定；AI 寻路默认 NavMovementProperties.bUseAccelerationForPaths=false，移动时加速度恒为 0，动画一直待机。（CharacterMovement.bUseAccelerationForPaths 在 5.8 已弃用，改写 NavMovementProperties 下的同名属性。）
- 修复：BP_Guard 类默认值 CharacterMovement.NavMovementProperties.bUseAccelerationForPaths=true（只改这一项，其他逻辑未动）。
- Agent PIE 复测：守卫移动时 CharacterMovement.Acceleration 读数 (0,-2048,0)（返工前恒为 0）；foot_l/foot_r 离地高度随时间交替变化（8.8→17、7.6→13.3），脚在抬起落下。到达端点时加速度回 0。
- 未验证：走路动画的肉眼观感（仍需用户确认脚确实在迈步）；Windows。

### S09 守卫看到玩家 → 警觉/追击 — 2026-11-15
- BP_Guard 新增组件：PawnSensing（PawnSensingComponent：SightRadius 1200，PeripheralVisionAngle 60，SensingInterval 0.2，不听声，仅感知玩家）、AlertMark（TextRender "!"，红色，z+130，默认隐藏）；新增变量 bChasing。
- 逻辑：OnSeePawn → 若尚未追击：bChasing=true、显示 AlertMark、MaxWalkSpeed 200→450、屏幕提示"Guard: Alert! (chasing player)"、AIController.MoveToActor(玩家, 100)。Patrol 事件开头增加 bChasing 闸门：追击中不再执行巡逻（循环自然终止）。
- 当前行为（仅 S09 范围）：一旦发现就一直追，不会丢失目标回到巡逻；追上后停在约 1m 处，无伤害/失败（属 S11/S12）。
- 关卡 Lvl_S04_Collision：新增遮挡墙 Wall_Hide（-850,-250,150，缩放 0.5×6×3，挡住出生点到守卫路线的视线）；重建导航；已保存。
- Agent PIE 实测：①玩家不动留在出生点（被墙遮挡）：11 秒内 bChasing 始终 false，守卫继续巡逻；②玩家按 D 走出遮挡（到 y≈698）：守卫在巡逻到约 y≈-455 时转为追击（bChasing=true、速度 450），追到 (-961,585)，距玩家约 1.1m 停下；AlertMark.bHiddenInGame=false。
- 未验证：警觉标记"!"的实际画面（TextRender 不随镜头旋转，可能从某些角度看不见或反向）；屏幕提示文字画面；视野锥肉眼观感；多次来回遮挡/失去目标行为；Windows。
- 备注：PawnSensing 为较老的组件，S10 幻术诱饵若要改变守卫目标，需在 OnSeePawn 之外再加"目标优先级"逻辑；必要时换 AIPerception。

### S09 补丁：持续追击 + 4 秒丢失目标 — 2026-11-15
- 用户反馈：追到后玩家离开，守卫不再追。原因：只下过一次 MoveToActor，到达即结束。用户确认丢失时间 4 秒。
- BP_Guard 修改：新增变量 TargetPawn、LastSeenTime、LoseTargetSeconds（公开，默认 4）；OnSeePawn 每次先刷新 TargetPawn 与 LastSeenTime（PawnSensing 每 0.2 秒触发），再进原有闸门；新增自定义事件 ChaseLoop：追击中每 0.3 秒对 TargetPawn 重发 MoveToActor(100)；若 当前时间-LastSeenTime > LoseTargetSeconds → StopMovement、bChasing=false、AlertMark 隐藏、速度回 200、屏幕提示"Guard: lost target, back to patrol"。巡逻循环在追击中不再终止，改为每 1 秒轮询，追击结束后约 1 秒内恢复巡逻（避免重复循环）。
- Agent PIE 实测：①被发现后追到玩家身边（约 1.1m 停下）；②玩家 t=10 开始走开，守卫约 1 秒内重新出发并跟到新位置（(-587,826) 离玩家约 1.1m）；③把玩家传送到 (1500,800) 视野外：守卫继续追了约 3 秒，约 t≈4 时 bChasing=false、速度 200、AlertMark 隐藏，随后转向巡逻点（朝 -X 走回 B 点附近）。
- 已知限制：追击期间守卫始终知道玩家的真实位置（即使被遮挡也沿路径追），"丢失"只按"多久没看见"计时；更像潜行的做法是追"最后看到的位置"，未做，待用户决定（可在 S10 前或 S10 中处理）。
- 未验证：肉眼观感；多次反复被发现/丢失；守卫追到玩家后无任何伤害/失败（属 S11/S12）；Windows。

### S09 补丁复验反馈与定位 — 2026-11-15
- 用户复验：追到后会跟着；4 秒没追到会停止，过一会才回去巡逻；放弃后需要主动靠近到很近的距离才会再次追踪。
- Agent 定位（PIE，未改任何文件）：①放弃追击并回到巡逻后，把玩家放到守卫正前方约 600cm，守卫立即重新发现并追击（bChasing 回到 true）——说明放弃后感知组件正常；②推断"靠得很近才发现"的原因是视野设定：PawnSensing 的 PeripheralVisionAngle=60（总视野约 120°，只朝巡逻方向 ±Y），无近距离全向感知，玩家从侧面/身后靠近会落在视野外。③"过一会才回去"来自：放弃后巡逻循环最多 1 秒轮询 + 走回端点的路程，属当前设计。
- 未实测"侧面/背后"精确距离（守卫在动，传送测试难以对齐），上述②为推断。
- 待用户决定：是否加宽视野（如半角 90°）并加近距离全向感知（如 300cm 内且有视线即发现）。

### S09 补丁 2：视野加宽 + 3 米近距离感知 — 2026-11-15
- BP_Guard：PawnSensing.PeripheralVisionAngle 60→90（前方 180°）；新增公开变量 ProximityRadius=300；新增 ProximityLoop（每 0.2 秒检查：玩家距离<ProximityRadius 且未在追击且 AIController.LineOfSightTo(玩家)=true → 调用新增事件 SpotPlayer，刷新 TargetPawn/LastSeenTime 并走原有警觉流程）；BeginPlay 用 Sequence 同时启动巡逻与 ProximityLoop。编译 0 错误。
- Agent PIE 实测（守卫被固定在 (-700,-267..-400)，朝 -Y，玩家被传送）：背后 500cm 时 3 秒内 bChasing 保持 false（视野外 + 超 3 米）；背后约 250cm 时立即转 bChasing=true 并转身冲向玩家（近距离感知生效）。
- 测试说明：多次测试因守卫每 5 秒掉头一次、传送延迟而受干扰，最终采用"固定守卫（临时把 CharMoveComp.MaxWalkSpeed=0，仅 PIE 内）+ 背后不同距离"的结果；PIE 内直接改 PeripheralVisionAngle 不会更新引擎缓存的视角余弦，此路径无效。结果均为 PIE 运行时修改，已随 PIE 结束丢弃，资产未受影响。
- 未验证：肉眼观感；远处侧面的实际发现距离。

### S10 幻术诱饵（Q 键）— 2026-11-15
- 新增资产：/Game/LOKI/Gameplay/BP_Decoy（Actor，根组件 SKM_Quinn_Simple + MM_Idle 循环，无碰撞，InitialLifeSpan=5 秒；公开变量 AttractRadius=1500）。BeginPlay：找到关卡里的 BP_Guard，若有效且距离<AttractRadius 则调用 Guard.DecoyAttract(诱饵位置)。
- BP_Guard 新增：变量 bDistracted；自定义事件 DecoyAttract(Where:Vector)：放弃追击（bChasing=false）、隐藏 AlertMark、bDistracted=true、速度 300、StopMovement 后 MoveToLocation(Where,50)、屏幕提示"Guard: distracted by decoy"、等待 4 秒后 bDistracted=false，若此时未在追击则速度回 200；巡逻循环的"追击中暂停"闸门改为 bChasing OR bDistracted。
- BP_ThirdPersonCharacter（模板角色，仅新增节点，原有逻辑未动）：新增变量 bIllusionReady（默认 true）、IllusionCooldown（公开，默认 8）；Q 键（K2Node_InputKey）→ 若可用：置 false，在角色前方 400cm、脚下高度生成 BP_Decoy，屏幕提示"Illusion: decoy placed (Q, cooldown 8s)"，等待冷却秒数后恢复；冷却中按 Q 提示"Illusion: cooling down"。限制方式：冷却 8 秒（未做次数限制）。
- Agent PIE 实测：t=1.0 按 Q（t=2.5 再按一次测冷却）：t=2 守卫 bDistracted=true、速度 300，转向并走向诱饵；t≈3 停在 (-618,-79)（离诱饵约 84cm，到达半径+胶囊半径）；t≈5 bDistracted=false、速度 200，随后回到巡逻（继续来回走）；整个过程 bChasing 始终 false（诱饵吸引时玩家被墙遮挡）。
- 未验证：诱饵的实际画面；Q 键冷却中的屏幕提示画面；诱饵与守卫追击中（bChasing=true）时的抢占（逻辑已写，未单独测）；诱饵在守卫 1500cm 外的无效情形；Windows。
- 关于 S09 补丁：用户已确认 3 米近距离感知并要求继续，记为通过。

### S10 返工：追击中放诱饵守卫仍追玩家 — 2026-11-15
- 用户反馈：1-4 通过，第 5 项（追击时按 Q）守卫仍然追玩家。
- 原因：玩家仍在守卫视野/3 米范围内，PawnSensing 与 ProximityLoop 每 0.2 秒重新触发"发现玩家"，在 bChasing=false 的瞬间立刻重新进入追击，覆盖了诱饵造成的分心。
- 修复（仅此一处）：BP_Guard 中"发现玩家"闸门条件由 bChasing 改为 bChasing OR bDistracted，即被诱饵吸引的约 4 秒内忽略玩家；SpotPlayer（近距离感知）与 OnSeePawn 共用该闸门，一并生效。编译 0 错误。
- Agent PIE 复测：守卫在 t≈4–8 追击玩家（bChasing=true，速度 450）；t=9 按 Q：bChasing=false、bDistracted=true、速度 300，守卫走向诱饵并停在 (-463,660)；玩家在 (-790,690) 一直处于其视野内但守卫不理；t≈13 bDistracted=false、速度 200，随即再次看到玩家并追击（bChasing=true，t=14）。
- 设计取舍：分心期间守卫对玩家"失明"约 4 秒（含走向诱饵的路程）；分心结束若玩家仍可见会立即重新追击。如需缩短/延长，改 DecoyAttract 里的等待时间（目前 4 秒）。
- 未验证：肉眼观感；Windows。

### S10 返工 2：逃跑中按 Q 守卫原地不动 — 2026-11-15
- 用户反馈：逃跑时按 Q，守卫原地不动；走到他面前后他仍追玩家。
- 复现（PIE）：玩家跑到场地边缘（y≈940，朝 +Y）按 Q，诱饵落在围墙/导航网格之外（约 y≈1340）；守卫 bDistracted=true 但 MoveToLocation 无路径，原地站 4 秒（bChasing=false、位置不变），分心结束后才恢复追击。
- 原因：诱饵生成点不保证在导航网格上，守卫无路可走。
- 修复：BP_Decoy BeginPlay 先用 NavigationSystemV1.K2_ProjectPointToNavigation（QueryExtent 800,800,500）把自己吸附到最近可走点（成功则 SetActorLocation 到投影点，再通知守卫），吸附失败则直接 DestroyActor（本次诱饵作废，守卫不受影响）。编译 0 错误。
- Agent PIE 复测：同样场景守卫被传送到 (-1000,100) 后按 Q：bDistracted=true，速度 300，从 y=100 一路走向 y≈840 追向诱饵；诱饵实际落在 (-1008,931,z=10)（围墙内侧，原本会落在墙外）。
- 备注：诱饵吸附后 z=10（略高于地面 0，导航面高度），肉眼上可能"悬浮"约 10cm；若介意可再微调。冷却时间到了才能再放诱饵，吸附失败时冷却也已开始计时（未回退）。
- 未验证：肉眼观感；Windows。

### S10 改版：残影 + 半透明隐身（用户提议）— 2026-11-15
- 用户提议并要求：按 Q 时在原位留下残影，主角变半透明并往别处逃跑/攻击。采用推荐默认：残影原地站立不动；隐身 4 秒；冷却 8 秒；隐身期间攻击/背刺留给 S11。
- 新增资产：/Game/LOKI/Materials/M_Ghost（Surface/Translucent/Unlit/双面，参数 GhostColor=(0.3,0.8,1.5)、GhostOpacity=0.35；已把 bUsedWithSkeletalMesh=true，否则游戏内会退回默认材质，PIE 日志有 "missing usage flag SkeletalMesh" 警告，修复后警告消失）。
- BP_ThirdPersonCharacter：Q 的生成位置由"前方 400cm"改为"脚下"（Multiply B=0），残影朝向与角色一致（新增 K2_GetActorRotation→MakeTransform.Rotation）；新增变量 bCloaked、CloakSeconds（公开，默认 4）；Q 后用 Sequence 并行：冷却 8 秒链保持不变，另一支：bCloaked=true、Mesh 两个材质槽换为 M_Ghost、Delay(CloakSeconds)、换回 MI_Quinn_01/02、bCloaked=false、提示"Illusion: cloak ended"。屏幕提示改为"Illusion: afterimage left, you are cloaked (Q, cooldown 8s)"。
- BP_Guard：新增（纯函数）GetPlayerPawn→Cast→读取 bCloaked；"发现玩家"闸门条件改为 bChasing OR bDistracted OR 玩家隐身；ChaseLoop 的"丢失目标"条件改为 超时 OR 玩家隐身（追击中玩家隐身则最多 0.3 秒内放弃追击）。
- 残影（BP_Decoy）：沿用既有逻辑（吸附到导航网格、吸引 1500cm 内守卫），外观仍为 Quinn 不透明站立，与半透明的玩家形成对比。
- Agent PIE 实测：守卫追击中（bChasing=true，贴近玩家）按 Q 并按 A 向相反方向跑：0.6 秒内守卫 bChasing=false、bDistracted=true，走向残影；玩家 bCloaked=true、bIllusionReady=false、Mesh 两个材质槽均为 M_Ghost；残影出现在按键时玩家所在位置（吸附后 (-1000,931,z10)），玩家已跑到 y≈438；约 4 秒后 bCloaked=false，材质换回 MI_Quinn_01/02；PIE 日志中不再有材质 usage 警告。
- 未验证：半透明/残影的实际观感（颜色、透明度是否合适）；隐身期间守卫与玩家近距离接触；守卫 1500cm 外时玩家隐身仍可让其放弃追击（逻辑已写，未单独测）；Windows。
- 备注：光影/材质为灰盒级临时美术，S15 后再换正式资源；M_Ghost 颜色/透明度可直接改材质参数。

### S11 简单近战攻击与受击反馈 — 2026-11-15
- BP_ThirdPersonCharacter（仅新增节点）：鼠标左键（K2Node_InputKey LeftMouseButton）→ 若 bCanAttack：置 false；若 bCloaked 则用 BackstabDamage（默认 100）否则 AttackDamage（默认 34）调用 ApplyRadialDamage（原点=角色前方 120cm，半径 180，DamageCauser=自己，bDoFullDamage=true）；屏幕提示"Attack!"；Delay 0.6 后恢复 bCanAttack。新增变量 bCanAttack、AttackDamage、BackstabDamage（后两者公开）。无攻击动画（项目暂无近战动画，沿用待机/走跑），无自伤保护（玩家自己在半径内但没有受伤处理，不会掉血）。
- BP_Guard：新增变量 Health（公开，默认 100）、bDead；Event AnyDamage：若未死亡则 Health -= Damage、屏幕显示当前血量（Key=GuardHP）、并行两支：①受击闪光——两个材质槽换为 M_Ghost 0.15 秒后换回 MI_Quinn_01/02；②若 Health<=0 则死亡流程，否则调用 SpotPlayer（被打后警觉，隐身/分心/追击中被现有闸门拦截，不重复触发）。死亡流程：bDead=true、隐藏 AlertMark、StopMovement、DisableMovement、胶囊 NoCollision、Mesh 开启物理模拟（布娃娃，视觉未验证）、屏幕提示"Guard: defeated"、等待 5 秒后 DestroyActor。
- Agent PIE 实测（每次均把玩家/守卫传送到相距约 130cm，玩家朝向守卫）：①普通攻击 3 次（0.2/1.2/2.2 秒按左键）：Health 100→66→32→-2，第 3 下 bDead=true；死亡 5 秒后关卡中找不到 Guard（已销毁）。②隐身背刺：Q 后 1.0 秒按左键一次：Health 100→0，bDead=true。
- 已知限制：无攻击动画；Health 显示为浮点数字（如 66、-2），未限制最小为 0（击杀后 Health 可能为负，未影响功能）；死亡后守卫被销毁，尸体只保留 5 秒；布娃娃/闪光/血量文字的实际观感未验证；守卫目前不会反击（无敌方伤害，属 S12 之前的暂缓范围）；玩家无生命值（S12 加入失败/重试）；Windows 未验证。
- 备注：被打后守卫的警觉复用 SpotPlayer，所以仍遵守 4 秒丢失、诱饵分心、隐身无视等既有规则。

### S11 返工：死亡碰撞与追击距离 — 2026-11-15
- 用户反馈：守卫死亡时没有明显倒地并穿入地面；追击时会在玩家约 1 米外停住，玩家移动后才继续追。
- BP_Guard 修改：死亡时先将 Mesh 的碰撞配置切为 `Ragdoll`，再启用物理模拟；追击 MoveToActor 接受半径 100→25cm，ChaseLoop 更新间隔 0.3→0.15 秒。
- MCP 编译结果：蓝图已编译并保存，`compilerStatus=UpToDate`，0 个编译错误。MCP 提示一条既有的 `DecoyAttract`/`bChasing` 跨 Delay 读取警告，本次没有改动该逻辑。
- 用户验收：用户确认可以合并到 main（2026-11-15）。Windows 未验证。
- 状态：S11 返工通过；本提交将改动从 `codex` 合入 `main`。S12 暂停，等待用户后续指示。

### 交接记录 — 2026-11-15
- 用户额度用尽，后续由 Codex 接手。已新增 `Docs/05_HANDOFF.md`（交接说明）与根目录 `AGENTS.md`，并更新 `CLAUDE.md` 的 MCP 现状与当前进度。
- 用户对 S11 的最终验收反馈：守卫原先"没有倒地动作、陷入地下、追击时在 1 米外停住"，Codex 已修复（死亡碰撞配置 Ragdoll、追击接受半径 25cm、追击更新 0.15 秒），用户确认现在"自然倒地、有动作"，S11 通过。该修复由 Codex 完成并已提交（commit f5a3db1），我（Claude）未参与也未复测。
- 本次交接只写文档，未改任何 UE 资产与配置。S12 未开始。

### S12 玩家死亡/失败后的重试 — 2026-11-15
- 设计选择（用户选）：失败=玩家有血量、守卫会打人。检查点=关卡开头，重试=重载当前关卡（场景、守卫、门全部重置；剧情标记存在 GameInstance，重载后保留）。
- BP_ThirdPersonCharacter（仅新增节点）：变量 Health（公开，默认 100）、bPlayerDead；Event AnyDamage：若 DamageCauser==自己则忽略（防止自己的 ApplyRadialDamage 伤到自己）；若已死亡忽略；Health-=Damage，屏幕显示"Player HP: x"（Key=PlayerHP）；Health<=0 → bPlayerDead=true、DisableInput、提示"You were caught - restarting in 2s..."、Delay 2、GetCurrentLevelName(去前缀)→OpenLevel。
- BP_Guard：变量 GuardDamage（公开，默认 34）、bAtkCD；自定义事件 GuardAttack：bAtkCD=true、ApplyDamage(TargetPawn, GuardDamage, 自己控制器, 自己)、提示"Guard attacks!"、Delay 1、bAtkCD=false。ChaseLoop 在 MoveToActor 后增加判断：距 TargetPawn<160cm 且未在攻击冷却且守卫未死亡 → 调用 GuardAttack。
- Agent PIE 实测：①玩家按左键攻击后 Health 仍为 100（无自伤）。②玩家与守卫相距约 130cm：Health 100→66→32→-2，bPlayerDead=true，约 4.5 秒时 levelChanged（重载）。③重载后玩家 Health=100、bPlayerDead=false；Guard_01 Health=100、bDead=false，回到巡逻点并开始巡逻。
- 已知限制：重试是整关重载，没有"中途检查点"（场景内无多个检查点）；PIE 日志有一条重载时的 "LogCrowdFollowing: Unable to find RecastNavMesh" 警告，重载后守卫巡逻正常，未见影响；守卫攻击无动画/音效；玩家无受击反馈（仅血量文字）；死亡无倒地动画；玩家的隐身/残影状态在重载时自然重置；守卫之间的状态与门状态也会重置；GameInstance 的 StoryFlag 保留（重载后仍为上次的值，是否应该重置待设计）。Windows 未验证。

### C++ 热编译实测（Mac，UE 5.8.4）— 2026-11-15
- 目的：验证"用 C++ 而不是 MCP 蓝图写逻辑"在本机的迭代速度（教程说法：要退出重编译 vs 改动快、省 token）。
- 新增（仅测试用，可删）：`Source/LOKI/LokiCompileTest.h/.cpp`（UActorComponent，含 `GetTestValue()` UFUNCTION）。LOKI 模块此前从未编译过（无 Binaries），本次首次生成 `Binaries/Mac/libUnrealEditor-LOKI.dylib`。
- 结果（编辑器全程开着，没有退出）：
  1. MCP `system_control.create_cpp_class`：首次编译 + 热加载约 35 秒，成功（日志 `LogHotReload: Recompiling module LOKI...`）。
  2. 手改文件后，编辑器**不会自动发现**；`LiveCoding`/`LiveCoding.Compile` 控制台命令在本机**不存在**（Mac 上没有 Live Coding，用的是经典 Hot Reload）。
  3. 控制台 `Module Recompile LOKI`（MCP `system_control.console_command` 可执行）：改函数体 + 重新加载约 8 秒；新增 UFUNCTION 后类的 functionCount 28→29，说明新函数已加载进运行中的编辑器，无需重启。
  4. 命令行外部构建 `Engine/Build/BatchFiles/Mac/Build.sh LOKIEditor Mac Development -Project="<绝对路径>/LOKI.uproject"`：增量约 9.6 秒，编辑器开着也能成功，但编辑器不会因此自动加载（需再执行 `Module Reload LOKI`）。
- 注意：MCP 的 `system_control.run_build kind=ubt` 把工程路径拼成相对路径，会报 "Unable to find project file"，目前不可用，需用命令行或 `Module Recompile`。`create_cpp_class` 不能用子文件夹（模块没有 Public/Private）。构建时 UBT 会清理 EngineWire 的 UnrealEditor.modules 并重新生成，实测文件仍在。
- 未验证：①新增/修改头文件（改 UPROPERTY、类布局）在已有蓝图实例上的热重载稳定性（Hot Reload 对布局变化可能要重启编辑器）；②Windows（Live Coding 在 Windows 可用，需到 Windows 实测）；③从蓝图迁移到 C++ 的实际 token 节省（本次未做迁移，仅测编译链路）。
- 结论：Mac 上"改 C++ → 控制台 Module Recompile → 立刻在编辑器里生效"可行，日常约 8–10 秒，不必退出编辑器；教程所说"必须退出重编译"在本机不成立，但"没有 Live Coding"成立。

### S13 短 Sequencer 实时镜头并返回控制 — 2026-11-15
- 路线判断：MCP（Sequencer 资产、摄像机、关卡触发器都是编辑器内容）+ 小型蓝图触发逻辑。
- 新增资产：`/Game/LOKI/Cinematics/SEQ_S13_Intro`（Level Sequence，24fps，120 帧=5 秒；CineCameraActor `CineCam_S13` 作为可拥有绑定；Transform 关键帧：第 0 帧 (-700,-350,300)→第 119 帧 (-250,-250,230)，全程 lookAt 门 (450,0,150)，线性；一条 Camera Cut 轨道指向该摄像机）。`/Game/LOKI/Gameplay/BP_CutsceneTrigger`（Actor，根 BoxComponent TriggerBox 150×150×100；变量 bPlayed）。
- BP_CutsceneTrigger 逻辑：玩家（BP_ThirdPersonCharacter）首次进入 → bPlayed=true → 玩家 bInvulnerable=true → PlayerController.SetCinematicMode(true, 影响移动与转向) → CreateLevelSequencePlayer(SEQ_S13_Intro) → 绑定 OnFinished → Play；序列结束 → 玩家 bInvulnerable=false → SetCinematicMode(false)，序列播放器自动把视角还给玩家。只触发一次。
- BP_ThirdPersonCharacter 修改（仅新增）：变量 bInvulnerable；AnyDamage 的忽略条件由"伤害来源是自己"改为"来源是自己 OR bInvulnerable"（镜头期间守卫打不到玩家）。
- 关卡：`Lvl_S04_Collision` 新增 `CineCam_S13`、`CutsceneTrigger_01`（-600,300,100）；已保存。
- Agent PIE 实测（守卫被传送到远处）：玩家被传送进触发范围后，约 5 秒内 bInvulnerable=true，按住 W 位置不变；约 5 秒时 bInvulnerable=false；之后 W 正常移动（速度 600）。镜头期间 PlayerCameraManager.ViewTarget.Target = CineCameraActor_0，POV 位置约 (-670,-343,295)、Pitch -7、Yaw 17，与第一帧一致。
- 测试中发现并修复：第一次测试镜头期间守卫贴身把玩家打死触发重载（S12 引入的失败规则与锁操作冲突），因此加了镜头期间无敌。镜头结束后若守卫在旁边，玩家仍会正常受伤。
- 已知限制：镜头只有一个静态的推镜，没有对话/角色动作/音效；镜头期间守卫、NPC 仍在正常行动（没有暂停世界）；触发只一次，死亡重载后关卡重置会再次触发；镜头画面观感（构图、速度）未验证；Windows 未验证。
- 未验证：镜头画面是否好看（需用户肉眼）；序列结束时从镜头切回玩家视角是否平滑（数据上 ViewTarget 切换由序列播放器自动完成，未单独读取结束后的 ViewTarget）。

### S14 把已通过元素串成灰盒完整流程 — 2026-11-15
- 路线判断：MCP（摆关卡/复用蓝图/Sequencer/导航/PIE 验证）+ 两个很小的新蓝图。
- 新增关卡 `/Game/LOKI/Levels/Lvl_S14_Greybox`（独立新关卡，不改 S04 测试关卡；默认地图仍未改，需手动打开）。线性路线（沿 +X）：牢房（x -1225…200，PlayerStart(-1000,0)，NPC_01(-500,-350)，Hint_Start，Cutscene_Intro 触发区(-650,0)，CineCam_S14）→ Door_01(200,0)（宽 600，缩放 0.2×6×3）→ 守卫通道（x 200…2200，宽 600，两侧墙 Corridor_N/S；Block_1(900,-175)、Block_2(1500,175) 交错挡块，中线 y∈(-50,50) 可直通，也可 S 形绕行；Guard_01 起点(700,0)，PatrolOffset=(1200,0,0) 沿通道巡逻）→ 出口房（x 2200…3200，Goal_Exit 触发区(2800,0)，缩放 1×4×1）。地板/墙均为 Cube 灰盒，NavBounds_S14 覆盖全区，导航已重建，已保存。
- 新增资产：`BP_MessageTrigger`（进入后一次性屏幕提示；公开变量 Message、bShowTime（附带本局时间秒数）、Duration、bShowOnBeginPlay（开局 1 秒后自动显示，因玩家出生即在触发盒内不会产生"进入"事件））；`SEQ_S14_Intro`（5 秒推镜，CineCam_S14 从牢房左下推近看门）。`BP_CutsceneTrigger` 增加公开变量 `Sequence`（默认仍为 S13 的序列，S14 实例指向 SEQ_S14_Intro）。
- 流程（预期时长，真人 2–3 分钟取决于潜行/战斗选择）：出生→开局提示→走进触发区播 5 秒镜头→找 NPC 按 E 提问→按 1 开门/按 2 或 E 自己开门→进守卫通道（潜行/Q 残影隐身/左键战斗/背刺）→到出口出现"DEMO COMPLETE! | Time(s): xx"。失败（血量归零）→2 秒后整关重载（剧情标记保留）。
- Agent PIE 实测（分段，Agent 自测）：①开场镜头：进入触发区 bInvulnerable=true、约 5 秒内移动被锁，之后恢复；②NPC：走进对话范围按 E、按 1 → StoryFlag_AcceptedHelp=true，Door_01 升到 z=450（门开）；③隐身通道：Q 后冲刺 bCloaked=true（脚本直线走时被站在中线的守卫身体挡住，隐身 4 秒结束后被守卫打到 -2 → 整关重载，符合失败规则）；④出口：玩家到 x≈3165，Goal_Exit.bFired=true；⑤开场提示：Hint_Start.bFired=true（修复前为 false）。
- 未在一次连续脚本中完整通关：通道里守卫挡中线、脚本无法精确控制横向，故通道段与出口段分开验证；真人可绕守卫或用战斗/背刺通过——该部分的真实通过性与体验需用户亲自试玩确认。
- 已知限制：通道仅 600 宽，守卫巡逻线与最短路线重合，玩家需绕开/残影/战斗；没有"同伴协作""熔岩出口"（总设计 Demo 路径中的同伴和熔岩尚未实现，S14 只串联已通过元素）；没有中途检查点（死亡整关重载）；镜头/提示/对白都是临时屏幕文字；全灰盒；Windows 未验证；整套未经真人完整试玩。

### S14 补充：检测范围调试可视化（F1 开关）— 2026-11-15
- 用户提议：给敌人视野和替身吸引范围加可见的检测范围，便于验收。路线：蓝图（DrawDebug 节点，发布版自动不绘制）。
- BP_LokiGameInstance：新增公开变量 `bShowDebug`（默认 true）。BP_ThirdPersonCharacter：F1 键切换 bShowDebug 并屏幕提示"Debug draw (F1): true/false"。
- BP_Guard：新增 `DebugLoop`（BeginPlay 的 Sequence 第 3 路启动，每 0.05 秒循环）：若 bShowDebug 则绘制 ①黄色视野锥（DrawDebugConeInDegrees，长度 1200=SightRadius，半角 90°=PeripheralVisionAngle，常量与 PawnSensing 当前设置一致，改感知参数时需同步改这里）②青色球 = ProximityRadius（300cm 近距离感知圈）③红色球 160cm = 守卫攻击范围 ④头顶状态文字：绿 PATROL / 红 CHASE / 蓝 DISTRACTED ⑤玩家距离<1200cm 时从守卫到玩家的白色连线。
- BP_Decoy：新增 Tick 绘制：紫色球 = AttractRadius（1500cm 吸引范围）+ 文字 "DECOY (attracts guards in magenta radius)"。
- 实现中把已废弃的 DrawDebugCone（弧度）换成 DrawDebugConeInDegrees（度），并删除旧节点。
- Agent PIE 验证：放残影、开关 F1 期间日志无蓝图警告/错误；F1 单按一次 bShowDebug true→false，再按恢复。调试线条/文字的实际画面未验证（需用户肉眼看）。
- 未包含：NPC 对话范围、门交互范围、镜头/消息触发区的可视化（它们是 Box 组件，可另做）；视野锥是简化的半球锥，未画遮挡（被墙挡住的视线仍会被画在锥内，真实判定会被墙挡）。

### S15 起点：导入 Last Wizard 的门并替换 S14 的"墙门" — 2026-11-15
- 路线判断：MCP（导入素材、网格/组件替换、关卡摆放、PIE 验证）。用户确认采用推荐方案（先换门）；Git 先提交（5512df2，仅本地、未推送 GitHub）。
- 导入：MCP 的 `maintain_content migrate` 找不到该 Fab 包目录（fabLibrary 根目录被判为 missing），改为直接把需要的文件复制进工程并保持包内路径：`Content/LastWizard/ModularInterior/Door/**`（门板、门框、两个把手、MI_Door、3 张贴图、BP_Door_Free）、`Content/LastWizard/Base/**`（M_RSG_Lit 等基础材质与贴图）、包内说明文档。共约 13MB；编辑器已识别（asset.list 可见）。只导入"门"，没有导入整包（186MB）。
- 模型尺寸：门板 214×383×31cm，门框 311×437×37cm，枢轴在地面中心。因关卡墙高 300cm，整体等比 0.68（门板约 146×260，门框约 212×297）。
- 新增 `BP_Door_LW`（`BP_Door` 的子类，保留 `OpenDoor`/按 E 开门逻辑；DoorMesh 改用 SM_Door_SM_Door，InteractBox 的 BoxExtent=(294,441,147)、相对位置 z=147，按等比缩放后世界半尺寸≈(200,300,100)；新增 HandleFront/HandleBack 把手，跟着门板一起上升）。父类 `BP_Door` 及 S04 测试关卡的旧门未改（已读回父类 BoxExtent/Mesh/Location 不变）。门框不是蓝图一部分（否则会跟着升起），作为独立静态网格 `DoorFrame_01` 放在关卡里。
- S14 关卡改动：删除旧 `Door_01`（方块门板，我自己创建的），新建 `Door_01`（BP_Door_LW，位置 (200,0,0)，缩放 0.68）、`DoorFrame_01`、两侧补墙 `DoorWall_N/S`（(200,±203,150)，0.5×1.94×3，填满门两侧至通道宽 600）；导航已重建；关卡已保存。
- Agent PIE 实测：①门关着，玩家 W 前进在 x≈154 被挡；②走到 NPC 旁按 E、按 1：Door_01 位置读回 (200,0,300)（门升起 300cm）；③门开后玩家从 (-100,0) 前进，经 x=-4、326 到 939（通过门洞）。
- 未验证：门的实际外观（材质、比例、风格是否与灰盒协调）；开门时门板升起的观感；门框与墙的接缝；把手位置；整关其他部分仍是灰盒方块，风格不统一属预期。
- 授权：已创建 `Docs/04_ASSET_LICENSES.md` 并登记，**许可条款待用户在 Fab 商品页核对**。

### S15：守卫换成 Last Wizard 骑士 — 2026-11-15
- 路线判断：MCP（导入素材、改网格/动画、PIE 实测）；动画不做动画蓝图，直接在 BP_Guard 里用 SkeletalMeshComponent.PlayAnimation（单节点模式）按状态切换，因为包内只有 4 个动画片段、无动画蓝图，逻辑简单、好验证。
- 导入：把 `Characters/Knight` 复制到 `Content/LastWizard/Characters/Knight`（保持包内路径；约 46MB，其中贴图 42MB）。已在 `Docs/04_ASSET_LICENSES.md` 追加登记，许可仍待用户核对。
- BP_Guard 修改：Mesh 改为 SK_Knight、AnimationMode=AnimationSingleNode、初始动画 Idle 循环、缩放 0.65（骑士原高约 272cm，缩到约 177cm 匹配胶囊）；沿用原来的 Z=-90、Yaw=-90（用 foot_l→ball_l 的方向实测：脚尖朝向与守卫前进方向一致，朝向正确）。新增变量 CurAnim（0/1待机/2行走）、bAttackAnim；新增事件 AnimLoop（每 0.1 秒，BeginPlay 的 Sequence 第 4 路启动）：未死亡且不在攻击时，速度>20 播 Walk（循环），否则播 Idle（循环），仅在状态变化时切换。GuardAttack：开始时 bAttackAnim=true、播 Attack（不循环）、PlayRate=2（让 2.17 秒的动画约 1.1 秒播完，匹配 1 秒攻击冷却），冷却结束后 bAttackAnim=false、CurAnim=0、PlayRate=1。死亡流程：把"开启物理模拟（布娃娃）"改为 SetSimulatePhysics(false) + 播 Death（不循环），其余（停止、DisableMovement、胶囊 NoCollision、5 秒后销毁）不变。受击闪光后恢复的材质由人偶的 MI_Quinn 改为骑士的 MI_Knight_Top / MI_Knight（否则闪光后会变成人偶材质）。
- Agent PIE 实测：①巡逻：CurAnim 在行走(2)与停顿处待机(1)间切换；foot_l 高度在 12–45cm 来回变化（走路动画在播）；②追击贴近玩家：bAttackAnim=true，玩家血量按 34 递减；③被玩家 3 下打倒：bDead=true；约 1.3 秒后头部高度从约 102 降到约 65（死亡动画在播），其后保持；5 秒后守卫销毁（actorDestroyed）。
- 未验证（需用户肉眼）：骑士的实际外观、比例、材质是否正常；走路动画在 200 速度下是否滑步、追击速度 450 时是否明显滑步（走路动画只有一种速度）；攻击动画与伤害时机是否对得上（伤害在挥击开始瞬间结算）；死亡动画最终姿势；受击闪光后材质是否复原；骑士武器位置。
- 已知限制：动画切换最多延迟约 0.1 秒；攻击连续出手时攻击动画几乎一直在播；PlayRate 2 倍可能显得急；包内无 Run，追击没有跑步动画；骑士是单独的外观替换，守卫的碰撞胶囊没改。Windows 未验证。

### S15 返工：骑士没换成功 + 调试线闪烁/过于夸张 — 2026-11-15
- 用户反馈：模型没换成功，还是原模型，且变小、走路变成姿势很奇怪的滑行；侦测范围线一直闪烁，不需要那么夸张，一个扇形就够了。
- 原因（我上一轮的错误）：我只改了 BP_Guard 的类默认值，但关卡里放置的 Guard_01 实例上保存过"网格=SKM_Quinn_Simple"的覆盖值，类默认值改动不会覆盖实例的覆盖值；缩放和动画模式没有被覆盖所以跟着变了——结果人偶被缩小并套上了骑士的动画，姿势错误。我上一轮的"骨骼名/脚踝高度"测试读到的是人偶骨骼（spine_05、weapon_r_muzzle 等），当时没有核对 SkeletalMeshAsset，结论不可靠，已作废。
- 修复：用 inspect.set_component_property 直接把 Lvl_S14_Greybox 里 Guard_01 的 CharacterMesh0.SkeletalMeshAsset 改为 SK_Knight（材质读回为 MI_Knight_Top/MI_Knight/MI_Knight_Weapon）；BP_Guard 的 Mesh.AnimClass 清空（单节点动画不需要 ABP_Unarmed）；关卡已保存。
- 调试线：BP_Guard 只保留"贴地的扇形"（DrawDebugConeInDegrees，AngleHeight=1° 使其很扁；半角 90°、长 1200，起点降到脚下 80cm，线宽 1）和头顶状态文字；删除了 3 米球、攻击圈球、到玩家的连线及其节点。BP_Decoy 的吸引范围由球改为贴地的紫色圆圈（DrawDebugCircle）。线条/文字存活时间由 0.05–0.06 秒改为 0.15 秒（循环间隔 0.05 秒），消除闪烁（原因：存活时间比刷新间隔短）。
- 这次实测（以运行中实例为准）：Guard_01.Mesh.SkeletalMeshAsset=SK_Knight、AnimationMode=AnimationSingleNode、AnimClass=None；骨骼列表为骑士的（spine_01…04、neck、socket_l/r、weapon_base、chain_01…03、weapon_head）；巡逻中 CurAnim 在 2（行走）/1（待机）间切换；左右肩位置与朝向一致（朝 -X 时左肩在 +Y）。
- 仍未验证（需用户肉眼）：骑士外观与动画观感、扇形的实际画面、闪烁是否消失、攻击/死亡动画。
- 遗留：S04 测试关卡（Lvl_S04_Collision）里的 Guard_01 实例也保存过人偶网格覆盖，但套用了骑士动画，可能显示不对；该关卡是旧测试关，未处理，需要时再修。

### S15 微调：扇形缩小、守卫降速与脚步匹配、开场镜头空格跳过 — 2026-11-15
- 用户反馈：S15 通过；脚下扇形太大；守卫移动太快、与脚下步伐不自然；开场摄像机对门的动画希望可以空格跳过。
- 路线判断：蓝图（参数与少量节点）。
- 扇形：PawnSensing.SightRadius 由 1200 缩到 800（真实视距同步缩小，降低潜行难度）；BP_Guard 调试扇形的长度改为直接读 PawnSensing.SightRadius，扇形与真实判定始终一致。注意：视距改小是玩法改动，如觉得 800 太近可在 BP_Guard 的 PawnSensing 组件里调。
- 速度：守卫巡逻/默认 200→120，被残影吸引 300→200，追击 450→300（共 4 处节点默认值 + 蓝图类默认 MaxWalkSpeed=120，已读回）。行走动画的播放速率每 0.1 秒按"当前速度 / WalkRefSpeed"调整（新增公开变量 WalkRefSpeed=120，对应播放速率 1.0；觉得脚步还滑可调这个值，数值越小动画越快）。
- 开场镜头跳过：BP_CutsceneTrigger 新增变量 SeqPlayer、bPlaying；镜头开始后给触发器开启输入，屏幕提示"Press SPACE to skip"；按空格 → 停止序列 → 走与"自然播完"同一条结束流程（无敌解除、取消 CinematicMode、SetViewTargetWithBlend 回玩家 0.3 秒、关闭触发器输入）。发现并修复副作用：镜头期间按空格会让角色跳起（跳跃输入没被 CinematicMode 屏蔽），现改为镜头期间 DisableInput 玩家角色本身（同时也屏蔽了镜头期间的 Q/E/左键），结束后 EnableInput 恢复。
- Agent PIE 实测：①守卫巡逻速度读回 120、SightRadius 读回 800、CurAnim=2；②镜头 t=1 秒按空格：bInvulnerable 立刻变 false、角色未跳起（z 保持 92）、ViewTarget 回到玩家角色；随后 W 正常前进、再按空格能跳起（z 升至 205）——输入已恢复。
- 未验证：脚步与速度是否看着自然（WalkRefSpeed=120 是估计值，需用户肉眼调）；追击速度 300 时走路动画被加速到约 2.5 倍是否难看；扇形缩小后的画面；镜头跳过的观感；Windows。

### S15 换墙地板：Last Wizard 模块替换灰盒 — 2026-11-15
- 路线判断：MCP（摆放/材质/导航/关卡内容，C++ 做不到）。
- 导入：把 Floor、Wall 两个目录从 Fab 缓存复制到 `Content/LastWizard/ModularInterior/`（路径保持原样）；`MI_Wall_Bricks_Plain`、`MI_Floor_Rocks_A` 设 two_sided=true（素材是单面平面）。
- 关卡 `Lvl_S14_Greybox`：新增共 97 个 `LW_*` 静态网格（tag LW_Visual，文件夹 LW_Visuals）：LW_Floor×27（500cm 块，z=0.5）、外墙/走廊/房间/挡块/门墙立面共 70 块（统一缩放 0.6=300cm 高）。全部 `NoCollision`。
- 原灰盒 11 个方块（Floor、Wall_Outer_N/S/W/E、Corridor_N/S、Block_1/2、DoorWall_N/S）仅把 StaticMeshComponent0.bVisible 设为 false，**碰撞保留**，所以导航/阻挡与原来完全一致。重建导航并保存关卡。
- Agent PIE 实测：守卫巡逻 x 785↔1339 来回、速度 120、z=90.2，导航正常；玩家按 D 4.5 秒在 y=589.9 被北侧墙挡住（与换前同一位置）。
- 未验证（需用户肉眼）：墙面/地板外观与接缝、墙面是否有缝隙或穿帮、门附近墙的衔接、光照下是否过暗；Last Wizard 许可仍待核对。

### C1 ULokiHealthComponent（C++ 血量/受伤/死亡）— 2026-11-15（Claude Code，Opus 5.5）
- 路线判断：混合。血量是多处复用的系统逻辑 → C++（规则 D-2）；挂组件、改蓝图连线、PIE 验证 → MCP。
- 新增 C++：`Source/LOKI/LokiHealthComponent.h/.cpp`（UActorComponent，显示名 "Loki Health"）。属性 MaxHealth(100)、bInvulnerable、bIgnoreSelfDamage(true)；事件 OnHealthChanged(NewHealth, Delta, DamageCauser)、OnDeath(Killer)；函数 GetHealth、GetHealthPercent、IsDead、ApplyHealthDamage、Heal、ResetHealth。BeginPlay 时自动绑定拥有者的 OnTakeAnyDamage，所以守卫原有的 ApplyDamage 不用改。编辑器内 `Module Recompile LOKI` 编译通过（UBT Result: Succeeded，新文件 0 警告），热加载成功。
- BP_ThirdPersonCharacter：新增组件 HealthComp；BeginPlay 绑定 OnHealthChanged → 自定义事件 OnPlayerHealthChanged（屏幕显示 "Player HP: x"，Key=PlayerHP，与原来相同）；OnDeath → OnPlayerDeath（DisableInput → 提示 "You were caught - restarting in 2s..." → Delay 2 → 重载当前关卡，与原来相同）。删除旧的 Event AnyDamage 整条链（24 个节点）和变量 Health、bPlayerDead、bInvulnerable。lint 0 条。
- BP_CutsceneTrigger：镜头开始/结束时原来设置角色的 bInvulnerable，改为设置 `HealthComp.bInvulnerable`（true/false），其余不变。lint 只有 1 条原有提示（SeqPlayer 跳过路径未判空，非本步引入）。
- 唯一有意的行为差异：血量最低限制为 0（原来会显示 -2）。
- Agent PIE 对比（Lvl_S14_Greybox，玩家传送到守卫前方 870,0）：迁移前 Health 100→66→32→-2，bPlayerDead=true，4.2 秒 levelChanged；迁移后 100→66→32→0，bDead=true，4.2 秒 levelChanged（时间点一致）。重载后 Health=100、bDead=false。左键攻击后 Health 仍 100（不自伤，但未单独证明攻击确实发出）。走进开场镜头触发器：bInvulnerable 立刻 true，镜头中按空格跳过后变 false。
- 未验证：肉眼观感（应与之前完全一样）；热重载后若出现异常需重启编辑器复测；Windows 编译。
- 过程备注：本会话开在 Claude 的 worktree 里，用户批准用命令行直接改主目录 main 的文件。EngineWire 的 write_graph_from_code 在中文编辑器下 `(if ...)`、`(not ...)` 会被译成英文节点名而失败，复杂改动改用 edit_graph batch。

### C1 返工：闪黑、"4 下才死"、无敌可见 — 2026-11-15（Claude Code，Opus 5.5）
- 用户反馈：①每次被击中画面闪黑；②被攻击 4 次才死；③开场镜头无敌无法判断（守卫太远）。
- ①原因（S15 换骑士后就有，非 C1 引入）：骑士网格用 CharacterMesh 碰撞预设，会挡 Camera 通道；守卫挥剑时弹簧臂把镜头拉到玩家身体里 1 帧。PIE 逐帧采样证据：镜头 x 从 429 跳到 829（玩家位置）。修复：BP_Guard 的 CharacterMesh0 碰撞响应 Camera=Ignore（只影响镜头，不影响攻击/布娃娃）。复测：4.4 秒共 266 帧，镜头位置恒定 470（min=max）。
- ②原因：日志显示实际第 3 下就死（66→32→0），但死后 2 秒等待中守卫还会再挥一刀（S12 起就有）。修复：C++ 新增 `ULokiHealthComponent::IsActorDead(Actor)`（静态、蓝图可用）；BP_Guard 的 GuardAttack 开头加 Branch：目标已死则不攻击。复测：日志只有 3 次 "Guard attacks!"，死后无攻击。
- ③新增调试显示：组件 `SetDebugDisplay(bool)`，开启时每帧屏幕显示 "[角色名] HP x/100 | Invulnerable: YES/NO"（无敌黄字，否则绿字；只在开启时 Tick）。BP_ThirdPersonCharacter：F1 切换 bShowDebug 后同步调用；BeginPlay 按 GameInstance.bShowDebug 同步（重载后保持）。复测：F1 后 IsComponentTickEnabled=true；走进开场镜头 bInvulnerable=true。截图工具抓不到屏幕调试文字，文字外观需用户肉眼确认。
- 热重载问题：第二次改头文件后蓝图连线报"Loki Health 与 Loki Health 不兼容"（新旧类并存）。处理：用户关闭编辑器 → 命令行 Build.sh 编译（Succeeded）→ 重开编辑器 → 再接线成功。结论：改 C++ 头文件（新增函数/成员）后需要接蓝图时，先重启编辑器。
- 发现：PIE 里按 F1 同时会触发 UE 自带的"线框模式"（F1 线框 / F3 恢复光照），这是编辑器调试快捷键，打包后的游戏不会有。S14 选了 F1 做调试开关，暂未改键。

### 设计对照检查与用户决定 — 2026-11-16（Claude Code，Opus 5.5）
- 对照总设计文档做了只读检查（未改资产）。发现：STATE 里的"室外断崖开场（主角被绑在石头上）"与设计文档冲突（文档：序章从牢房开始；绑石/蛇毒是巴德尔死后的原典刑罚）。
- 用户决定：
  1. 序章开场暂定按设计文档的**牢房**方案，忽略断崖方案（`Lvl_S16_Cliff` 计划作废）。
  2. 同伴、字幕/对话 UMG 尚未做；分段检查点是未来功能。
  3. Git LFS：批准给**新增**资产启用（不改历史）。本机未安装 git-lfs，本次未启用，留给下一位 Agent（安装命令需用户同意）。
  4. 本地未推送的提交：推送到 GitHub（本次已推送）。
  5. Last Wizard FREE 许可：用户确认没有禁止商用的条款。
  6. S15 换墙地板：确认通过，测试场景不再打磨。
  7. F1 与编辑器线框快捷键冲突：忽略。守卫 DecoyAttract 旧警告：C2 顺带解决。Windows 验证：暂时忽略。启动日志杂项、默认地图、LokiCompileTest：忽略。
  8. 路线：A 段（推送、LFS、确认验收）本次处理；B 段 C2→C5（守卫 AI 迁 C++、分段检查点、同伴基础、对话/字幕 UMG）后续做；C/D 段（序章灰盒、写实资产、Windows、正式 Demo）以后再说。
- 工作区里 `Lvl_S14_Greybox.umap`、`SEQ_S14_Intro.uasset` 是用户关闭编辑器时保存的自己的改动，按用户"只提交必要的"未提交。
