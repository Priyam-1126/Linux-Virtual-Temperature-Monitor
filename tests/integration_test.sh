#!/priyamusr/bin/env bash
set -euo pipefail

TARGET="${1:?usage: integration_test.sh PATH_TO_BINARY}"
TMP_LOG="build/integration.log"
rm -f "$TMP_LOG"

OUTPUT_FILE="build/integration-output.txt"
rm -f "$OUTPUT_FILE"

printf '1\n2\n65\n1\n4\n' | "$TARGET" --simulate > "$OUTPUT_FILE"

grep -q "Temperature : 28.0 C" "$OUTPUT_FILE"
grep -q "Temperature : 65.0 C" "$OUTPUT_FILE"
grep -q "Status      : WARNING" "$OUTPUT_FILE"

grep -q "START | using simulated sensor" logs/temperature.log
grep -q "SET | temperature=65" logs/temperature.log
grep -q "READ | temperature=65.0C | status=WARNING" logs/temperature.log

echo "Integration test passed."
