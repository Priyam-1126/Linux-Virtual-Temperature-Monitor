#include "alert_manager.hpp"

AlertManager::AlertManager(double threshold) : threshold_(threshold) {}

bool AlertManager::isWarning(double temperature) const {
    return temperature > threshold_;
}

std::string AlertManager::status(double temperature) const {
    return isWarning(temperature) ? "WARNING" : "NORMAL";
}

double AlertManager::threshold() const {
    return threshold_;
}
