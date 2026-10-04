#include "device_monitor.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

namespace {

// Small helper: closes the file descriptor automatically (RAII).
class FileDescriptor {
public:
    explicit FileDescriptor(int fd) : fd_(fd) {}
    ~FileDescriptor() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }
    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    bool valid() const { return fd_ >= 0; }
    int get() const { return fd_; }

private:
    int fd_;
};

} // namespace

DeviceMonitor::DeviceMonitor(const std::string& devicePath)
    : devicePath_(devicePath) {}

bool DeviceMonitor::read(TemperatureReading& reading, std::string& error) {
    FileDescriptor fd(open(devicePath_.c_str(), O_RDONLY));
    if (!fd.valid()) {
        error = "cannot open " + devicePath_ + ": " + std::strerror(errno);
        return false;
    }

    char buffer[64]{};
    const ssize_t count = ::read(fd.get(), buffer, sizeof(buffer) - 1);
    if (count < 0) {
        error = std::string("read failed: ") + std::strerror(errno);
        return false;
    }
    if (count == 0) {
        error = "device returned no data";
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

    FileDescriptor fd(open(devicePath_.c_str(), O_WRONLY));
    if (!fd.valid()) {
        error = "cannot open " + devicePath_ + ": " + std::strerror(errno);
        return false;
    }

    // The driver reads a whole number (kstrtol), so we send an integer
    // such as "65\n" and not "65.000000".
    const std::string payload = std::to_string(std::lround(temperature)) + "\n";
    const ssize_t count = ::write(fd.get(), payload.c_str(), payload.size());
    if (count < 0) {
        error = std::string("write failed: ") + std::strerror(errno);
        return false;
    }
    if (static_cast<std::size_t>(count) != payload.size()) {
        error = "write was incomplete";
        return false;
    }
    return true;
}

bool DeviceMonitor::probe(std::string& error) const {
    FileDescriptor fd(open(devicePath_.c_str(), O_RDONLY));
    if (!fd.valid()) {
        error = std::strerror(errno);
        return false;
    }
    return true;
}

bool DeviceMonitor::available() const {
    std::string ignored;
    return probe(ignored);
}

std::string DeviceMonitor::name() const {
    return "LINUX_CHARACTER_DEVICE";
}

const std::string& DeviceMonitor::devicePath() const {
    return devicePath_;
}
