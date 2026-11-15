# 素材来源与授权登记

> 规则：每引入一个外部素材，必须登记来源、许可、是否允许商用与分发条件。**Agent 不能代替用户核对许可条款**——"许可（待核对）"需用户在素材页面确认后改成确定结论。

| 素材 | 来源 | 导入日期 | 工程内路径 | 用途 | 许可 | 商用 | 备注 |
|---|---|---|---|---|---|---|---|
| Last Wizard FREE – Stylized Castle Room with Rigged Knight and Fireplace（仅导入"门"相关部分） | Fab 免费 listing（用户账号领取；本机缓存目录 `/Users/Shared/UnrealEngine/Launcher/VaultCache/LastWiza…`，包内文档称支持 UE 5.8） | 2026-11-15 | `/Game/LastWizard/ModularInterior/Door/**`、`/Game/LastWizard/Base/**`（M_RSG_Lit、M_RSG_ParticleSprite、M_LastWizard_Free_OverviewGround、T_RSG_Default_*）、`/Game/LastWizard/LastWizard_Free_Documentation.md` | S15：替换 S14 关卡的门（`BP_Door_LW` 使用 `SM_Door_SM_Door` + 把手；`DoorFrame_01` 使用 `SM_Door_SM_DoorFrame`） | **待核对**（须用户在 Fab 商品页确认许可类型与条款；包内文档未写许可） | **待核对** | 仅导入约 13MB；其余（城堡房间构件、骑士、火把、桌子等）尚未导入，仍在 Fab 缓存中；整包约 186MB。 |

| Last Wizard FREE（骑士部分，同一素材包） | 同上 | 2026-11-15 | `/Game/LastWizard/Characters/Knight/**`（SK_Knight、SKEL_Knight、PHYS_Knight、4 个动画 A_Knight_Idle/Walk/Attack/Death、3 个材质、9 张贴图，约 46MB） | S15：替换守卫 `BP_Guard` 的外观与动画 | **待核对**（同上） | **待核对** | 骨架为包内自带自定义骨架（骨骼命名类似 UE5 Mannequin），无 Run/Jump。 |

## 备注
- 本机 Fab 缓存里还看到另外几个 listing：`Electric_Dreams_Env`、`Game_Animation_Sample`、`Valley_of_the_Ancient`（均未导入，只是缓存目录名）。使用前同样需要用户确认来源与许可，并在此登记。
- 保持 `/Game/LastWizard/...` 原路径不变（包内资产以绝对路径互相引用，搬家会断引用）。
- 最终发行版若换成付费高清素材，同样在此登记，并确认可打包到 Windows 商业发行。
