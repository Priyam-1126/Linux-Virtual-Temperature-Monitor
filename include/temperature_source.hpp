#ifndef TEMPERATURE_SOURCE_HPP
#define TEMPERATURE_SOURCE_HPP

#include "temperature_reading.hpp"
#include <string>

class TemperatureSource {
public:
    virtual ~TemperatureSource() = default;
    virtual bool read(TemperatureReading& reading, std::string& error) = 0;
    virtual bool writeTemperature(double temperature, std::string& error) = 0;
    virtual bool available() const = 0;
    virtual std::string name() const = 0;
};

#endif
