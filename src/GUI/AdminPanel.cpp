#include "GUI/AdminPanel.h"
#include "GUI/DashboardScreen.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include "Bank.h"
#include <iostream>
#include <sstream>
#include <iomanip>

AdminPanel::AdminPanel(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Admin Panel"), bank(bankSystem), messageVisible(false), messageTimer(0) {
}

void AdminPanel::initialize() {
    setupUI();
}

void AdminPanel::render() {
    Screen::render();
    
    // Render title
    window->renderCenteredText("Administrative Panel", 50, window->getLargeFont(), window->getPrimaryColor());
    
    // Render system statistics
    renderSystemStats();
    
    // Render user and account lists
    renderUserList();
    renderAccountList();
    
    // Render messages
    renderMessage();
}

void AdminPanel::handleEvent(const SDL_Event& event) {
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

void AdminPanel::update() {
    // Update message timer
    if (messageTimer > 0) {
        messageTimer--;
        if (messageTimer <= 0) {
            clearMessage();
        }
    }
}

void AdminPanel::onEnter() {
    clearMessage();
}

void AdminPanel::onExit() {
    // Clean up
}

void AdminPanel::setupUI() {
    int buttonWidth = 120;
    int buttonHeight = 40;
    int spacing = 20;
    int startX = 50;
    int startY = window->getHeight() - 100;
    
    // Create search field
    searchField = std::make_shared<TextField>(startX, startY - 60, 200, 30, "search", "Search users/accounts");
    
    // Create buttons
    searchButton = std::make_shared<Button>(startX + 220, startY - 60, 80, 30, "Search", "search");
    createUserButton = std::make_shared<Button>(startX, startY, buttonWidth, buttonHeight, "Create User", "createUser");
    createAccountButton = std::make_shared<Button>(startX + buttonWidth + spacing, startY, buttonWidth, buttonHeight, "Create Account", "createAccount");
    backButton = std::make_shared<Button>(startX + 2*(buttonWidth + spacing), startY, buttonWidth, buttonHeight, "Back", "back");
    refreshButton = std::make_shared<Button>(startX + 3*(buttonWidth + spacing), startY, buttonWidth, buttonHeight, "Refresh", "refresh");
    
    // Set up callbacks
    searchButton->setOnClick([this]() { handleSearch(); });
    createUserButton->setOnClick([this]() { handleCreateUser(); });
    createAccountButton->setOnClick([this]() { handleCreateAccount(); });
    backButton->setOnClick([this]() { handleBack(); });
    refreshButton->setOnClick([this]() { handleRefresh(); });
    
    // Add UI elements
    addTextField(searchField);
    addButton(searchButton);
    addButton(createUserButton);
    addButton(createAccountButton);
    addButton(backButton);
    addButton(refreshButton);
}

void AdminPanel::renderSystemStats() {
    int startY = 100;
    int lineHeight = 25;
    
    window->renderText("System Statistics:", 50, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    // Get system statistics
    int totalUsers = bank->getTotalUsers();
    int totalAccounts = bank->getTotalAccounts();
    double totalAssets = bank->getTotalAssets();
    
    std::string usersStr = "Total Users: " + std::to_string(totalUsers);
    std::string accountsStr = "Total Accounts: " + std::to_string(totalAccounts);
    std::string assetsStr = "Total Assets: $" + std::to_string(static_cast<int>(totalAssets));
    
    window->renderText(usersStr, 50, startY, window->getFont(), window->getTextColor());
    window->renderText(accountsStr, 250, startY, window->getFont(), window->getTextColor());
    window->renderText(assetsStr, 450, startY, window->getFont(), window->getTextColor());
}

void AdminPanel::renderUserList() {
    int startY = 200;
    int lineHeight = 20;
    
    window->renderText("User Management:", 50, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    auto users = bank->getAllUsers();
    if (users.empty()) {
        window->renderText("No users found.", 50, startY, window->getFont(), window->getSecondaryColor());
        return;
    }
    
    // Render user headers
    window->renderText("Username", 50, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Name", 200, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Role", 350, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Accounts", 450, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    // Render users (limit to 10 for display)
    int count = 0;
    for (auto& user : users) {
        if (count >= 10) break;
        
        std::string roleStr = user->isAdmin() ? "Admin" : "Customer";
        std::string accountsStr = std::to_string(user->getAccounts().size());
        
        window->renderText(user->getUsername(), 50, startY, window->getFont(), window->getTextColor());
        window->renderText(user->getFirstName() + " " + user->getLastName(), 200, startY, window->getFont(), window->getTextColor());
        window->renderText(roleStr, 350, startY, window->getFont(), window->getTextColor());
        window->renderText(accountsStr, 450, startY, window->getFont(), window->getTextColor());
        
        startY += lineHeight;
        count++;
    }
    
    if (users.size() > 10) {
        window->renderText("... and " + std::to_string(users.size() - 10) + " more users", 50, startY, window->getFont(), window->getSecondaryColor());
    }
}

void AdminPanel::renderAccountList() {
    int startY = 450;
    int lineHeight = 20;
    
    window->renderText("Account Management:", 50, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    auto accounts = bank->getAllAccounts();
    if (accounts.empty()) {
        window->renderText("No accounts found.", 50, startY, window->getFont(), window->getSecondaryColor());
        return;
    }
    
    // Render account headers
    window->renderText("Account #", 50, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Holder", 200, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Type", 350, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Balance", 450, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    // Render accounts (limit to 8 for display)
    int count = 0;
    for (auto& account : accounts) {
        if (count >= 8) break;
        
        std::string balanceStr = "$" + std::to_string(static_cast<int>(account->getBalance()));
        
        window->renderText(account->getAccountNumber(), 50, startY, window->getFont(), window->getTextColor());
        window->renderText(account->getAccountHolderName(), 200, startY, window->getFont(), window->getTextColor());
        window->renderText(account->getAccountTypeString(), 350, startY, window->getFont(), window->getTextColor());
        window->renderText(balanceStr, 450, startY, window->getFont(), window->getTextColor());
        
        startY += lineHeight;
        count++;
    }
    
    if (accounts.size() > 8) {
        window->renderText("... and " + std::to_string(accounts.size() - 8) + " more accounts", 50, startY, window->getFont(), window->getSecondaryColor());
    }
}

void AdminPanel::handleSearch() {
    searchQuery = searchField->getText();
    if (searchQuery.empty()) {
        showMessage("Please enter a search term");
        return;
    }
    
    // In a real implementation, this would search through users and accounts
    showMessage("Search functionality coming soon!");
}

void AdminPanel::handleCreateUser() {
    showMessage("User creation feature coming soon!");
}

void AdminPanel::handleCreateAccount() {
    showMessage("Account creation feature coming soon!");
}

void AdminPanel::handleBack() {
    auto dashboard = std::make_shared<DashboardScreen>(window, bank);
    dashboard->initialize();
    window->setScreen(dashboard);
}

void AdminPanel::handleRefresh() {
    showMessage("Data refreshed!");
}

void AdminPanel::showMessage(const std::string& msg) {
    message = msg;
    messageVisible = true;
    messageTimer = 180; // Show for 3 seconds at 60 FPS
}

void AdminPanel::clearMessage() {
    messageVisible = false;
    message.clear();
}

void AdminPanel::renderMessage() {
    if (messageVisible && !message.empty()) {
        window->renderCenteredText(message, 550, window->getFont(), window->getAccentColor());
    }
} 