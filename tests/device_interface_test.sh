#!/usr/bin/env bash

set -euo pipefail

TARGET="${1:?usage: device_interface_test.sh PATH_TO_BINARY}"
FAKE="build/fake_virtual_temperature"
LOG="build/device.log"
OUT="build/device-output.txt"

# Read from the device file
rm -f "$FAKE" "$OUT" "$LOG"
printf '41\n' > "$FAKE"
printf '1\n4\n' | "$TARGET" --device "$FAKE" --log "$LOG" > "$OUT"
grep -q "Source: LINUX_CHARACTER_DEVICE" "$OUT"
grep -q "Temperature : 41.0 C" "$OUT"
grep -q "Status      : NORMAL" "$OUT"
grep -q "START | using device=$FAKE" "$LOG"

# Write through the device file, then read back
printf '2\n65\n1\n4\n' | "$TARGET" --device "$FAKE" --log "$LOG" > "$OUT"
grep -q "Temperature : 65.0 C" "$OUT"
grep -q "Status      : WARNING" "$OUT"
grep -qx '65' "$FAKE"                        # exactly "65": integer text, as the driver expects

# Device file missing -> falls back to simulation with a clear message
printf '1\n4\n' | "$TARGET" --device build/no_such_device --log "$LOG" > "$OUT"
grep -q "Note: cannot use build/no_such_device" "$OUT"
grep -q "Source: SIMULATED_SENSOR" "$OUT"

# Device returns garbage -> read error is shown and logged, program keeps running
printf 'abc\n' > "$FAKE"
printf '1\n4\n' | "$TARGET" --device "$FAKE" --log "$LOG" > "$OUT" 2>&1
grep -q "Read error: device returned an invalid temperature" "$OUT"
grep -q "READ_ERROR" "$LOG"

echo "Device interface test passed (regular-file stand-in)."
