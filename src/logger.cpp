#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>

Logger::Logger(const std::string& filePath) : filePath_(filePath) {}

bool Logger::write(const std::string& message) {
    try {
        const std::filesystem::path path(filePath_);
        if (!path.parent_path().empty()) {
            std::filesystem::create_directories(path.parent_path());
        }

        std::ofstream out(filePath_, std::ios::app);
        if (!out) {
            return false;
        }

        const std::time_t now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        std::tm tm{};
        localtime_r(&now, &tm);

        out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " | " << message << '\n';
        return static_cast<bool>(out);
    } catch (...) {
        return false;
    }
}

const std::string& Logger::path() const {
    return filePath_;
}
