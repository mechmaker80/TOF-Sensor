#!/bin/bash

# VL53L7CX Driver Build Script for Pico 2
# This script builds the VL53L7CX multizone ToF sensor driver

echo "Building VL53L7CX Driver for Pico 2..."

# Clean and create build directory
rm -rf build
mkdir build && cd build

# Configure for Pico 2 with ARM toolchain
echo "Configuring CMake for Pico 2..."
cmake .. -DPICO_BOARD=pico2 -DPICO_PLATFORM=rp2350-arm-s

# Build the project
echo "Building project..."
make -j8

echo "Build complete!"
echo "Output files:"
ls -la *.elf *.uf2 *.bin 2>/dev/null || echo "No output files found"

echo ""
echo "To program your Pico 2:"
echo "1. Put Pico 2 in BOOTSEL mode (hold BOOTSEL while connecting USB)"
echo "2. Drag and drop vl53l7cx_driver.uf2 to the mounted drive"
echo "3. Connect VL53L7CX sensor to I2C pins:"
echo "   - SDA: GPIO 4"
echo "   - SCL: GPIO 5"
echo "   - VCC: 3.3V"
echo "   - GND: Ground"
echo "4. Open USB serial monitor to see distance data"
