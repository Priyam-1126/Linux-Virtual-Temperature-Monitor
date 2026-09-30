# Component Design

## 1. Virtual Temperature Sensor
**Responsibility:** Generate a simple temperature value for the project.

**Input:** Simulated sensor value.

**Output:** Temperature value.

## 2. Linux Character Device Driver
**Responsibility:** Provide a Linux device interface for communication with the application.

**Input:** Device operation requests.

**Output:** Temperature data / device response.

## 3. C++ Monitoring Application
**Responsibility:** Read the temperature and display the current device status.

**Input:** Data from the device interface.

**Output:** Temperature and status on the terminal.

## 4. Alert Manager
**Responsibility:** Check the temperature against the configured limit.

**Input:** Current temperature.

**Output:** Normal status or high-temperature warning.

## 5. Logger
**Responsibility:** Store important events and alerts.

**Input:** Reading or alert message.

**Output:** Log file entry.

## Component Relationship

```text
Virtual Sensor
      |
      v
Character Device Driver
      |
      v
C++ Monitoring Application
      |
      +----> Alert Manager
      |
      +----> Logger
```
