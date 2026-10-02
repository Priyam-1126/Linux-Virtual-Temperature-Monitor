#include "alert_manager.hpp"
#include "device_monitor.hpp"
#include "logger.hpp"
#include "simulated_sensor.hpp"
#include "temperature_source.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace {

void printHeader() {
    std::cout << "\n==============================================\n"
              << " Linux Virtual Temperature Monitor\n"
              << "==============================================\n";
}

bool parseDouble(const std::string& text, double& value) {
    try {
        std::size_t used = 0;
        value = std::stod(text, &used);
        return used == text.size();
    } catch (...) {
        return false;
    }
}

void showReading(TemperatureSource& source, AlertManager& alerts, Logger& logger) {
    TemperatureReading reading{};
    std::string error;

    if (!source.read(reading, error)) {
        std::cerr << "Read error: " << error << "\n";
        logger.write("READ_ERROR | " + error);
        return;
    }

    const std::string status = alerts.status(reading.celsius);
    std::cout << "Temperature : " << std::fixed << std::setprecision(1)
              << reading.celsius << " C\n"
              << "Source      : " << reading.source << "\n"
              << "Status      : " << status << "\n"
              << "Threshold   : " << alerts.threshold() << " C\n";

    std::ostringstream logMessage;
    logMessage << "READ | temperature=" << std::fixed << std::setprecision(1)
               << reading.celsius << "C | status=" << status
               << " | source=" << reading.source;
    logger.write(logMessage.str());
}

void setReading(TemperatureSource& source, Logger& logger) {
    std::string input;
    std::cout << "Enter new temperature (C): ";
    std::getline(std::cin, input);

    double value = 0.0;
    if (!parseDouble(input, value)) {
        std::cout << "Invalid temperature.\n";
        logger.write("SET_ERROR | invalid numeric input");
        return;
    }

    std::string error;
    if (!source.writeTemperature(value, error)) {
        std::cout << "Unable to set temperature: " << error << "\n";
        logger.write("SET_ERROR | " + error);
        return;
    }

    std::cout << "Temperature updated.\n";
    std::ostringstream logMessage;
    logMessage << "SET | temperature=" << value << "C";
    logger.write(logMessage.str());
}

} // namespace

int main(int argc, char* argv[]) {
    std::string devicePath = "/dev/virtual_temperature";
    bool forceSimulation = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--simulate") {
            forceSimulation = true;
        } else if (arg == "--device" && i + 1 < argc) {
            devicePath = argv[++i];
        } else if (arg == "--help") {
            std::cout << "Usage: temperature_monitor [--simulate] [--device PATH]\n";
            return EXIT_SUCCESS;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return EXIT_FAILURE;
        }
    }

    Logger logger;
    AlertManager alerts(50.0);

    std::unique_ptr<TemperatureSource> source;
    if (!forceSimulation) {
        auto device = std::make_unique<DeviceMonitor>(devicePath);
        if (device->available()) {
            source = std::move(device);
        }
    }

    if (!source) {
        source = std::make_unique<SimulatedSensor>();
        logger.write("START | using simulated sensor");
    } else {
        logger.write("START | using device=" + devicePath);
    }

    printHeader();
    std::cout << "Source: " << source->name() << "\n";

    while (true) {
        std::cout << "\n1. Read temperature\n"
                  << "2. Set temperature\n"
                  << "3. Device status\n"
                  << "4. Exit\n"
                  << "Choose: ";

        std::string choice;
        if (!std::getline(std::cin, choice)) {
            break;
        }

        if (choice == "1") {
            showReading(*source, alerts, logger);
        } else if (choice == "2") {
            setReading(*source, logger);
        } else if (choice == "3") {
            std::cout << "Device source : " << source->name() << "\n"
                      << "Available     : " << (source->available() ? "YES" : "NO") << "\n";
        } else if (choice == "4") {
            logger.write("STOP | application closed");
            break;
        } else {
            std::cout << "Please choose 1, 2, 3 or 4.\n";
        }
    }

    return EXIT_SUCCESS;
}
