#include "device_monitor.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>

DeviceMonitor::DeviceMonitor(const std::string& devicePath)
    : devicePath_(devicePath), fd_(-1) {}

DeviceMonitor::~DeviceMonitor() {
    if (fd_ >= 0) {
        close(fd_);
        fd_ = -1;
    }
}

bool DeviceMonitor::ensureOpen(std::string& error) const {
    if (fd_ >= 0) {
        return true;
    }

    fd_ = open(devicePath_.c_str(), O_RDWR);
    if (fd_ < 0) {
        error = std::string("cannot open ") + devicePath_ + ": " + std::strerror(errno);
        return false;
    }
    return true;
}

bool DeviceMonitor::read(TemperatureReading& reading, std::string& error) {
    if (!ensureOpen(error)) {
        return false;
    }

    char buffer[64]{};
    const ssize_t count = ::read(fd_, buffer, sizeof(buffer) - 1);
    if (count < 0) {
        error = std::string("read failed: ") + std::strerror(errno);
        return false;
    }

    buffer[count] = '\0';
    char* end = nullptr;
    const double value = std::strtod(buffer, &end);
    if (end == buffer) {
        error = "device returned an invalid temperature";
        return false;
    }

    reading.celsius = value;
    reading.source = name();
    reading.timestamp = std::time(nullptr);
    return true;
}

bool DeviceMonitor::writeTemperature(double temperature, std::string& error) {
    if (temperature < -50.0 || temperature > 150.0) {
        error = "temperature must be between -50 C and 150 C";
        return false;
    }

    if (!ensureOpen(error)) {
        return false;
    }

    const std::string payload = std::to_string(temperature);
    const ssize_t count = ::write(fd_, payload.c_str(), payload.size());
    if (count < 0 || static_cast<std::size_t>(count) != payload.size()) {
        error = std::string("write failed: ") + std::strerror(errno);
        return false;
    }

    return true;
}

bool DeviceMonitor::available() const {
    std::string ignored;
    if (fd_ >= 0) {
        return true;
    }
    const int testFd = open(devicePath_.c_str(), O_RDWR);
    if (testFd < 0) {
        return false;
    }
    close(testFd);
    return true;
}

std::string DeviceMonitor::name() const {
    return "LINUX_CHARACTER_DEVICE";
}

const std::string& DeviceMonitor::devicePath() const {
    return devicePath_;
}
