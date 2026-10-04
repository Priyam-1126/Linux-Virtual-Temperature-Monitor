# UML Diagrams – Stage 3

The first drafts were made during design. After implementation (Stage 4–5) they were
updated so that they match the final code in `include/` and `src/`.

## Class Diagram

```mermaid
classDiagram
    class TemperatureReading {
        <<struct>>
        +double celsius
        +string source
        +time_t timestamp
    }

    class TemperatureSource {
        <<abstract>>
        +read(reading, error) bool
        +writeTemperature(value, error) bool
        +available() bool
        +name() string
    }

    class SimulatedSensor {
        -double temperature_
        +read(reading, error) bool
        +writeTemperature(value, error) bool
        +available() bool
        +name() string
    }

    class DeviceMonitor {
        -string devicePath_
        +read(reading, error) bool
        +writeTemperature(value, error) bool
        +available() bool
        +probe(error) bool
        +name() string
    }

    class AlertManager {
        -double threshold_
        +isWarning(temperature) bool
        +status(temperature) string
        +threshold() double
    }

    class Logger {
        -string filePath_
        +write(message) bool
        +path() string
    }

    class main {
        <<program>>
        menu loop
    }

    TemperatureSource <|-- SimulatedSensor
    TemperatureSource <|-- DeviceMonitor
    TemperatureSource ..> TemperatureReading : fills
    main --> TemperatureSource : uses one source
    main --> AlertManager : checks limit
    main --> Logger : saves events
```

**How to read it**

- `TemperatureSource` is an abstract class (like a Java interface). `main` only knows this type.
- `SimulatedSensor` and `DeviceMonitor` are the two real sources.
- `AlertManager` and `Logger` are used by `main`; they do not know about each other.

## Sequence Diagram – device mode

```mermaid
sequenceDiagram
    actor User
    participant Main as main (C++ app)
    participant Dev as DeviceMonitor
    participant Kernel as Linux kernel
    participant Drv as Character driver
    participant Alert as AlertManager
    participant Log as Logger

    User->>Main: Choose 1 (Read temperature)
    Main->>Dev: read()
    Dev->>Kernel: open, read, close
    Kernel->>Drv: vtemp_open, vtemp_read
    Drv-->>Kernel: "28\n" (copy_to_user)
    Kernel-->>Dev: bytes
    Dev-->>Main: TemperatureReading (28.0)
    Main->>Alert: status(28.0)
    Alert-->>Main: NORMAL
    Main->>Log: write("READ | ...")
    Main-->>User: Show temperature and status
```

In **simulation mode** the steps with Kernel and Driver are skipped:
`main` calls `SimulatedSensor::read()`, which returns its stored value.

## State Machine Diagram

```mermaid
stateDiagram-v2
    [*] --> Starting
    Starting --> Menu: source chosen (device or simulation)

    Menu --> Reading: choose 1
    Reading --> Normal: temperature <= 50
    Reading --> Warning: temperature > 50
    Reading --> ReadError: read fails
    Normal --> Menu
    Warning --> Menu
    ReadError --> Menu: error shown and logged

    Menu --> Setting: choose 2
    Setting --> Menu: value stored, or error shown

    Menu --> Menu: choose 3 (status) or invalid choice
    Menu --> [*]: choose 4 (exit)
```

The source  is chosen once, at start-up.
