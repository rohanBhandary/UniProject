# Banking System with SDL GUI

A comprehensive banking system built in C++ with a graphical user interface using SDL2. This project demonstrates object-oriented programming principles, GUI development, and banking system functionality.

## Features

### Core Banking Features
- **Account Management**: Create and manage different types of accounts (Savings, Checking, Business)
- **Transaction System**: Deposit, withdraw, and transfer funds between accounts
- **User Authentication**: Secure login/logout system with password hashing
- **Interest Calculation**: Automatic interest calculation for savings accounts
- **Transaction History**: Complete tracking of all banking transactions
- **Account Types**: Support for multiple account types with different features

### GUI Features
- **Modern Interface**: Clean, professional banking interface
- **Responsive Design**: Adapts to different screen sizes
- **Interactive Elements**: Buttons, text fields, and navigation
- **Visual Feedback**: Hover effects, animations, and status messages
- **Multi-screen Navigation**: Login, dashboard, and feature screens

### Technical Features
- **Object-Oriented Design**: Well-structured C++ classes
- **Memory Management**: Smart pointers for automatic memory management
- **Error Handling**: Comprehensive error handling and validation
- **Data Persistence**: File-based data storage (extensible)
- **Cross-platform**: Works on Windows, Linux, and macOS

## System Requirements

### Prerequisites
- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake** 3.16 or higher
- **SDL2** development libraries
- **SDL2_ttf** for text rendering
- **SDL2_image** for image support

### Dependencies
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

# macOS (using Homebrew)
brew install sdl2 sdl2_ttf sdl2_image

# Windows (using vcpkg)
vcpkg install sdl2 sdl2-ttf sdl2-image
```

## Building the Project

### 1. Clone the Repository
```bash
git clone <repository-url>
cd banking-system
```

### 2. Create Build Directory
```bash
mkdir build
cd build
```

### 3. Configure with CMake
```bash
cmake ..
```

### 4. Build the Project
```bash
make -j$(nproc)
# or on Windows with Visual Studio
cmake --build . --config Release
```

### 5. Run the Application
```bash
./BankingSystem
```

## Project Structure

```
banking-system/
├── CMakeLists.txt          # CMake configuration
├── README.md              # This file
├── include/               # Header files
│   ├── Account.h         # Account management
│   ├── Transaction.h     # Transaction handling
│   ├── User.h           # User authentication
│   ├── Bank.h           # Main banking system
│   └── GUI/             # GUI components
│       ├── Window.h     # Main window management
│       ├── Screen.h     # Base screen class
│       ├── Button.h     # Interactive buttons
│       ├── TextField.h  # Input fields
│       ├── LoginScreen.h
│       ├── DashboardScreen.h
│       ├── TransactionScreen.h
│       └── AccountScreen.h
├── src/                  # Source files
│   ├── main.cpp         # Application entry point
│   ├── Account.cpp      # Account implementations
│   ├── Transaction.cpp  # Transaction handling
│   ├── User.cpp         # User management
│   ├── Bank.cpp         # Banking system logic
│   └── GUI/             # GUI implementations
│       ├── Window.cpp
│       ├── Screen.cpp
│       ├── Button.cpp
│       ├── TextField.cpp
│       ├── LoginScreen.cpp
│       ├── DashboardScreen.cpp
│       ├── TransactionScreen.cpp
│       └── AccountScreen.cpp
└── assets/              # Resources (fonts, images)
    └── fonts/
        └── arial.ttf    # Font file (you need to provide this)
```

## Usage

### Default Login Credentials
The system comes with pre-configured test accounts:

**Administrator Account:**
- Username: `admin`
- Password: `admin123`
- Role: Administrator (full access)

**Customer Account:**
- Username: `john`
- Password: `password123`
- Role: Customer (limited access)

### Features Available

#### For Customers:
- View account balances
- Check transaction history
- Transfer funds between accounts
- Update profile information
- Logout functionality

#### For Administrators:
- All customer features
- User management
- Account management
- System statistics
- Admin panel access

## Architecture Overview

### Core Classes

#### Bank Class
- Central banking system management
- User authentication and session management
- Account creation and management
- Transaction processing
- Data persistence

#### Account Classes
- **Account**: Base account class with common functionality
- **SavingsAccount**: Interest-bearing savings accounts
- **CheckingAccount**: Checking accounts with overdraft protection
- **BusinessAccount**: Business accounts with additional features

#### User Class
- User authentication and profile management
- Account ownership and access control
- Role-based permissions

#### Transaction Class
- Transaction recording and history
- Multiple transaction types (deposit, withdrawal, transfer, interest, fees)
- Timestamp and balance tracking

### GUI Architecture

#### Window Class
- SDL2 window and renderer management
- Screen navigation and lifecycle
- Event handling and input processing
- Text rendering and drawing utilities

#### Screen Classes
- **LoginScreen**: User authentication interface
- **DashboardScreen**: Main banking dashboard
- **TransactionScreen**: Transaction history and management
- **AccountScreen**: Account management interface

#### UI Components
- **Button**: Interactive buttons with hover effects
- **TextField**: Text input fields with validation

## Development

### Adding New Features

1. **New Account Types**: Extend the `Account` class hierarchy
2. **New Screens**: Create new screen classes inheriting from `Screen`
3. **New Transactions**: Add transaction types to the `TransactionType` enum
4. **Data Persistence**: Implement serialization in the `Bank` class

### Code Style
- Follow C++17 standards
- Use smart pointers for memory management
- Implement RAII principles
- Use const correctness where appropriate
- Add comprehensive error handling

### Testing
- Unit tests for core banking logic
- Integration tests for GUI components
- Manual testing for user workflows

## Troubleshooting

### Common Issues

#### SDL2 Not Found
```bash
# Install SDL2 development libraries
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```

#### Font Loading Issues
- Ensure `assets/fonts/arial.ttf` exists
- Or modify `Window.cpp` to use system fonts

#### Build Errors
- Ensure C++17 is enabled
- Check all SDL2 dependencies are installed
- Verify CMake version is 3.16+

### Debug Mode
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- SDL2 development team for the excellent multimedia library
- CMake team for the build system
- C++ community for best practices and guidance

## Future Enhancements

- [ ] Database integration (SQLite/PostgreSQL)
- [ ] Network banking features
- [ ] Mobile app companion
- [ ] Advanced security features
- [ ] Multi-language support
- [ ] Reporting and analytics
- [ ] API for third-party integrations
- [ ] Cloud synchronization
- [ ] Advanced UI themes
- [ ] Accessibility features 