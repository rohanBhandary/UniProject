#include "GUI/LoginScreen.h"
#include "GUI/DashboardScreen.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include "Bank.h"
#include <iostream>

LoginScreen::LoginScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Banking System - Login"), bank(bankSystem), showError(false), errorTimer(0) {
}

void LoginScreen::initialize() {
    std::cout << "LoginScreen::initialize() called" << std::endl;
    setupUI();
    std::cout << "LoginScreen::initialize() completed" << std::endl;
}

void LoginScreen::render() {
    Screen::render();
    
    std::cout << "LoginScreen::render() called" << std::endl;
    
    // Render additional elements
    window->renderCenteredText("Welcome to " + bank->getBankName(), 100, window->getLargeFont(), window->getPrimaryColor());
    window->renderCenteredText("Please enter your credentials", 130, window->getFont(), window->getTextColor());
    
    // Render error message if any
    if (showError) {
        renderError();
    }
    
    // Render bank logo or branding
    window->renderCenteredText("Secure Banking System", window->getHeight() - 50, window->getSmallFont(), window->getSecondaryColor());
}

void LoginScreen::handleEvent(const SDL_Event& event) {
    Screen::handleEvent(event);
    
    // Handle mouse movement for button hover effects
    if (event.type == SDL_MOUSEMOTION) {
        for (auto& button : buttons) {
            button->handleMouseMove(event.motion.x, event.motion.y);
        }
    }
    
    // Handle mouse button release
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            for (auto& button : buttons) {
                button->handleMouseRelease(event.button.x, event.button.y);
            }
        }
    }
}

void LoginScreen::update() {
    // Update error timer
    if (showError && errorTimer > 0) {
        errorTimer--;
        if (errorTimer <= 0) {
            clearError();
        }
    }
}

void LoginScreen::onEnter() {
    // Clear any previous state
    if (usernameField) usernameField->clearText();
    if (passwordField) passwordField->clearText();
    clearError();
    
    // Focus on username field
    if (usernameField) {
        usernameField->setFocused(true);
    }
}

void LoginScreen::onExit() {
    // Clear sensitive data
    if (passwordField) passwordField->clearText();
}

void LoginScreen::setupUI() {
    std::cout << "LoginScreen::setupUI() called" << std::endl;
    
    int centerX = window->getWidth() / 2;
    int startY = 200;
    int fieldWidth = 300;
    int fieldHeight = 40;
    int buttonWidth = 120;
    int buttonHeight = 40;
    int spacing = 20;
    
    std::cout << "Creating UI elements at centerX=" << centerX << ", startY=" << startY << std::endl;
    
    // Create text fields
    usernameField = std::make_shared<TextField>(centerX - fieldWidth/2, startY, fieldWidth, fieldHeight, "username", "Username");
    passwordField = std::make_shared<TextField>(centerX - fieldWidth/2, startY + fieldHeight + spacing, fieldWidth, fieldHeight, "password", "Password");
    passwordField->setPassword(true);
    
    // Create buttons
    loginButton = std::make_shared<Button>(centerX - buttonWidth - spacing/2, startY + 2*(fieldHeight + spacing), buttonWidth, buttonHeight, "Login", "login");
    registerButton = std::make_shared<Button>(centerX + spacing/2, startY + 2*(fieldHeight + spacing), buttonWidth, buttonHeight, "Register", "register");
    exitButton = std::make_shared<Button>(centerX - buttonWidth/2, startY + 3*(fieldHeight + spacing), buttonWidth, buttonHeight, "Exit", "exit");
    
    // Set up callbacks
    loginButton->setOnClick([this]() { handleLogin(); });
    registerButton->setOnClick([this]() { handleRegister(); });
    exitButton->setOnClick([this]() { handleExit(); });
    
    // Add UI elements
    addTextField(usernameField);
    addTextField(passwordField);
    addButton(loginButton);
    addButton(registerButton);
    addButton(exitButton);
    
    std::cout << "LoginScreen::setupUI() completed - added " << buttons.size() << " buttons and " << textFields.size() << " text fields" << std::endl;
}

void LoginScreen::handleLogin() {
    if (!usernameField || !passwordField) return;
    
    std::string username = usernameField->getText();
    std::string password = passwordField->getText();
    
    if (username.empty() || password.empty()) {
        showErrorMessage("Please enter both username and password");
        return;
    }
    
    if (bank->authenticateUser(username, password)) {
        // Login successful - switch to dashboard
        auto dashboard = std::make_shared<DashboardScreen>(window, bank);
        dashboard->initialize();
        window->setScreen(dashboard);
    } else {
        showErrorMessage("Invalid username or password");
        passwordField->clearText();
    }
}

void LoginScreen::handleRegister() {
    // For now, just show a message
    showErrorMessage("Registration feature coming soon!");
}

void LoginScreen::handleExit() {
    window->quit();
}

void LoginScreen::showErrorMessage(const std::string& message) {
    errorMessage = message;
    showError = true;
    errorTimer = 180; // Show for 3 seconds at 60 FPS
}

void LoginScreen::clearError() {
    showError = false;
    errorMessage.clear();
}

void LoginScreen::renderError() {
    if (!errorMessage.empty()) {
        window->renderCenteredText(errorMessage, 350, window->getFont(), window->getErrorColor());
    }
} 