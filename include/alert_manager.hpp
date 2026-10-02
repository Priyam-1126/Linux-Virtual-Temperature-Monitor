#ifndef ALERT_MANAGER_HPP
#define ALERT_MANAGER_HPP

#include "temperature_reading.hpp"
#include <string>

class AlertManager {
public:
    explicit AlertManager(double threshold = 50.0);

    bool isWarning(double temperature) const;
    std::string status(double temperature) const;
    double threshold() const;

private:
    double threshold_;
};

#endif
