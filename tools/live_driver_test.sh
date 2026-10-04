#!/usr/bin/env bash
# One-command live test of the kernel driver + C++ app.
# Run on a Linux VM that has kernel headers for the running kernel:
#     sudo apt install build-essential linux-headers-$(uname -r)
#     bash tools/live_driver_test.sh
# Output is saved to tests/results/driver-live-test.txt (real output from YOUR machine).
set -u
cd "$(dirname "$0")/.."
OUT="tests/results/driver-live-test.txt"
mkdir -p tests/results build
PASS=0; FAIL=0

say()  { echo "$@" | tee -a "$OUT"; }
step() { # step "title" command...
    local title="$1"; shift
    say ""; say "\$ $*"
    local result; result="$("$@" 2>&1)"; local rc=$?
    [ -n "$result" ] && say "$result"
    if [ $rc -eq 0 ]; then say "  -> PASS: $title"; PASS=$((PASS+1)); else say "  -> FAIL: $title (exit $rc)"; FAIL=$((FAIL+1)); fi
    return $rc
}
check() { # check "title" expected actual
    if [ "$2" = "$3" ]; then say "  -> PASS: $1 (got '$3')"; PASS=$((PASS+1)); else say "  -> FAIL: $1 (expected '$2', got '$3')"; FAIL=$((FAIL+1)); fi
}

: > "$OUT"
say "Live driver test - $(date '+%Y-%m-%d %H:%M:%S')"
say "User: $(whoami)@$(hostname)   Kernel: $(uname -r)"
say "=============================================================="

if [ ! -d "/lib/modules/$(uname -r)/build" ]; then
    say "STOP: kernel headers for $(uname -r) are missing."
    say "Install them:  sudo apt install linux-headers-$(uname -r)"
    exit 1
fi

sudo rmmod virtual_temperature 2>/dev/null   # clean start if loaded earlier

step "build C++ application"           make
step "build kernel module"             make -C driver
step "load module (insmod)"            sudo insmod driver/virtual_temperature.ko || { say "Cannot continue."; exit 1; }
step "device node exists"              ls -l /dev/virtual_temperature
sudo chmod 666 /dev/virtual_temperature

step "module is listed (lsmod)"        bash -c "lsmod | grep virtual_temperature"
check "initial value is 28"            "28" "$(cat /dev/virtual_temperature)"
step  "cat stops (no endless loop)"    timeout 3 cat /dev/virtual_temperature
step  "write 65 with echo"             bash -c "echo 65 | sudo tee /dev/virtual_temperature"
check "value is now 65"                "65" "$(cat /dev/virtual_temperature)"
say ""; say "\$ echo 999 > /dev/virtual_temperature   (must be rejected)"
if echo 999 > /dev/virtual_temperature 2>/dev/null; then say "  -> FAIL: out-of-range value accepted"; FAIL=$((FAIL+1)); else say "  -> PASS: out-of-range value rejected"; PASS=$((PASS+1)); fi
say ""; say "\$ echo abc > /dev/virtual_temperature   (must be rejected)"
if echo abc > /dev/virtual_temperature 2>/dev/null; then say "  -> FAIL: text accepted"; FAIL=$((FAIL+1)); else say "  -> PASS: invalid text rejected"; PASS=$((PASS+1)); fi

say ""; say "\$ printf '1\\n2\\n30\\n1\\n3\\n4\\n' | ./build/temperature_monitor --log build/live.log"
APP_OUT="$(printf '1\n2\n30\n1\n3\n4\n' | ./build/temperature_monitor --log build/live.log 2>&1)"
say "$APP_OUT"
echo "$APP_OUT" | grep -q "Source: LINUX_CHARACTER_DEVICE" && { say "  -> PASS: app used the real device"; PASS=$((PASS+1)); } || { say "  -> FAIL: app did not use the device"; FAIL=$((FAIL+1)); }
echo "$APP_OUT" | grep -q "Temperature : 65.0 C" && { say "  -> PASS: app read 65 (WARNING range) from driver"; PASS=$((PASS+1)); } || { say "  -> FAIL: app did not read 65"; FAIL=$((FAIL+1)); }
check "app wrote 30 into the driver" "30" "$(cat /dev/virtual_temperature)"

say ""; say "\$ dmesg | grep virtual_temperature | tail"
sudo dmesg | grep virtual_temperature | tail -n 12 | tee -a "$OUT"

step "unload module (rmmod)"           sudo rmmod virtual_temperature
say ""; say "\$ ls /dev/virtual_temperature   (must be gone)"
if [ -e /dev/virtual_temperature ]; then say "  -> FAIL: device node still exists"; FAIL=$((FAIL+1)); else say "  -> PASS: device node removed"; PASS=$((PASS+1)); fi

say ""; say "=============================================================="
say "Result: $PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ] && say "LIVE DRIVER TEST: ALL PASSED" || say "LIVE DRIVER TEST: SOME FAILED (send me this file)"
