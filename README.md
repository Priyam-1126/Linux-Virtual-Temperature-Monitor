# Linux-Based Virtual Temperature Monitoring System

A small Linux and C++ project developed as a Wipro training project.

The main idea is simple: a temperature value is provided by a virtual sensor, the C++ application reads it, checks a limit, and shows the current status. A Linux character device driver is included to demonstrate how a user-space application can communicate with a device interface.

## What the project does

The application can:

- Read the current temperature
- Set a new temperature value
- Show `NORMAL` or `WARNING` based on a 50°C limit
- Show the current device source and availability
- Save important events in a log file
- Work in simulation mode when the real Linux device is not available

The project also includes a Linux character-device driver for the virtual temperature device.

## Basic flow

```text
Virtual Temperature Source
          |
          v
Linux Character Device
          |
          v
C++ Monitoring Application
       /          \
      v            v
Alert Manager    Logger
```

For local testing, the application can use the simulated sensor. When a real device node such as `/dev/virtual_temperature` is available, the same application can use it through the device interface.

## Technologies used

- C++17
- Linux
- Linux System Programming
- Linux Character Device Driver
- g++
- Make
- Git / GitHub

> **Note:** The user-space application is written in C++. The Linux kernel driver is written in C because Linux kernel modules use the kernel's C-based development model. The `.hpp` files in this project are C++ header files.

## Project structure

```text
Linux-Virtual-Temperature-Monitor/
├── include/                    # C++ header files
├── src/                        # C++ application source files
├── driver/                     # Linux character-device driver
├── tests/                      # Unit and integration tests
│   └── results/                # Test and demo output
├── docs/                       # Stage-wise project documentation
│   ├── day-01/
│   ├── day-02/
│   ├── day-03/
│   ├── day-04/
│   ├── day-05/
│   └── day-06/
├── progress/                   # Short day-wise progress records
├── logs/                       # Runtime log files
├── Makefile
├── PROJECT-MAP.md
└── .gitignore
```

## How the application works

The project uses a common `TemperatureSource` interface. There are two implementations:

- `SimulatedSensor` – used for normal local testing
- `DeviceMonitor` – used when the Linux character device is available

The application normally checks for the device first. When it is not available, it falls back to the simulated sensor.

The default temperature is **28.0°C** and the warning limit is **50.0°C**.

Example:

```text
Temperature : 28.0 C
Source      : SIMULATED_SENSOR
Status      : NORMAL
Threshold   : 50.0 C
```

After changing the value to 65°C:

```text
Temperature : 65.0 C
Source      : SIMULATED_SENSOR
Status      : WARNING
Threshold   : 50.0 C
```

## Build the C++ application

Run these commands from the project folder on Linux:

```bash
make
```

The executable is created at:

```text
build/temperature_monitor
```

## Run in simulation mode

```bash
./build/temperature_monitor --simulate
```

You can also use:

```bash
make run
```

The terminal menu provides options to read temperature, set temperature, check device status, and exit.

## Testing

Run the test suite with:

```bash
make test
```

The current tests cover:

- C++ build
- Alert threshold logic
- Logger
- Simulator integration
- Device-interface stand-in

The driver source is also checked by compiling it against Linux kernel headers.

Test and demo records are kept in:

```text
tests/results/
```

## Linux character driver

The driver source is:

```text
driver/virtual_temperature.c
```

Build it on a Linux system with headers that match the running kernel:

```bash
cd driver
make
```

This creates:

```text
virtual_temperature.ko
```

On a suitable Linux system, the basic driver flow is:

```bash
sudo insmod virtual_temperature.ko
ls -l /dev/virtual_temperature
cat /dev/virtual_temperature
echo 65 | sudo tee /dev/virtual_temperature
cat /dev/virtual_temperature
sudo rmmod virtual_temperature
```

The live kernel-module load was not performed in the verification environment because the available headers and running kernel did not match and privileged module loading was not available there. The project does not mark that step as passed.

## Project documentation

The documentation follows the six project stages given for the Wipro training:

1. **Stage 1** – Project Introduction
2. **Stage 2** – Requirements and Development Plan
3. **Stage 3** – System Design, Architecture and UML
4. **Stage 4** – Initial Implementation and Prototype
5. **Stage 5** – Testing, Integration and Improvement
6. **Stage 6** – Final Implementation and Presentation

The `progress/` folder keeps one short progress note for each project day.

## Final project demo

The main demo is based on the simulation mode:

1. Start the application.
2. Read the default temperature of 28°C.
3. Set the temperature to 65°C.
4. Read it again and show the warning.
5. Check the device status.
6. Show the generated log file.

## Limitations

- The temperature sensor is virtual.
- Live driver testing needs a suitable Linux environment with matching kernel headers and permission to load kernel modules.
- The current version is terminal-based and does not use a web or desktop interface.

## Future improvements

- Connect a real temperature sensor
- Add more device types
- Add continuous/background monitoring
- Add a simple dashboard or remote monitoring option


