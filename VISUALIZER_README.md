# VL53L7CX Real-Time Heat Map Visualizer

A Python application that creates a beautiful real-time heat map visualization of your VL53L7CX multizone ToF sensor data.

## 🎯 Features

- **Real-time 8x8 Heat Map** - Live visualization of all 64 distance zones
- **Color-coded Distances** - Different colors for different distance ranges
- **Distance Values Overlay** - Shows exact distance values on each zone
- **Performance Statistics** - FPS counter and measurement statistics
- **Cross-platform** - Works on macOS, Linux, and Windows
- **Easy Setup** - Simple installation and configuration

## 📊 What You'll See

The visualizer displays:
- **8x8 Grid** representing your VL53L7CX sensor zones
- **Color Heat Map** where:
  - 🔴 **Red/Dark** = Close objects (50-200mm)
  - 🟡 **Yellow/Medium** = Medium distance (200-1000mm)  
  - 🟢 **Green/Light** = Far objects (1000-2000mm)
  - ⚫ **Black** = No object detected
- **Distance Values** overlaid on each zone
- **Real-time Updates** as new measurements arrive

## 🚀 Quick Start

### 1. Prerequisites

- **Python 3.7+** installed
- **Pico 2** running the ST driver example
- **USB connection** between Pico 2 and computer

### 2. Setup

```bash
# Run the setup script
./setup_visualizer.sh

# Or install manually
pip3 install pyserial matplotlib numpy
```

### 3. Run the Visualizer

```bash
# Start the visualizer
python3 vl53l7cx_visualizer.py
```

## 🔧 Configuration

### Serial Port Settings

**macOS (default):**
```bash
python3 vl53l7cx_visualizer.py --port /dev/cu.usbmodem23101
```

**Linux:**
```bash
python3 vl53l7cx_visualizer.py --port /dev/ttyUSB0
```

**Windows:**
```bash
python3 vl53l7cx_visualizer.py --port COM3
```

### Custom Baud Rate
```bash
python3 vl53l7cx_visualizer.py --baudrate 115200
```

## 📱 Usage

1. **Start your Pico 2** with the ST driver example running
2. **Connect to serial monitor** to verify data is flowing
3. **Close the serial monitor** (only one app can use the port)
4. **Run the Python visualizer**
5. **Watch the magic happen!** 🎉

## 🎨 Customization

### Distance Range
Edit the visualizer to change distance ranges:
```python
self.max_distance = 2000  # Maximum distance in mm
self.min_distance = 50    # Minimum distance in mm
```

### Color Scheme
Change the colormap for different visual styles:
```python
# Available options: 'viridis', 'plasma', 'inferno', 'magma', 'jet', 'hot', 'cool'
self.im = self.ax.imshow(self.distance_matrix, cmap='viridis', ...)
```

### Update Rate
Adjust the refresh rate:
```python
ani = animation.FuncAnimation(self.fig, self.update_plot, interval=100, ...)
# interval=100 means 100ms between updates (10 FPS)
```

## 🛠️ Troubleshooting

### "Failed to connect to serial port"
- Check that Pico 2 is connected and programmed
- Verify the correct serial port (use `ls /dev/cu.*` on macOS)
- Make sure no other application is using the serial port
- Try different port numbers (e.g., `/dev/cu.usbmodem23102`)

### "No data received"
- Ensure the ST driver example is running on Pico 2
- Check that the sensor is working (LED should be blinking)
- Verify the sensor is connected properly
- Try restarting the Pico 2

### "Import errors"
- Make sure all required packages are installed: `pip3 install -r requirements.txt`
- Check Python version: `python3 --version` (should be 3.7+)

### "Plot window not updating"
- Check that the Pico 2 is sending data
- Look for error messages in the terminal
- Try increasing the update interval

## 📊 Data Format

The visualizer expects data in this format from your Pico 2:
```
Measurement #1:
=== VL53L7CX Zone Distance Data (8x8 grid) ===
Zone distances in mm:
Row 0:  245  230  215  200  185  170  155  140
Row 1:  265  250  235  220  205  190  175  160
Row 2:  285  270  255  240  225  210  195  180
...
===============================================
```

## 🎯 Advanced Features

### Recording Data
You can modify the visualizer to save data:
```python
# Add to the update_plot function
if self.measurement_count % 10 == 0:  # Save every 10th measurement
    np.savetxt(f'measurement_{self.measurement_count}.csv', 
               self.distance_matrix, delimiter=',')
```

### Multiple Sensors
Extend the visualizer to handle multiple VL53L7CX sensors by:
- Opening multiple serial ports
- Creating multiple subplots
- Synchronizing data from different sensors

### 3D Visualization
Add 3D visualization using matplotlib's 3D plotting:
```python
from mpl_toolkits.mplot3d import Axes3D
# Create 3D surface plot of distance data
```

## 🎉 Enjoy Your Real-Time Sensor Visualization!

This visualizer transforms your VL53L7CX sensor into a powerful real-time distance mapping tool. Perfect for:
- **Robotics applications**
- **Object detection systems**
- **Distance measurement projects**
- **Educational demonstrations**
- **Research and development**

## 📄 License

This visualizer is part of the VL53L7CX Pico 2 project and follows the same licensing terms.
