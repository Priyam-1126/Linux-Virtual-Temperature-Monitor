CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -pthread
BUILD_DIR := build
TARGET := $(BUILD_DIR)/temperature_monitor

SRC := src/main.cpp \
       src/alert_manager.cpp \
       src/logger.cpp \
       src/simulated_sensor.cpp \
       src/device_monitor.cpp

OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) --simulate

test: $(TARGET)
	$(CXX) $(CXXFLAGS) tests/test_core.cpp src/alert_manager.cpp src/logger.cpp -o $(BUILD_DIR)/test_core
	$(BUILD_DIR)/test_core
	bash tests/integration_test.sh $(TARGET)
	bash tests/device_interface_test.sh $(TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f logs/temperature.log
