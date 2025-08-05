# Installation Guide - Banking System

This guide will help you install and set up the Banking System with SDL GUI.

## Prerequisites

### System Requirements
- **Operating System**: Windows 10+, macOS 10.14+, or Linux (Ubuntu 18.04+)
- **Compiler**: C++17 compatible compiler
- **Memory**: At least 4GB RAM
- **Storage**: 100MB free space

### Required Software

#### 1. C++ Compiler
- **Windows**: Visual Studio 2019/2022 or MinGW-w64
- **macOS**: Xcode Command Line Tools
- **Linux**: GCC 7+ or Clang 5+

#### 2. CMake
- Version 3.16 or higher
- Download from: https://cmake.org/download/

#### 3. SDL2 Libraries

**Windows:**
```cmd
# Using vcpkg (recommended)
vcpkg install sdl2 sdl2-ttf sdl2-image

# Or download from SDL2 website
# https://www.libsdl.org/download-2.0.php
```

**macOS:**
```bash
# Using Homebrew
brew install sdl2 sdl2-ttf sdl2-image

# Or using MacPorts
sudo port install libsdl2 libsdl2-ttf libsdl2-image
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```

**Linux (Fedora):**
```bash
sudo dnf install SDL2-devel SDL2_ttf-devel SDL2_image-devel
```

## Installation Steps

### Step 1: Clone or Download the Project
```bash
git clone <repository-url>
cd banking-system
```

### Step 2: Install Dependencies

#### Windows
1. Install Visual Studio 2019/2022 with C++ workload
2. Install CMake
3. Install SDL2 using vcpkg:
   ```cmd
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg install sdl2 sdl2-ttf sdl2-image
   ```

#### macOS
1. Install Xcode Command Line Tools:
   ```bash
   xcode-select --install
   ```
2. Install Homebrew (if not already installed):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
3. Install SDL2:
   ```bash
   brew install sdl2 sdl2-ttf sdl2-image
   ```

#### Linux
1. Update package manager:
   ```bash
   sudo apt-get update  # Ubuntu/Debian
   # or
   sudo dnf update      # Fedora
   ```
2. Install SDL2:
   ```bash
   sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
   ```

### Step 3: Build the Project

#### Using the Build Script (Recommended)

**Windows:**
```cmd
build.bat
```

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh
```

#### Manual Build

1. Create build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Configure with CMake:
   ```bash
   cmake ..
   ```

3. Build the project:
   ```bash
   # Linux/macOS
   make -j$(nproc)
   
   # Windows
   cmake --build . --config Release
   ```

### Step 4: Run the Application

**Linux/macOS:**
```bash
./BankingSystem
```

**Windows:**
```cmd
BankingSystem.exe
```

## Configuration

### Font Configuration
The application requires a TTF font file. By default, it looks for `assets/fonts/arial.ttf`.

1. Create the fonts directory:
   ```bash
   mkdir -p assets/fonts
   ```

2. Copy a TTF font file to `assets/fonts/arial.ttf`

   **Windows:** Copy from `C:\Windows\Fonts\arial.ttf`
   
   **macOS:** Copy from `/System/Library/Fonts/Arial.ttf`
   
   **Linux:** Install and copy a font:
   ```bash
   sudo apt-get install fonts-liberation
   cp /usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf assets/fonts/arial.ttf
   ```

### Data Directory
The application creates a `data` directory for storing user and transaction data:
```bash
mkdir data
```

## Troubleshooting

### Common Issues

#### 1. SDL2 Not Found
**Error:** `Could not find SDL2`
**Solution:** Install SDL2 development libraries for your platform.

#### 2. Font Loading Error
**Error:** `Could not load font files`
**Solution:** Ensure `assets/fonts/arial.ttf` exists and is readable.

#### 3. CMake Configuration Fails
**Error:** `CMake configuration failed`
**Solution:** 
- Check CMake version: `cmake --version`
- Ensure all dependencies are installed
- Try clearing build directory: `rm -rf build`

#### 4. Build Errors
**Error:** `Build failed`
**Solution:**
- Check compiler version supports C++17
- Ensure all SDL2 libraries are installed
- Check for missing dependencies

#### 5. Runtime Errors
**Error:** `SDL could not initialize`
**Solution:**
- Install SDL2 runtime libraries
- Check display drivers
- Try running in compatibility mode

### Platform-Specific Issues

#### Windows
- **Visual Studio not found:** Install Visual Studio with C++ workload
- **SDL2.dll missing:** Copy SDL2.dll to executable directory
- **Font rendering issues:** Install Microsoft Visual C++ Redistributable

#### macOS
- **Xcode not found:** Install Xcode Command Line Tools
- **SDL2 framework issues:** Use Homebrew installation
- **Permission errors:** Check file permissions

#### Linux
- **Package not found:** Update package lists
- **Library linking errors:** Install development packages
- **Display issues:** Check X11/Wayland compatibility

## Verification

After successful installation, you should be able to:

1. **Launch the application** and see the login screen
2. **Log in** with default credentials:
   - Username: `admin`, Password: `admin123`
   - Username: `john`, Password: `password123`
3. **Navigate** through the banking interface
4. **View account information** and balances
5. **Log out** and return to login screen

## Support

If you encounter issues:

1. Check the troubleshooting section above
2. Verify all dependencies are installed correctly
3. Check the console output for error messages
4. Ensure your system meets the minimum requirements
5. Try building in debug mode for more detailed error information

## Uninstallation

To remove the Banking System:

1. **Delete the project directory:**
   ```bash
   rm -rf banking-system
   ```

2. **Remove build artifacts:**
   ```bash
   rm -rf build
   ```

3. **Remove data files (optional):**
   ```bash
   rm -rf data
   ```

Note: SDL2 libraries installed via package managers should be kept as they may be used by other applications. 