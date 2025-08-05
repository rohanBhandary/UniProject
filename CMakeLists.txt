cmake_minimum_required(VERSION 3.16)
project(BankingSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(include)

# Fallback for SDL2_ttf and SDL2_image if find_package fails
if(NOT SDL2_TTF_LIBRARIES)
    find_library(SDL2_TTF_LIBRARIES NAMES SDL2_ttf)
endif()

if(NOT SDL2_IMAGE_LIBRARIES)
    find_library(SDL2_IMAGE_LIBRARIES NAMES SDL2_image)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Bank.cpp
    src/Account.cpp
    src/Transaction.cpp
    src/User.cpp
    src/GUI/Screen.cpp
    src/GUI/Window.cpp
    src/GUI/Button.cpp
    src/GUI/TextField.cpp
    src/GUI/LoginScreen.cpp
    src/GUI/DashboardScreen.cpp
    src/GUI/TransactionScreen.cpp
    src/GUI/TransferScreen.cpp
    src/GUI/AdminPanel.cpp
    src/GUI/AccountScreen.cpp
)

# Create executable
add_executable(BankingSystem ${SOURCES})

# Link libraries
target_link_libraries(BankingSystem 
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    SDL2_ttf
    SDL2_image
)

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}) 