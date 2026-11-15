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
