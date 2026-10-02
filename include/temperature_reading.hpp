#ifndef TEMPERATURE_READING_HPP
#define TEMPERATURE_READING_HPP

#include <string>
#include <ctime>

struct TemperatureReading {
    double celsius{};
    std::string source;
    std::time_t timestamp{};
};

#endif
