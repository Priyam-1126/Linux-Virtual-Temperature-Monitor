#ifndef DEVICE_MONITOR_HPP
#define DEVICE_MONITOR_HPP

#include "temperature_source.hpp"
#include <memory>

class DeviceMonitor final : public TemperatureSource {
public:
    explicit DeviceMonitor(const std::string& devicePath);
    ~DeviceMonitor() override;

    bool read(TemperatureReading& reading, std::string& error) override;
    bool writeTemperature(double temperature, std::string& error) override;
    bool available() const override;
    std::string name() const override;
    const std::string& devicePath() const;

private:
    std::string devicePath_;
    mutable int fd_;

    bool ensureOpen(std::string& error) const;
};

#endif
