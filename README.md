# Linux-Based Virtual Temperature Monitoring System

A small Linux and C++ project that demonstrates how a user-space application can read and update temperature data through a Linux character device. The project also checks a temperature limit and keeps a simple log.

## Main Features

- Virtual temperature sensor for local testing
- Linux character device driver
- C++ monitoring application
- Temperature threshold alert
- Device status
- Simple file logging
- Unit and integration tests
- Makefile-based build

## Project Flow

```text
Virtual Temperature Source
          |
          v
Linux Character Device Driver
          |
          v
C++ Monitoring Application
       /        \
      v          v
Alert Manager  Logger
```

The application supports two modes:

1. `--simulate` for development and testing without a kernel module.
2. `--device /dev/virtual_temperature` for the Linux character device.

## Project Structure

```text
Linux-Virtual-Temperature-Monitor/
├── include/               # C++ headers (.hpp)
├── src/                   # C++ application
├── driver/                # Linux character-device driver
├── tests/                 # Unit and integration tests
├── docs/                  # Six-day project documentation
├── progress/              # Short daily progress notes
├── report/                # Final project report
├── presentation/          # Final presentation
├── logs/                  # Runtime logs (ignored by Git)
├── Makefile
└── .gitignore
```

## Build the C++ Application

```bash
make
```

Run the local simulator:

```bash
./build/temperature_monitor --simulate
```

## Test

```bash
make test
```

The tests cover the threshold logic, logging, and the main simulated application flow.

## Linux Driver

The driver source is in `driver/virtual_temperature.c`. It remains C because the Linux kernel and normal out-of-tree kernel modules are developed in GNU C; the user-space project code is C++.

Build it on a Linux system with matching kernel headers:

```bash
cd driver
make
```

The resulting module is `virtual_temperature.ko`.

On a suitable test Linux VM, the module can then be loaded and the device node can be checked under `/dev/virtual_temperature`.

## Current Verification

The C++ application and tests were built and executed successfully in a Linux build environment. The driver was compiled successfully against the available 6.12.96 kernel headers. The running build environment uses kernel 6.18.44, so a live module-load test was not performed because the running kernel and available headers do not match and module loading requires a controlled privileged Linux environment.

## Six-Day Project Plan

- Day 1 – Stage 1: Project Introduction
- Day 2 – Stage 2: Requirements and Development Plan
- Day 3 – Stage 3: System Design and Architecture
- Day 4 – Stage 4: C++ Prototype + Linux Character Driver
- Day 5 – Stage 4 Integration + Stage 5 Testing and Improvement
- Day 6 – Stage 6: Final Implementation and Presentation

The six-day plan combines the implementation and testing work into the available project window while keeping the required project stages.

## Future Scope

- Read from a real temperature sensor
- Add more device types
- Add periodic background monitoring
- Add a small web or desktop dashboard
