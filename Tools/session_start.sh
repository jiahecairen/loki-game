#!/bin/bash
# 开工检查：任何 Agent 新会话的第一步。只读，不修改任何文件。
cd "$(dirname "$0")/.." || exit 1
echo "===== LOKI 开工检查 ($(date '+%F %T')) ====="
echo
echo "--- Docs/STATE.md ---"
cat Docs/STATE.md 2>/dev/null || echo "(缺少 Docs/STATE.md)"
echo
echo "--- Git 分支与最近提交 ---"
git branch --show-current 2>/dev/null
git log --format='%h %ad %an %s' --date=format:'%m-%d %H:%M' -5 2>/dev/null
echo
echo "--- Git 未提交改动 ---"
git status --short 2>/dev/null | head -30
echo
echo "--- 编辑器 / MCP ---"
if pgrep -f "UnrealEditor" >/dev/null 2>&1; then echo "UnrealEditor: 正在运行（若有别的 Agent 也在用，请先和用户确认）"; else echo "UnrealEditor: 未运行"; fi
if nc -z 127.0.0.1 8765 >/dev/null 2>&1; then echo "MCP 端口 8765: 可连接"; else echo "MCP 端口 8765: 未监听（编辑器未开或 MCP 未启动）"; fi
echo
echo "--- 开发日志最近 12 行 ---"
tail -12 Docs/01_DEV_LOG.md 2>/dev/null | cut -c1-200
echo
echo "必读：Docs/05_HANDOFF.md、Docs/06_CROSS_AGENT_WORKFLOW.md。若 STATE 状态为'进行中'，先向用户汇报并核对再继续。"
