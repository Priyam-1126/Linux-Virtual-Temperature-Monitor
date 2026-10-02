# Day 5 –Integration and Stage 5 Testing

Today the separate project pieces were brought together and checked with tests.

## Integration

The C++ application uses a common temperature-source interface. This keeps the application logic the same while allowing two sources:

- simulation mode for normal local testing
- Linux device mode for `/dev/virtual_temperature` when the driver is available

The main flow is:

```text
C++ Application
      ↓
Temperature Source
      ↓
Temperature Reading
      ↓
Alert Check
      ↓
Log Event
```

## Testing

The project includes:

- unit tests for the temperature limit and logger
- an integration test for the simulated application flow
- a device-interface test for the expected device behaviour

The tested cases include normal temperature, the warning limit, a high temperature, and log creation.

## Driver Build Check

The character-driver source was compiled against the available Linux 6.12.96 headers in the build environment.

A live kernel-module load was not performed there because the running kernel was different from the available headers and module loading needs a controlled Linux environment with the required permissions.

## Result

The user-space C++ application and the available automated tests were checked successfully. The driver source is prepared for the target Linux environment.

