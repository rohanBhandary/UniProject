@echo off
REM Banking System Build Script for Windows
REM This script automates the build process for the banking system

echo === Banking System Build Script ===

REM Check if CMake is installed
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: CMake is not installed. Please install CMake first.
    pause
    exit /b 1
)

REM Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Navigate to build directory
cd build

REM Configure the project
echo Configuring project with CMake...
cmake ..

if %errorlevel% neq 0 (
    echo Error: CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo Error: Build failed!
    pause
    exit /b 1
)

echo === Build completed successfully! ===
echo You can now run the application with: BankingSystem.exe
echo.
echo Default login credentials:
echo   Username: admin, Password: admin123
echo   Username: john, Password: password123
pause 