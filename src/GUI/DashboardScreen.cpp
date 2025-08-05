#include "GUI/DashboardScreen.h"
#include "GUI/LoginScreen.h"
#include "GUI/TransferScreen.h"
#include "GUI/TransactionScreen.h"
#include "GUI/AdminPanel.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include "Bank.h"
#include "User.h"
#include "Account.h"
#include <iostream>
#include <sstream>
#include <iomanip>

DashboardScreen::DashboardScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Banking Dashboard"), bank(bankSystem), selectedAccountIndex(0), 
      animationTimer(0), showBalance(true), balanceOpacity(1.0f) {
}

void DashboardScreen::initialize() {
    setupUI();
    loadUserAccounts();
    updateWelcomeMessage();
    updateBalanceDisplay();
}

void DashboardScreen::render() {
    Screen::render();
    
    // Render dashboard content
    renderUserInfo();
    renderAccountInfo();
    renderBalanceInfo();
    renderQuickActions();
    renderAccountSelector();
    
    // Render animations
    if (animationTimer > 0) {
        animationTimer--;
    }
}

void DashboardScreen::handleEvent(const SDL_Event& event) {
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

void DashboardScreen::update() {
    // Update animations
    if (animationTimer > 0) {
        animationTimer--;
    }
    
    // Update balance display animation
    if (showBalance && balanceOpacity < 1.0f) {
        balanceOpacity += 0.05f;
        if (balanceOpacity > 1.0f) balanceOpacity = 1.0f;
    }
}

void DashboardScreen::onEnter() {
    refreshData();
}

void DashboardScreen::onExit() {
    // Clean up any resources
}

void DashboardScreen::setupUI() {
    int buttonWidth = 150;
    int buttonHeight = 50;
    int spacing = 20;
    int startX = 50;
    int startY = 150;
    
    // Create navigation buttons
    accountsButton = std::make_shared<Button>(startX, startY, buttonWidth, buttonHeight, "My Accounts", "accounts");
    transactionsButton = std::make_shared<Button>(startX + buttonWidth + spacing, startY, buttonWidth, buttonHeight, "Transactions", "transactions");
    transferButton = std::make_shared<Button>(startX + 2*(buttonWidth + spacing), startY, buttonWidth, buttonHeight, "Transfer", "transfer");
    
    startY += buttonHeight + spacing;
    profileButton = std::make_shared<Button>(startX, startY, buttonWidth, buttonHeight, "Profile", "profile");
    logoutButton = std::make_shared<Button>(startX + buttonWidth + spacing, startY, buttonWidth, buttonHeight, "Logout", "logout");
    
    // Admin button (only show for admin users)
    if (bank->getCurrentUser() && bank->getCurrentUser()->isAdmin()) {
        adminButton = std::make_shared<Button>(startX + 2*(buttonWidth + spacing), startY, buttonWidth, buttonHeight, "Admin Panel", "admin");
        addButton(adminButton);
    }
    
    // Account navigation buttons
    prevAccountButton = std::make_shared<Button>(window->getWidth() - 200, 300, 80, 30, "Previous", "prev");
    nextAccountButton = std::make_shared<Button>(window->getWidth() - 100, 300, 80, 30, "Next", "next");
    refreshButton = std::make_shared<Button>(window->getWidth() - 150, 350, 100, 30, "Refresh", "refresh");
    
    // Set up callbacks
    accountsButton->setOnClick([this]() { handleAccountsClick(); });
    transactionsButton->setOnClick([this]() { handleTransactionsClick(); });
    transferButton->setOnClick([this]() { handleTransferClick(); });
    profileButton->setOnClick([this]() { handleProfileClick(); });
    logoutButton->setOnClick([this]() { handleLogoutClick(); });
    if (adminButton) adminButton->setOnClick([this]() { handleAdminClick(); });
    prevAccountButton->setOnClick([this]() { handlePrevAccount(); });
    nextAccountButton->setOnClick([this]() { handleNextAccount(); });
    refreshButton->setOnClick([this]() { handleRefresh(); });
    
    // Add buttons
    addButton(accountsButton);
    addButton(transactionsButton);
    addButton(transferButton);
    addButton(profileButton);
    addButton(logoutButton);
    addButton(prevAccountButton);
    addButton(nextAccountButton);
    addButton(refreshButton);
}

void DashboardScreen::loadUserAccounts() {
    auto currentUser = bank->getCurrentUser();
    if (currentUser) {
        userAccounts = currentUser->getAccounts();
        if (!userAccounts.empty()) {
            selectedAccount = userAccounts[0];
            selectedAccountIndex = 0;
        }
    }
}

void DashboardScreen::updateWelcomeMessage() {
    auto currentUser = bank->getCurrentUser();
    if (currentUser) {
        welcomeMessage = "Welcome back, " + currentUser->getFirstName() + "!";
        lastLoginInfo = "Last login: " + currentUser->getFormattedLastLogin();
    }
}

void DashboardScreen::updateBalanceDisplay() {
    if (selectedAccount) {
        accountInfo = "Account: " + selectedAccount->getAccountNumber() + 
                     " (" + selectedAccount->getAccountTypeString() + ")";
        balanceInfo = "Balance: " + selectedAccount->getFormattedBalance();
    } else {
        accountInfo = "No accounts available";
        balanceInfo = "Balance: $0.00";
    }
}

void DashboardScreen::renderUserInfo() {
    window->renderText(welcomeMessage, 50, 50, window->getLargeFont(), window->getPrimaryColor());
    window->renderText(lastLoginInfo, 50, 80, window->getFont(), window->getSecondaryColor());
}

void DashboardScreen::renderAccountInfo() {
    window->renderText(accountInfo, 50, 250, window->getFont(), window->getTextColor());
}

void DashboardScreen::renderBalanceInfo() {
    // Apply opacity for animation
    SDL_Color balanceColor = window->getTextColor();
    balanceColor.a = static_cast<Uint8>(255 * balanceOpacity);
    
    window->renderText(balanceInfo, 50, 280, window->getLargeFont(), balanceColor);
}

void DashboardScreen::renderQuickActions() {
    window->renderText("Quick Actions:", 50, 400, window->getFont(), window->getTextColor());
}

void DashboardScreen::renderAccountSelector() {
    if (!userAccounts.empty()) {
        std::string accountText = "Account " + std::to_string(selectedAccountIndex + 1) + 
                                 " of " + std::to_string(userAccounts.size());
        window->renderText(accountText, window->getWidth() - 200, 250, window->getFont(), window->getSecondaryColor());
    }
}

void DashboardScreen::handleAccountsClick() {
    // Navigate to accounts screen
    window->renderCenteredText("Accounts feature coming soon!", 500, window->getFont(), window->getAccentColor());
}

void DashboardScreen::handleTransactionsClick() {
    // Navigate to transactions screen
    auto transactionScreen = std::make_shared<TransactionScreen>(window, bank);
    transactionScreen->initialize();
    window->setScreen(transactionScreen);
}

void DashboardScreen::handleTransferClick() {
    // Navigate to transfer screen
    auto transferScreen = std::make_shared<TransferScreen>(window, bank);
    transferScreen->initialize();
    window->setScreen(transferScreen);
}

void DashboardScreen::handleProfileClick() {
    // Navigate to profile screen
    window->renderCenteredText("Profile feature coming soon!", 500, window->getFont(), window->getAccentColor());
}

void DashboardScreen::handleLogoutClick() {
    bank->logoutUser();
    auto loginScreen = std::make_shared<LoginScreen>(window, bank);
    loginScreen->initialize();
    window->setScreen(loginScreen);
}

void DashboardScreen::handleAdminClick() {
    // Navigate to admin panel
    auto adminPanel = std::make_shared<AdminPanel>(window, bank);
    adminPanel->initialize();
    window->setScreen(adminPanel);
}

void DashboardScreen::handlePrevAccount() {
    if (!userAccounts.empty()) {
        selectedAccountIndex = (selectedAccountIndex - 1 + userAccounts.size()) % userAccounts.size();
        selectAccount(selectedAccountIndex);
    }
}

void DashboardScreen::handleNextAccount() {
    if (!userAccounts.empty()) {
        selectedAccountIndex = (selectedAccountIndex + 1) % userAccounts.size();
        selectAccount(selectedAccountIndex);
    }
}

void DashboardScreen::handleRefresh() {
    refreshData();
    animationTimer = 60; // 1 second animation
}

void DashboardScreen::selectAccount(int index) {
    if (index >= 0 && index < userAccounts.size()) {
        selectedAccount = userAccounts[index];
        selectedAccountIndex = index;
        updateBalanceDisplay();
        balanceOpacity = 0.0f; // Start fade-in animation
    }
}

void DashboardScreen::refreshData() {
    loadUserAccounts();
    updateWelcomeMessage();
    updateBalanceDisplay();
} 