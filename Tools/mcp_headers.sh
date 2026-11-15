#!/bin/sh
# 读取 EngineWire 生成的令牌，输出为 MCP 请求头（令牌文件在 UE 首次启动后生成）
F="/Users/rain/DevGame/Loki/Saved/MCP/capability-token"
if [ -f "$F" ]; then printf '{"X-MCP-Capability-Token":"%s"}' "$(tr -d '\r\n' < "$F")"; else printf '{}'; fi
