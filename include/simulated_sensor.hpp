#ifndef SIMULATED_SENSOR_HPP
#define SIMULATED_SENSOR_HPP

#include "temperature_source.hpp"

class SimulatedSensor final : public TemperatureSource {
public:
    explicit SimulatedSensor(double initialTemperature = 28.0);

    bool read(TemperatureReading& reading, std::string& error) override;
    bool writeTemperature(double temperature, std::string& error) override;
    bool available() const override;
    std::string name() const override;

private:
    double temperature_;
};

#endif
