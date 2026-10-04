# Day 5 – Testing

Run everything with `make test`. The saved result is in `tests/results/test.txt`.

| Level | File | What is checked |
|---|---|---|
| Unit | `tests/test_core.cpp` | alert limit (50 is NORMAL, 50.1 is WARNING), logger, simulated sensor, device code with a file |
| Integration | `tests/integration_test.sh` | whole menu flow in simulation, bad input, log lines |
| Device interface | `tests/device_interface_test.sh` | read, write, missing device fallback, bad data from a stand-in file |
| Driver build | `driver/Makefile` | module compiles against kernel headers (`tests/results/driver-build-output.txt`) |

 
