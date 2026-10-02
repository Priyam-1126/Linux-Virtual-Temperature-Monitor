#include "logger.hpp"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

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

        const auto now = std::chrono::system_clock::now();
        const std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &time);
#else
        localtime_r(&time, &tm);
#endif
        out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " | " << message << '\n';
        return static_cast<bool>(out);
    } catch (...) {
        return false;
    }
}

const std::string& Logger::path() const {
    return filePath_;
}
