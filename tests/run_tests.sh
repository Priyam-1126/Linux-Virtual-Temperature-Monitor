#!/usr/bin/env bash
# Runs all tests and prints a PASS/FAIL summary. Use: make test
set -uo pipefail

TARGET="build/temperature_monitor"
TEST_BIN="build/test_core"
FAILED=0

run() {
    local label="$1"; shift
    printf '%s\n' "$label"
    if "$@" > build/last-test.txt 2>&1; then
        echo "      PASS"
    else
        echo "      FAIL"
        sed 's/^/      /' build/last-test.txt
        FAILED=1
    fi
    echo
}

echo
echo "Linux Virtual Temperature Monitor - Test Results"
echo "================================================"
echo

run "[1/4] C++ build (make)"                       test -x "$TARGET"
run "[2/4] Unit tests (alert, logger, sensor, device code)" "$TEST_BIN"
run "[3/4] Integration test (simulation + bad input)"       bash tests/integration_test.sh "$TARGET"
run "[4/4] Device-interface test (file stand-in)"           bash tests/device_interface_test.sh "$TARGET"

echo "Not covered here: loading the kernel driver (needs matching kernel headers and root)."
echo
if [ "$FAILED" -eq 0 ]; then
    echo "Result: ALL TESTS PASSED"
else
    echo "Result: SOME TESTS FAILED"
    exit 1
fi
