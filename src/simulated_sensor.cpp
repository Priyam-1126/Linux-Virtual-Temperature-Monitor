#include "simulated_sensor.hpp"
#include <ctime>

SimulatedSensor::SimulatedSensor(double initialTemperature)
    : temperature_(initialTemperature) {}

bool SimulatedSensor::read(TemperatureReading& reading, std::string& error) {
    error.clear();
    reading.celsius = temperature_;
    reading.source = name();
    reading.timestamp = std::time(nullptr);
    return true;
}

bool SimulatedSensor::writeTemperature(double temperature, std::string& error) {
    if (temperature < -50.0 || temperature > 150.0) {
        error = "temperature must be between -50 C and 150 C";
        return false;
    }
    temperature_ = temperature;
    error.clear();
    return true;
}

bool SimulatedSensor::available() const {
    return true;
}

std::string SimulatedSensor::name() const {
    return "SIMULATED_SENSOR";
}
