# Day 5 – Application Integration

The C++ side has two sources:

- `SimulatedSensor` for easy local testing.
- `DeviceMonitor` for the Linux character device.

Both implement the same `TemperatureSource` interface.

This keeps the application code simple while allowing the real driver to be connected   in a Linux VM.
