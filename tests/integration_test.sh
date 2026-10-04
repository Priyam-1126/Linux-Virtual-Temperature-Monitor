#!/usr/bin/env bash
 set -euo pipefail

TARGET="${1:?usage: integration_test.sh PATH_TO_BINARY}"
LOG="build/integration.log"
OUT="build/integration-output.txt"
rm -f "$LOG" "$OUT"

# 1 read, 2 set 65, 1 read, 3 status, 4 exit
printf '1\n2\n65\n1\n3\n4\n' | "$TARGET" --simulate --log "$LOG" > "$OUT"

grep -q "Temperature : 28.0 C" "$OUT"
grep -q "Status      : NORMAL" "$OUT"
grep -q "Temperature : 65.0 C" "$OUT"
grep -q "Status      : WARNING" "$OUT"
grep -q "Available     : YES" "$OUT"

grep -q "START | using simulated sensor" "$LOG"
grep -q "READ | temperature=28.0C | status=NORMAL" "$LOG"
grep -q "SET | temperature=65C" "$LOG"
grep -q "READ | temperature=65.0C | status=WARNING" "$LOG"
grep -q "STOP | application closed" "$LOG"

# Bad input: letters, out-of-range value, unknown menu option
rm -f "$LOG" "$OUT"
printf '2\nabc\n2\n200\n7\n1\n4\n' | "$TARGET" --simulate --log "$LOG" > "$OUT"
grep -q "Invalid temperature" "$OUT"
grep -q "between -50 C and 150 C" "$OUT"
grep -q "Please choose 1, 2, 3 or 4" "$OUT"
grep -q "Temperature : 28.0 C" "$OUT"          # value unchanged
grep -q "SET_ERROR | invalid numeric input" "$LOG"

echo "Integration test passed."
