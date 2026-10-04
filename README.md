# Linux-Based Virtual Temperature Monitoring System

A small Linux + C++ project made for the Wipro training (capstone).

A virtual sensor provides a temperature. A C++ application reads it, compares it with a
50 °C limit, shows `NORMAL` or `WARNING`, and writes events to a log file.
A Linux character device driver is included to show how an application talks to a device.

## Problem statement

Software cannot access hardware directly. A driver gives the application a simple
interface to the device. This project studies that idea with a virtual temperature
sensor, so no real hardware is needed.

## Objectives

- Read a temperature from a virtual device through a Linux device file
- Show the temperature and status with a C++ application
- Warn when the temperature is above 50 °C
- Keep a log of important events

## Features

- Menu: read temperature, set temperature, device status, exit
- `NORMAL` / `WARNING` status (limit 50.0 °C, exactly 50.0 is NORMAL)
- Input checks (letters and values outside -50 to 150 °C are rejected)
- Log file with timestamps
- Automatic fallback to a simulated sensor if the device cannot be used
- Linux kernel driver for `/dev/virtual_temperature`

## Technologies used

C++17 · Linux · Linux system calls (`open`, `read`, `write`, `close`) ·
Linux character device driver (C) · g++ · Make · Git/GitHub

> The application is C++. The kernel driver is C, because Linux kernel modules are
> written in C. The `.hpp` files are C++ headers.

## System architecture

```text
 main.cpp (menu)
     |
     +--> TemperatureSource  (abstract class)
     |        |-- SimulatedSensor   value kept inside the program
     |        '-- DeviceMonitor     open/read/write on /dev/virtual_temperature
     |                                      |
     |                              Linux kernel (system calls)
     |                                      |
     |                              Character driver (driver/virtual_temperature.c)
     |
     +--> AlertManager  (NORMAL / WARNING)
     '--> Logger        (logs/temperature.log)
```

At start-up the program tries to open the device. If that works it uses `DeviceMonitor`,
otherwise it uses `SimulatedSensor` and prints the reason.

## Project structure

```text
Linux-Virtual-Temperature-Monitor/
├── include/      C++ header files
├── src/          C++ source files
├── driver/       Linux character driver, its Makefile, driver guide
├── tests/        unit, integration and device tests; results/
├── docs/         stage-wise documentation (day-01 ... day-06), UML in day-03
├── progress/     short day-wise notes
├── tools/        live_driver_test.sh (one-command driver test)
├── logs/         log files are created here
├── Makefile
└── README.md
```

| File | Job |
|---|---|
| `src/main.cpp` | start-up, menu loop |
| `include/temperature_source.hpp` | abstract class every source follows |
| `src/simulated_sensor.cpp` | fake sensor, default 28.0 °C |
| `src/device_monitor.cpp` | reads/writes the device file with system calls |
| `src/alert_manager.cpp` | limit check |
| `src/logger.cpp` | writes log lines |
| `driver/virtual_temperature.c` | kernel module |

## Requirements

- Linux (Ubuntu recommended)
- `g++` (C++17), `make`
- For the driver only: kernel headers matching your kernel, and `sudo`

```bash
sudo apt install build-essential linux-headers-$(uname -r)
```

## Build and run the application

Run these commands from the project folder:

```bash
make                                  # builds build/temperature_monitor
./build/temperature_monitor --simulate
```

Options:

| Option | Meaning |
|---|---|
| `--simulate` | use the simulated sensor |
| `--device PATH` | use another device file (default `/dev/virtual_temperature`) |
| `--log FILE` | log file (default `logs/temperature.log`) |
| `--help` | show usage |

Other commands: `make run` (simulation), `make test`, `make clean`.

## Example

```text
Choose: 1
Temperature : 28.0 C
Source      : SIMULATED_SENSOR
Status      : NORMAL
Threshold   : 50.0 C

Choose: 2    (enter 65)
Choose: 1
Temperature : 65.0 C
Status      : WARNING
```

Log file (`logs/temperature.log`):

```text
2026-10-04 17:31:00 | START | using simulated sensor
2026-10-04 17:31:00 | READ | temperature=28.0C | status=NORMAL | source=SIMULATED_SENSOR
2026-10-04 17:31:00 | SET | temperature=65C
2026-10-04 17:31:00 | READ | temperature=65.0C | status=WARNING | source=SIMULATED_SENSOR
```

More output is saved in `tests/results/` (`local-execution-output.txt`, `test.txt`).

## Use the Linux driver

Full steps and common problems are in [`driver/driver.md`](driver/driver.md).

```bash
cd driver && make
sudo insmod virtual_temperature.ko
cat /dev/virtual_temperature
echo 65 | sudo tee /dev/virtual_temperature
cat /dev/virtual_temperature
sudo chmod 666 /dev/virtual_temperature     # lets the app open it without sudo
cd .. && ./build/temperature_monitor        # now shows source LINUX_CHARACTER_DEVICE
sudo rmmod virtual_temperature
```

The device is chosen when the program starts, so load the driver first.
The driver keeps whole degrees only (for example `65`).

**One-command live test** (needs a Linux VM with kernel headers and `sudo`):

```bash
bash tools/live_driver_test.sh
```

It builds, loads the module, reads/writes, runs the app on the real device, unloads, and
saves the result in `tests/results/driver-live-test.txt`.

## Testing

```bash
make test
```

| Test | What it checks |
|---|---|
| Unit (`tests/test_core.cpp`) | alert limit, logger, simulated sensor, device code with a file |
| Integration (`tests/integration_test.sh`) | full menu flow, wrong input, log content |
| Device interface (`tests/device_interface_test.sh`) | read/write, missing device, bad data (a normal file stands in for the device) |
| Driver build | module compiles against kernel headers (`tests/results/driver-build-output.txt`) |

## Project stages

| Stage | Work | Documents |
|---|---|---|
| 1 | Project introduction | `docs/day-01/` |
| 2 | Requirements and development plan | `docs/day-02/` |
| 3 | System design, architecture, UML | `docs/day-03/` |
| 4 | Initial implementation and prototype | `docs/day-04/` |
| 5 | Testing, integration, improvement | `docs/day-05/` |
| 6 | Final implementation and presentation | `docs/day-06/` |

Short day-wise notes are in `progress/`.

## Limitations

- The sensor is virtual.
- Terminal only, one device, one fixed limit (50 °C).

## Future scope

- Connect a real sensor (I2C/SPI) and change only the driver
- Continuous background monitoring
- More than one device
- Simple dashboard or remote monitoring

## Conclusion

The project shows the full path from a user program to a kernel driver: C++ application,
system calls, character device, alert and log, with tests and stage-wise documentation.
