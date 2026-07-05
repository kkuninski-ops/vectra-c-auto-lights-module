CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror

TEST_BIN := build/controller_test

.PHONY: test clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): test/controller_test.cpp src/AutoLightsController.cpp src/FromStartTrip.cpp src/AutoLightsController.h src/FromStartTrip.h include/Config.h include/VehicleState.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Iinclude -Isrc test/controller_test.cpp src/AutoLightsController.cpp src/FromStartTrip.cpp -o $(TEST_BIN)

clean:
	rm -rf build
