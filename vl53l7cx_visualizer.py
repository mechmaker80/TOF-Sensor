#!/usr/bin/env python3
"""
VL53L7CX Real-Time Heat Map Visualizer
=====================================

This application reads distance data from the VL53L7CX sensor via USB serial
and displays it as a real-time 8x8 heat map with color-coded distances.

Requirements:
- pyserial
- matplotlib
- numpy

Install with: pip install pyserial matplotlib numpy
"""

import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import re
import time
from collections import deque
import argparse

class VL53L7CXVisualizer:
    def __init__(self, port='/dev/cu.usbmodem23101', baudrate=115200):
        """
        Initialize the VL53L7CX visualizer
        
        Args:
            port: Serial port (default for macOS)
            baudrate: Serial baud rate
        """
        self.port = port
        self.baudrate = baudrate
        self.serial_conn = None
        self.distance_matrix = np.zeros((8, 8))  # 8x8 distance matrix
        self.max_distance = 2000  # Maximum distance in mm
        self.min_distance = 50    # Minimum distance in mm
        
        # Set up the plot
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        self.im = None
        self.text_annotations = []
        
        # Statistics
        self.measurement_count = 0
        self.last_update_time = time.time()
        self.fps = 0
        
    def connect_serial(self):
        """Connect to the VL53L7CX sensor via serial"""
        try:
            self.serial_conn = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                timeout=1,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.EIGHTBITS
            )
            print(f"✅ Connected to {self.port} at {self.baudrate} baud")
            return True
        except serial.SerialException as e:
            print(f"❌ Failed to connect to {self.port}: {e}")
            return False
    
    def parse_distance_data(self, line):
        """
        Parse distance data from serial line
        
        Expected format:
        Row 0:  245  230  215  200  185  170  155  140
        Row 1:  265  250  235  220  205  190  175  160
        ...
        """
        # Look for row data pattern
        row_match = re.match(r'Row (\d+):\s+(.+)', line.strip())
        if row_match:
            row_num = int(row_match.group(1))
            distances_str = row_match.group(2)
            
            # Extract distance values
            distances = re.findall(r'\d+', distances_str)
            if len(distances) == 8:  # Should have 8 distance values
                for col, dist_str in enumerate(distances):
                    try:
                        distance = int(dist_str)
                        # Filter out invalid distances
                        if self.min_distance <= distance <= self.max_distance:
                            self.distance_matrix[row_num, col] = distance
                        else:
                            self.distance_matrix[row_num, col] = 0  # No object
                    except ValueError:
                        self.distance_matrix[row_num, col] = 0
                return True
        return False
    
    def update_plot(self, frame):
        """Update the heat map plot with new data"""
        if not self.serial_conn or not self.serial_conn.is_open:
            return
        
        # Read serial data
        try:
            while self.serial_conn.in_waiting > 0:
                line = self.serial_conn.readline().decode('utf-8', errors='ignore').strip()
                
                # Look for measurement start
                if "Measurement #" in line:
                    self.measurement_count += 1
                    # Clear the matrix for new measurement
                    self.distance_matrix.fill(0)
                
                # Parse distance data
                self.parse_distance_data(line)
                
                # Look for end of measurement
                if "===============================================" in line:
                    break
                    
        except Exception as e:
            print(f"Serial read error: {e}")
            return
        
        # Update the heat map
        if self.im is None:
            # Create initial heat map
            self.im = self.ax.imshow(
                self.distance_matrix,
                cmap='viridis',
                vmin=0,
                vmax=self.max_distance,
                aspect='equal'
            )
            
            # Add colorbar
            cbar = plt.colorbar(self.im, ax=self.ax)
            cbar.set_label('Distance (mm)', rotation=270, labelpad=20)
            
            # Set up the plot
            self.ax.set_title('VL53L7CX Real-Time Distance Heat Map', fontsize=16, fontweight='bold')
            self.ax.set_xlabel('Column', fontsize=12)
            self.ax.set_ylabel('Row', fontsize=12)
            
            # Set tick labels
            self.ax.set_xticks(range(8))
            self.ax.set_yticks(range(8))
            self.ax.set_xticklabels(range(8))
            self.ax.set_yticklabels(range(8))
            
            # Add grid
            self.ax.grid(True, alpha=0.3)
            
        else:
            # Update existing heat map
            self.im.set_array(self.distance_matrix)
        
        # Clear previous text annotations
        for text in self.text_annotations:
            text.remove()
        self.text_annotations.clear()
        
        # Add distance values as text
        for i in range(8):
            for j in range(8):
                distance = self.distance_matrix[i, j]
                if distance > 0:
                    # Choose text color based on background
                    text_color = 'white' if distance < self.max_distance * 0.5 else 'black'
                    text = self.ax.text(
                        j, i, f'{int(distance)}',
                        ha='center', va='center',
                        color=text_color, fontsize=8, fontweight='bold'
                    )
                    self.text_annotations.append(text)
        
        # Update statistics
        current_time = time.time()
        if current_time - self.last_update_time >= 1.0:  # Update FPS every second
            self.fps = self.measurement_count / (current_time - self.last_update_time)
            self.last_update_time = current_time
            self.measurement_count = 0
        
        # Update title with statistics
        title = f'VL53L7CX Real-Time Distance Heat Map\n'
        title += f'Measurements: {self.measurement_count} | FPS: {self.fps:.1f}'
        self.ax.set_title(title, fontsize=14, fontweight='bold')
        
        return [self.im] + self.text_annotations
    
    def run(self):
        """Run the real-time visualization"""
        print("🚀 Starting VL53L7CX Real-Time Visualizer...")
        print(f"📡 Connecting to {self.port}...")
        
        if not self.connect_serial():
            print("❌ Failed to connect to sensor. Please check:")
            print("   1. Pico 2 is connected and programmed")
            print("   2. Serial port is correct")
            print("   3. No other applications are using the serial port")
            return
        
        print("✅ Connected! Starting visualization...")
        print("📊 Close the plot window to stop the visualizer")
        
        # Create animation
        ani = animation.FuncAnimation(
            self.fig, self.update_plot, interval=100, blit=False
        )
        
        try:
            plt.show()
        except KeyboardInterrupt:
            print("\n🛑 Stopping visualizer...")
        finally:
            if self.serial_conn and self.serial_conn.is_open:
                self.serial_conn.close()
                print("📡 Serial connection closed")

def main():
    """Main function with command line argument parsing"""
    parser = argparse.ArgumentParser(
        description='VL53L7CX Real-Time Heat Map Visualizer',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python vl53l7cx_visualizer.py
  python vl53l7cx_visualizer.py --port /dev/ttyUSB0
  python vl53l7cx_visualizer.py --port COM3 --baudrate 115200
        """
    )
    
    parser.add_argument(
        '--port', '-p',
        default='/dev/cu.usbmodem23101',
        help='Serial port (default: /dev/cu.usbmodem23101)'
    )
    
    parser.add_argument(
        '--baudrate', '-b',
        type=int,
        default=115200,
        help='Serial baud rate (default: 115200)'
    )
    
    args = parser.parse_args()
    
    # Create and run visualizer
    visualizer = VL53L7CXVisualizer(port=args.port, baudrate=args.baudrate)
    visualizer.run()

if __name__ == '__main__':
    main()
