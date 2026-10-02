# Day 2 - Stage 2: Project Requirements and Development Plan



## Functional Requirements

1. The system should provide a virtual temperature value.
2. The Linux device driver should provide an interface to the application.
3. The C++ application should read and display the temperature.
4. The system should show the device status.
5. The system should give an alert when the temperature goes above the set limit.
6. The system should save important events in a log file.

## Non-Functional Requirements

- The program should be simple to use from the Linux terminal.
- The code should be divided into clear modules.
- Basic input and device errors should be handled.
- The project should run in an Ubuntu/Linux environment.
- Changes should be tracked with Git.

## Main Modules

### 1. Virtual Temperature Sensor
Provides sample temperature values.

### 2. Linux Character Device Driver
Provides the Linux interface between the virtual device and the application.

### 3. C++ Monitoring Application
Reads the temperature and displays the result.

### 4. Alert Manager
Checks the temperature against the selected limit and shows a warning.

### 5. Logger
Stores important readings, alerts and errors.

## Main Features

### Must Have

- Temperature reading
- Device interface
- Temperature display
- High-temperature alert
- Basic logging

### Optional

- Small usability improvements
- Extra device-status details

Optional features will only be added if the main project is working first.

## Deliverables

- C++ source code
- Linux device-driver source code
- Build files
- Project documentation
- Required UML and design diagrams
- Test cases and results
- GitHub repository
- Final project report

## Development Plan

| Stage | Work |
|---|---|
| Stage 1 | Project introduction |
| Stage 2 | Requirements and development plan |
| Stage 3 | System design, architecture and UML |
| Stage 4 | Initial implementation and prototype |
| Stage 5 | Testing, integration and improvement |
| Stage 6 | Final implementation and presentation |



## Next Step

Prepare the system architecture, component responsibilities and design diagrams for Stage 3.
