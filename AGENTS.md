# AGENTS.md — LOKI（给 Codex 等 AI Agent）

你是《LOKI / 洛基》UE5 项目的执行 Agent。用户是导演/验收者，几乎不懂 UE；用**中文、简洁**，直接给结果。

**开始任何工作前按顺序阅读：**
1. `Docs/05_HANDOFF.md` — 当前状态、资产清单、键位、MCP 经验、已知问题（交接说明，最重要）
2. `CLAUDE.md` — 项目事实与工作规则
3. `Docs/LOKI_UE5_Claude_MCP_项目总设计与分步验收_v1.0.md` — 10 条规则、S00–S15 路线图、附录 A 剧情（只读，只能提案）
4. `Docs/01_DEV_LOG.md`、`Docs/02_ACCEPTANCE_LOG.md`

**核心规则：** 一次只做一个最小步骤，做完停下等用户明确说"通过，继续"；Git 提交需用户批准；不自行编造剧情；Agent 自测不等于用户验收。

**当前状态：** 以 `Docs/STATE.md` 为准（不要依赖本文件里的进度）。

**多 Agent 协作（必须遵守）：** 新会话先运行 `bash Tools/session_start.sh` 并读 `Docs/STATE.md`、`Docs/06_CROSS_AGENT_WORKFLOW.md`；同一时刻只让一个 Agent 动编辑器/资产；每个小步骤结束要更新 `01_DEV_LOG.md`、`02_ACCEPTANCE_LOG.md` 并**覆盖更新 `Docs/STATE.md`**；每步开始前先判断实现路线（C++ / MCP / 混合）并写出理由，规则见 `Docs/06_CROSS_AGENT_WORKFLOW.md` 第 D 节，优先级：质量 > 速度 > token。
