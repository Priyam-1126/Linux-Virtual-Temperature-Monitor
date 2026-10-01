# Day 4 – Stage 4: Initial Implementation & Prototype

Today the project moved from design to actual implementation.

## Work Completed

### C++ Application
The first version of the monitoring application was created. It can take a temperature value, compare it with the project limit of 50 C, show the current status, and record basic events in a log.

The application was kept in separate modules so that the temperature source can later be changed without changing the whole program.

### Linux Character Device Driver
A basic Linux character-device driver was added for the virtual temperature device. The driver is the Linux-side interface that will allow the user application to communicate with the virtual device.

The driver uses the normal character-device approach with a device node and read/write operations.

## Prototype Flow

```text
Temperature Source
        ↓
C++ Monitoring Application
        ↓
Threshold Check
   ┌────┴────┐
   ↓         ↓
NORMAL     WARNING
        ↓
       Log
```

The planned Linux device flow is:

```text
C++ Application
       ↓
   Linux Device
       ↓
Character Driver
       ↓
Virtual Temperature Device
```

## Main Files Added

```text
src/
├── main.cpp
├── alert_manager.cpp
├── device_monitor.cpp
├── logger.cpp
└── simulated_sensor.cpp

include/
├── alert_manager.hpp
├── device_monitor.hpp
├── logger.hpp
├── simulated_sensor.hpp
├── temperature_reading.hpp
└── temperature_source.hpp

driver/
├── virtual_temperature.c
└── Makefile
```

## Development Note
The application supports simulation so the core C++ logic can be checked without loading the kernel module. The real device mode is kept for the Linux environment where the driver can be loaded and accessed.

## Next Step
Connect the application with the device interface and continue with integration and testing.
