#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

class Logger {
public:
    explicit Logger(const std::string& filePath = "logs/temperature.log");

    bool write(const std::string& message);
    const std::string& path() const;

private:
    std::string filePath_;
};

#endif
