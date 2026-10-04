#ifndef DEVICE_MONITOR_HPP
#define DEVICE_MONITOR_HPP

#include "temperature_source.hpp"
#include <string>

// Reads/writes the temperature through a Linux device file such as
// /dev/virtual_temperature. The file is opened for every operation
// (open -> read/write -> close), which is the normal way to use a simple character device.
class DeviceMonitor final : public TemperatureSource {
public:
    explicit DeviceMonitor(const std::string& devicePath);

    bool read(TemperatureReading& reading, std::string& error) override;
    bool writeTemperature(double temperature, std::string& error) override;
    bool available() const override;
    std::string name() const override;
    const std::string& devicePath() const;

    // Tries to open the device and reports the reason if it fails.
    bool probe(std::string& error) const;

private:
    std::string devicePath_;
};

#endif
