#!/bin/bash
# Static analysis scan for MaaYYS business layer
# Checks for forbidden dependencies and code quality issues

SRC_DIR="d:/code/maa/MaaAssistantYYS/src/MaaYYS"
FAIL=0

echo "=== MaaYYS Static Scan ==="

check() {
    local pattern="$1"
    local desc="$2"
    local count
    count=$(grep -r "$pattern" "$SRC_DIR" --include="*.cpp" --include="*.h" -l 2>/dev/null | \
            grep -v "Bridge/" | grep -v "Tools/" | wc -l)
    if [ "$count" -gt 0 ]; then
        echo "FAIL: $desc ($count files)"
        grep -r "$pattern" "$SRC_DIR" --include="*.cpp" --include="*.h" -l 2>/dev/null | \
            grep -v "Bridge/" | grep -v "Tools/"
        FAIL=1
    else
        echo "  OK: $desc"
    fi
}

check 'm_ctx->resource()' 'Direct resource access (use ITemplateResolver)'
check 'm_ctx->controller()' 'Direct controller access (use IActionExecutor)'
check 'm_ctx->vision()' 'Direct vision access (use ITemplateResolver)'
check 'asst/Controller' 'MaaCore Controller include in business code'
check 'asst/Vision' 'MaaCore Vision include in business code'
check 'Logger.hpp' 'MaaUtils Logger include (use YYSLogger)'

echo ""
if [ "$FAIL" -eq 0 ]; then
    echo "static_scan=ok"
else
    echo "static_scan=failed"
fi
