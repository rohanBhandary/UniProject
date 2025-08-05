#!/bin/bash

# Banking System Build Script
# This script automates the build process for the banking system

echo "=== Banking System Build Script ==="

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake first."
    exit 1
fi

# Check if make is installed
if ! command -v make &> /dev/null; then
    echo "Error: Make is not installed. Please install Make first."
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Navigate to build directory
cd build

# Configure the project
echo "Configuring project with CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed!"
    exit 1
fi

# Build the project
echo "Building project..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "Error: Build failed!"
    exit 1
fi

echo "=== Build completed successfully! ==="
echo "You can now run the application with: ./build/BankingSystem"
echo ""
echo "Default login credentials:"
echo "  Username: admin, Password: admin123"
echo "  Username: john, Password: password123" 