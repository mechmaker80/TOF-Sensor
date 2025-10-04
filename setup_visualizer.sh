#!/bin/bash

echo "🐍 Setting up VL53L7CX Python Visualizer..."
echo "=========================================="

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    echo "❌ Python 3 is not installed. Please install Python 3 first."
    exit 1
fi

echo "✅ Python 3 found: $(python3 --version)"

# Check if pip is installed
if ! command -v pip3 &> /dev/null; then
    echo "❌ pip3 is not installed. Please install pip3 first."
    exit 1
fi

echo "✅ pip3 found"

# Install required packages
echo "📦 Installing required Python packages..."
pip3 install -r requirements.txt

if [ $? -eq 0 ]; then
    echo "✅ All packages installed successfully!"
    echo ""
    echo "🚀 Setup complete! You can now run the visualizer:"
    echo "   python3 vl53l7cx_visualizer.py"
    echo ""
    echo "📡 Make sure your Pico 2 is connected and running the ST driver example"
    echo "   before starting the visualizer."
else
    echo "❌ Failed to install packages. Please check your Python/pip installation."
    exit 1
fi
