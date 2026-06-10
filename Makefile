.PHONY: configure build run clean rebuild

BUILD_DIR := build/vcpkg
TARGET := $(BUILD_DIR)/cpp-vampire

configure:
	cmake --preset default-local

build:
	cmake --build --preset default-local

run:
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean configure build
