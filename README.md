# VL53L7CX Multizone ToF Sensor for Raspberry Pi Pico 2

A complete implementation of the VL53L7CX multizone Time-of-Flight sensor driver for the Raspberry Pi Pico 2, using the official STMicroelectronics driver adapted for the Pico SDK.

## 🎯 Features

- **8x8 Multizone Distance Sensing** - 64 individual distance measurement zones
- **Official ST Driver** - Uses the professional STMicroelectronics VL53L7CX Ultra Lite Driver
- **Pico 2 Compatible** - Custom platform layer for Pico SDK integration
- **Real-time Data Output** - USB serial output with formatted distance data
- **Complete Build System** - CMake configuration for easy building

## 📁 Project Structure

```
pico2/
├── vl53l7cx_project/           # Main VL53L7CX project
│   ├── main_st_driver.c        # Working ST driver example
│   ├── platform_pico.h/c       # Pico 2 platform layer
│   ├── inc/                    # VL53L7CX API headers
│   ├── src/                    # VL53L7CX API source files
│   ├── CMakeLists.txt          # Build configuration
│   └── STSW-IMG036/           # Original ST driver files
├── main.c                      # Simple blink example
├── CMakeLists.txt              # Root build configuration
└── README.md                   # This file
```

## 🔧 Hardware Requirements

- **Raspberry Pi Pico 2** (RP2350)
- **VL53L7CX Sensor Module** (Pololu or similar)
- **I2C Connections**:
  - SDA: GPIO 4
  - SCL: GPIO 5
  - VCC: 3.3V
  - GND: Ground
- **Pull-up Resistors**: 4.7kΩ on SDA and SCL lines

## 🚀 Quick Start

### 1. Prerequisites

- Raspberry Pi Pico SDK installed
- ARM GCC toolchain
- CMake 3.13+

### 2. Build the Project

```bash
cd vl53l7cx_project
./build.sh
```

Or manually:
```bash
cd vl53l7cx_project
rm -rf build
mkdir build && cd build
cmake .. -DPICO_BOARD=pico2 -DPICO_PLATFORM=rp2350-arm-s
make -j8
```

### 3. Program the Pico 2

1. Put Pico 2 in BOOTSEL mode
2. Drag and drop `st_driver_example.uf2` to the mounted drive

### 4. View Output

Connect to serial monitor:
```bash
screen /dev/cu.usbmodem23101 115200
```

## 📊 Expected Output

```
VL53L7CX ST Driver Example for Pico 2
=====================================
I2C initialized on pins SDA=4, SCL=5
Checking if VL53L7CX sensor is alive...
VL53L7CX sensor detected!
Initializing VL53L7CX sensor...
VL53L7CX ULD ready ! (Version : VL53L7CX_2.0.0)
Setting sensor to 8x8 mode...
Sensor set to 8x8 mode successfully
Starting ranging...
Ranging started successfully!
Reading distance data from all zones (8x8 grid)...
Press Ctrl+C to stop

Measurement #1:
=== VL53L7CX Zone Distance Data (8x8 grid) ===
Zone distances in mm:
Row 0:  245  230  215  200  185  170  155  140
Row 1:  265  250  235  220  205  190  175  160
Row 2:  285  270  255  240  225  210  195  180
...
```

## 🔧 Technical Details

### Platform Layer

The project includes a custom platform layer (`platform_pico.h/c`) that adapts the ST driver from STM32 HAL to Pico SDK:

- **I2C Communication**: Uses Pico SDK `hardware_i2c` functions
- **Timing**: Uses Pico SDK `sleep_ms` for delays
- **Memory Management**: Standard C library functions

### Key Files

- `main_st_driver.c` - Complete working example using ST driver
- `platform_pico.h/c` - Pico 2 platform abstraction layer
- `inc/vl53l7cx_api.h` - ST VL53L7CX API (modified for Pico)
- `src/vl53l7cx_api.c` - ST VL53L7CX implementation

### I2C Configuration

- **Address**: 0x29 (configured for your sensor)
- **Speed**: 400 kHz
- **Pins**: SDA=4, SCL=5

## 🛠️ Troubleshooting

### Sensor Not Detected

1. Check I2C connections (SDA=4, SCL=5)
2. Verify 3.3V power supply
3. Ensure pull-up resistors are present
4. Check I2C address (should be 0x29)

### Build Issues

1. Ensure Pico SDK is properly installed
2. Check CMake version (3.13+)
3. Verify ARM GCC toolchain is available

### No Serial Output

1. Wait 2 seconds after programming for USB initialization
2. Check USB cable connection
3. Verify correct serial port (`/dev/cu.usbmodem23101`)

## 📚 Additional Examples

The project includes several example programs:

- `main_st_driver.c` - Full ST driver example (recommended)
- `vl53l7cx_driver.c` - Custom driver implementation
- `test_serial.c` - Serial communication test
- `simple_blink.c` - Basic LED blink test

## 📄 License

This project includes the official STMicroelectronics VL53L7CX driver, which is licensed under ST's terms. See `LICENSE.txt` files for details.

## 🎉 Success!

If you see the 8x8 distance grid output, congratulations! You have successfully implemented a professional-grade multizone ToF sensor system with your Pico 2.

## 🔗 References

- [STMicroelectronics VL53L7CX Datasheet](https://www.st.com/en/imaging-and-photonics-solutions/vl53l7cx.html)
- [Raspberry Pi Pico 2 Documentation](https://datasheets.raspberrypi.com/pico2/)
- [Pico SDK Documentation](https://datasheets.raspberrypi.com/pico-sdk/)