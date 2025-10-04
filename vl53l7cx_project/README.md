# VL53L7CX Multizone ToF Sensor Driver for Pico 2

A complete driver implementation for the VL53L7CX multizone Time-of-Flight sensor on the Raspberry Pi Pico 2 using the ARM toolchain.

## Features

- **64-zone distance measurement** (8x8 grid)
- **I2C communication** at 400 kHz
- **Real-time distance data output** via USB serial
- **Status LED indication** for system health
- **Error handling** and diagnostics
- **Optimized for Pico 2** with ARM Cortex-M33

## Hardware Requirements

- Raspberry Pi Pico 2
- VL53L7CX Time-of-Flight sensor module
- Jumper wires for I2C connection
- Pull-up resistors (4.7kΩ) on I2C lines (optional, Pico has internal pull-ups)

## Wiring Diagram

```
Pico 2          VL53L7CX
------          --------
GPIO 4 (SDA) -> SDA
GPIO 5 (SCL) -> SCL
3.3V          -> VCC
GND           -> GND
```

## Project Structure

```
vl53l7cx_project/
├── CMakeLists.txt          # CMake configuration
├── main.c                  # Main application
├── vl53l7cx_driver.h       # Driver header file
├── vl53l7cx_driver.c       # Driver implementation
├── build.sh                # Build script
└── README.md               # This file
```

## Building

### Option 1: Use the build script
```bash
./build.sh
```

### Option 2: Manual build
```bash
rm -rf build
mkdir build && cd build
cmake .. -DPICO_BOARD=pico2 -DPICO_PLATFORM=rp2350-arm-s
make -j8
```

## Programming

1. Put your Pico 2 into BOOTSEL mode (hold BOOTSEL button while connecting USB)
2. Drag and drop the `vl53l7cx_driver.uf2` file to the mounted drive
3. The Pico 2 will automatically restart and start reading sensor data

## Usage

### Serial Output
The driver outputs distance data in a formatted 8x8 grid:

```
=== VL53L7CX Zone Distance Data (8x8 grid) ===
Zone distances in mm:
Row 0:  245  267  289  312  334  356  378  400
Row 1:  223  245  267  289  312  334  356  378
Row 2:  201  223  245  267  289  312  334  356
...
```

### LED Status
- **Solid ON**: System starting up
- **Solid OFF**: Normal operation
- **Fast blink**: Initialization error
- **Slow blink**: Ranging start error
- **Brief flash**: Successful measurement

## Technical Details

### I2C Configuration
- **Address**: 0x29 (default VL53L7CX address)
- **Frequency**: 400 kHz
- **Pins**: GPIO 4 (SDA), GPIO 5 (SCL)

### Measurement Range
- **Minimum**: ~20 mm
- **Maximum**: 4000 mm (4 meters)
- **Resolution**: 1 mm
- **Update rate**: ~10 Hz (depending on configuration)

### Zone Layout
The VL53L7CX provides 64 zones arranged in an 8x8 grid:
```
Zone layout (8x8):
 0  1  2  3  4  5  6  7
 8  9 10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
```

## Troubleshooting

### Common Issues

1. **"Failed to communicate with sensor"**
   - Check I2C wiring (SDA/SCL connections)
   - Verify power supply (3.3V)
   - Ensure pull-up resistors are present
   - Check I2C address (should be 0x29)

2. **"Failed to start ranging"**
   - Sensor may not be properly initialized
   - Check power supply stability
   - Verify I2C communication is working

3. **No distance data or all zeros**
   - Sensor may be too close or too far from target
   - Check for obstructions in sensor field of view
   - Verify sensor is properly mounted

### Debug Tips

- Use a multimeter to verify 3.3V power supply
- Check I2C communication with an I2C scanner
- Monitor USB serial output for error messages
- LED status provides visual feedback on system health

## API Reference

### Main Functions

- `vl53l7cx_init()` - Initialize sensor
- `vl53l7cx_start_ranging()` - Start distance measurements
- `vl53l7cx_stop_ranging()` - Stop distance measurements
- `vl53l7cx_get_data_ready()` - Check if new data is available
- `vl53l7cx_get_zone_data()` - Read distance data from all zones
- `vl53l7cx_print_zone_data()` - Print formatted zone data

## License

This project is provided as-is for educational and development purposes.

## Contributing

Feel free to submit issues and enhancement requests!
