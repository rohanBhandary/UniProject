#include "GUI/TransactionScreen.h"
#include "GUI/DashboardScreen.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include "Bank.h"
#include <iostream>
#include <sstream>
#include <iomanip>

TransactionScreen::TransactionScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Transaction History"), bank(bankSystem) {
}

void TransactionScreen::initialize() {
    setupUI();
}

void TransactionScreen::render() {
    Screen::render();
    
    // Render title
    window->renderCenteredText("Transaction History", 50, window->getLargeFont(), window->getPrimaryColor());
    
    // Render account selector
    auto currentUser = bank->getCurrentUser();
    if (currentUser) {
        auto accounts = currentUser->getAccounts();
        if (!accounts.empty()) {
            std::string accountInfo = "Account: " + accounts[0]->getAccountNumber() + " - " + accounts[0]->getFormattedBalance();
            window->renderCenteredText(accountInfo, 80, window->getFont(), window->getSecondaryColor());
        }
    }
    
    // Render transaction list
    renderTransactionList();
    
    // Render summary
    renderTransactionSummary();
}

void TransactionScreen::handleEvent(const SDL_Event& event) {
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

void TransactionScreen::update() {
    // Update logic
}

void TransactionScreen::onEnter() {
    // Refresh transaction data
}

void TransactionScreen::onExit() {
    // Clean up
}

void TransactionScreen::setupUI() {
    int buttonWidth = 120;
    int buttonHeight = 40;
    int spacing = 20;
    int startX = 50;
    int startY = window->getHeight() - 100;
    
    // Create navigation buttons
    backButton = std::make_shared<Button>(startX, startY, buttonWidth, buttonHeight, "Back", "back");
    refreshButton = std::make_shared<Button>(startX + buttonWidth + spacing, startY, buttonWidth, buttonHeight, "Refresh", "refresh");
    
    // Set up callbacks
    backButton->setOnClick([this]() { handleBack(); });
    refreshButton->setOnClick([this]() { handleRefresh(); });
    
    // Add buttons
    addButton(backButton);
    addButton(refreshButton);
}

void TransactionScreen::renderTransactionList() {
    auto currentUser = bank->getCurrentUser();
    if (!currentUser) return;
    
    auto accounts = currentUser->getAccounts();
    if (accounts.empty()) return;
    
    auto transactions = bank->getTransactionHistory(accounts[0]->getAccountNumber());
    
    int startY = 120;
    int lineHeight = 25;
    int maxTransactions = 15; // Limit to prevent overflow
    
    window->renderText("Recent Transactions:", 50, startY, window->getFont(), window->getTextColor());
    startY += 30;
    
    if (transactions.empty()) {
        window->renderText("No transactions found.", 50, startY, window->getFont(), window->getSecondaryColor());
        return;
    }
    
    // Render transaction headers
    window->renderText("Date", 50, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Type", 200, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Amount", 300, startY, window->getFont(), window->getPrimaryColor());
    window->renderText("Description", 450, startY, window->getFont(), window->getPrimaryColor());
    startY += lineHeight;
    
    // Render transactions (most recent first)
    int count = 0;
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < maxTransactions; ++it, ++count) {
        auto transaction = *it;
        
        // Determine color based on transaction type
        SDL_Color textColor = window->getTextColor();
        if (transaction->getType() == TransactionType::DEPOSIT) {
            textColor = window->getSuccessColor();
        } else if (transaction->getType() == TransactionType::WITHDRAWAL) {
            textColor = window->getErrorColor();
        }
        
        // Format date
        std::string dateStr = transaction->getFormattedTimestamp();
        if (dateStr.length() > 10) dateStr = dateStr.substr(0, 10);
        
        // Format amount
        std::string amountStr = "$" + std::to_string(static_cast<int>(transaction->getAmount()));
        if (transaction->getType() == TransactionType::WITHDRAWAL) {
            amountStr = "-" + amountStr;
        }
        
        // Render transaction row
        window->renderText(dateStr, 50, startY, window->getFont(), textColor);
        window->renderText(transaction->getTypeString(), 200, startY, window->getFont(), textColor);
        window->renderText(amountStr, 300, startY, window->getFont(), textColor);
        window->renderText(transaction->getDescription(), 450, startY, window->getFont(), textColor);
        
        startY += lineHeight;
    }
}

void TransactionScreen::renderTransactionSummary() {
    auto currentUser = bank->getCurrentUser();
    if (!currentUser) return;
    
    auto accounts = currentUser->getAccounts();
    if (accounts.empty()) return;
    
    auto transactions = bank->getTransactionHistory(accounts[0]->getAccountNumber());
    
    // Calculate summary statistics
    double totalDeposits = 0.0;
    double totalWithdrawals = 0.0;
    int transactionCount = transactions.size();
    
    for (auto& transaction : transactions) {
        if (transaction->getType() == TransactionType::DEPOSIT) {
            totalDeposits += transaction->getAmount();
        } else if (transaction->getType() == TransactionType::WITHDRAWAL) {
            totalWithdrawals += transaction->getAmount();
        }
    }
    
    // Render summary
    int summaryY = window->getHeight() - 200;
    window->renderText("Summary:", 50, summaryY, window->getFont(), window->getPrimaryColor());
    summaryY += 25;
    
    std::string totalDepositsStr = "Total Deposits: $" + std::to_string(static_cast<int>(totalDeposits));
    std::string totalWithdrawalsStr = "Total Withdrawals: $" + std::to_string(static_cast<int>(totalWithdrawals));
    std::string transactionCountStr = "Total Transactions: " + std::to_string(transactionCount);
    
    window->renderText(totalDepositsStr, 50, summaryY, window->getFont(), window->getSuccessColor());
    window->renderText(totalWithdrawalsStr, 250, summaryY, window->getFont(), window->getErrorColor());
    window->renderText(transactionCountStr, 450, summaryY, window->getFont(), window->getSecondaryColor());
}

void TransactionScreen::handleBack() {
    auto dashboard = std::make_shared<DashboardScreen>(window, bank);
    dashboard->initialize();
    window->setScreen(dashboard);
}

void TransactionScreen::handleRefresh() {
    // Refresh transaction data (in a real app, this would reload from database)
    onEnter();
} 