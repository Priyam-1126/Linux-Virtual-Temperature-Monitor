CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -MMD -MP
BUILD_DIR := build
TARGET := $(BUILD_DIR)/temperature_monitor
TEST_BIN := $(BUILD_DIR)/test_core

APP_SRC := src/alert_manager.cpp \
           src/logger.cpp \
           src/simulated_sensor.cpp \
           src/device_monitor.cpp

SRC := src/main.cpp $(APP_SRC)
OBJ := $(SRC:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

# -MMD -MP makes g++ write .d files, so changing a header rebuilds the right files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN): tests/test_core.cpp $(APP_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET) --simulate

test: $(TARGET) $(TEST_BIN)
	bash tests/run_tests.sh

clean:
	rm -rf $(BUILD_DIR)

-include $(OBJ:.o=.d)
