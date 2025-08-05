#include <iostream>
#include <memory>
#include "Bank.h"
#include "GUI/Window.h"
#include "GUI/LoginScreen.h"

int main(int argc, char* argv[]) {
    try {
        // Initialize the bank
        auto bank = std::make_shared<Bank>("SecureBank", "SB001");
        
        // Initialize the GUI window
        auto window = std::make_shared<Window>("Banking System", 1200, 800);
        if (!window->initialize()) {
            std::cerr << "Failed to initialize window!" << std::endl;
            return 1;
        }
        
        // Set initial screen to login
        auto loginScreen = std::make_shared<LoginScreen>(window, bank);
        loginScreen->initialize();  // Initialize the screen
        window->setScreen(loginScreen);
        
        // Print default credentials
        std::cout << "=== Banking System ===" << std::endl;
        std::cout << "Default login credentials:" << std::endl;
        std::cout << "  Admin: username=admin, password=admin123" << std::endl;
        std::cout << "  Customer: username=john, password=password123" << std::endl;
        std::cout << "=====================" << std::endl;
        
        // Start the main loop
        std::cout << "Starting main loop..." << std::endl;
        window->run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
} 