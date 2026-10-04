// Unit tests for the C++ classes. Uses plain assert(): no test library needed.
#include "alert_manager.hpp"
#include "device_monitor.hpp"
#include "logger.hpp"
#include "simulated_sensor.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

static void testAlertManager() {
    AlertManager alerts(50.0);
    assert(!alerts.isWarning(50.0));   // exactly 50 is NORMAL
    assert(alerts.isWarning(50.1));    // above 50 is WARNING
    assert(alerts.status(25.0) == "NORMAL");
    assert(alerts.status(55.0) == "WARNING");
    assert(alerts.threshold() == 50.0);
}

static void testLogger() {
    const std::string path = "build/test.log";
    std::filesystem::remove(path);
    Logger logger(path);
    assert(logger.write("TEST_EVENT"));

    std::ifstream in(path);
    std::string line;
    std::getline(in, line);
    assert(line.find("TEST_EVENT") != std::string::npos);
}

static void testSimulatedSensor() {
    SimulatedSensor sensor;
    TemperatureReading reading{};
    std::string error;

    assert(sensor.available());
    assert(sensor.read(reading, error));
    assert(reading.celsius == 28.0);                  // default value
    assert(reading.source == "SIMULATED_SENSOR");

    assert(sensor.writeTemperature(65.0, error));
    assert(sensor.read(reading, error));
    assert(reading.celsius == 65.0);

    assert(!sensor.writeTemperature(151.0, error));   // out of range
    assert(!sensor.writeTemperature(-51.0, error));
    assert(sensor.read(reading, error));
    assert(reading.celsius == 65.0);                  // unchanged after error
}

static void testDeviceMonitorWithFile() {
    // A normal file is used as a stand-in for /dev/virtual_temperature.
    const std::string path = "build/unit_fake_device";
    std::filesystem::remove(path);
    { std::ofstream(path) << "41\n"; }

    DeviceMonitor device(path);
    TemperatureReading reading{};
    std::string error;

    assert(device.available());
    assert(device.read(reading, error));
    assert(reading.celsius == 41.0);
    assert(reading.source == "LINUX_CHARACTER_DEVICE");

    assert(device.writeTemperature(65.0, error));
    assert(device.read(reading, error));
    assert(reading.celsius == 65.0);

    assert(!device.writeTemperature(200.0, error));   // rejected before writing

    { std::ofstream(path, std::ios::trunc) << "abc\n"; }
    assert(!device.read(reading, error));             // not a number
    assert(error.find("invalid") != std::string::npos);

    DeviceMonitor missing("build/does_not_exist");
    assert(!missing.available());
    assert(!missing.read(reading, error));
}

int main() {
    testAlertManager();
    testLogger();
    testSimulatedSensor();
    testDeviceMonitorWithFile();
    std::cout << "All unit tests passed.\n";
    return 0;
}
