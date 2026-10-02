#include "alert_manager.hpp"
#include "logger.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    AlertManager alerts(50.0);
    assert(!alerts.isWarning(50.0));
    assert(alerts.isWarning(50.1));
    assert(alerts.status(25.0) == "NORMAL");
    assert(alerts.status(55.0) == "WARNING");

    const std::string path = "build/test.log";
    std::filesystem::remove(path);
    Logger logger(path);
    assert(logger.write("TEST_EVENT"));

    std::ifstream in(path);
    std::string line;
    std::getline(in, line);
    assert(line.find("TEST_EVENT") != std::string::npos);

    return 0;
}
