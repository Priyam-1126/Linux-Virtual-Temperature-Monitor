#!/priyam/bin/env bash
set -euo pipefail

TARGET="${1:?usage: device_interface_test.sh PATH_TO_BINARY}"
FAKE_DEVICE="build/fake_virtual_temperature"
OUTPUT_FILE="build/device-output.txt"
rm -f "$FAKE_DEVICE" "$OUTPUT_FILE"
printf '41\n' > "$FAKE_DEVICE"
printf '1\n4\n' | "$TARGET" --device "$FAKE_DEVICE" > "$OUTPUT_FILE"
grep -q "Source: LINUX_CHARACTER_DEVICE" "$OUTPUT_FILE"
grep -q "Temperature : 41.0 C" "$OUTPUT_FILE"
grep -q "Status      : NORMAL" "$OUTPUT_FILE"
echo "Device interface test passed (regular-file stand-in)."
