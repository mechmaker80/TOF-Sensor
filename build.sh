#!/bin/bash

# Pico 2 Blink Example Build Script
# This script builds the project for Pico 2 using ARM toolchain

echo "Building Pico 2 Blink Example..."

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
