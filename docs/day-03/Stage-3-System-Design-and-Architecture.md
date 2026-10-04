# Day 3 – Stage 3: System Design & Architecture

## Project
Linux-Based Virtual Temperature Monitoring System

## Stage Status
Completed (diagrams updated after implementation)

## 1. Basic System Flow

The project will have a simple flow:

Virtual Temperature Sensor
→ Linux Character Device Driver
→ C++ Monitoring Application
→ Temperature Check
→ Alert and Log

The application will read the temperature through the Linux device interface.

## 2. Main Components

### Virtual Temperature Sensor
Provides a sample temperature value for testing.

### Linux Character Device Driver
Acts as the interface between the virtual device and the user application.

### C++ Monitoring Application
Reads the temperature and shows the current value and device status.

### Alert Manager
Checks the temperature against a fixed limit and gives a warning when needed.

### Logger
Stores important readings and alerts in a log file.

## 3. Data Flow

1. The virtual sensor provides a temperature value.
2. The device driver exposes the device interface.
3. The C++ application reads the value.
4. The application checks the temperature limit.
5. An alert is shown when the limit is crossed.
6. Important events are written to the log.

## 4. Design Approach

The project will be divided into small modules so that each part can be developed and tested separately.

This version will use a terminal-based C++ application.

## 5. Implementation Order

1. Create the basic C++ application.
2. Add the temperature data handling.
3. Prepare the Linux device interface.
4. Implement the character driver.
5. Connect the application with the driver.
6. Add alerts and logging.
7. Test the complete flow.

## 6. Git Strategy

The project will be maintained using Git.

The main branch will contain stable work. Feature branches can be used for separate modules when needed.

Daily progress will be committed with clear messages.


## Next Step

Start the initial C++ prototype in Stage 4.
