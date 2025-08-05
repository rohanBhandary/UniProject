#include "GUI/TransferScreen.h"
#include "GUI/DashboardScreen.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include "Bank.h"
#include <iostream>
#include <sstream>
#include <iomanip>

TransferScreen::TransferScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Money Transfer"), bank(bankSystem), showError(false), showSuccess(false), messageTimer(0) {
}

void TransferScreen::initialize() {
    setupUI();
}

void TransferScreen::render() {
    Screen::render();
    
    // Render title and instructions
    window->renderCenteredText("Money Transfer", 50, window->getLargeFont(), window->getPrimaryColor());
    window->renderCenteredText("Transfer money between accounts", 80, window->getFont(), window->getTextColor());
    
    // Render account information
    auto currentUser = bank->getCurrentUser();
    if (currentUser) {
        auto accounts = currentUser->getAccounts();
        if (!accounts.empty()) {
            std::string accountInfo = "Your accounts: ";
            for (size_t i = 0; i < accounts.size(); ++i) {
                if (i > 0) accountInfo += ", ";
                accountInfo += accounts[i]->getAccountNumber() + " ($" + std::to_string(static_cast<int>(accounts[i]->getBalance())) + ")";
            }
            window->renderCenteredText(accountInfo, 120, window->getFont(), window->getSecondaryColor());
        }
    }
    
    // Render messages
    renderMessages();
}

void TransferScreen::handleEvent(const SDL_Event& event) {
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

void TransferScreen::update() {
    // Update message timer
    if (messageTimer > 0) {
        messageTimer--;
        if (messageTimer <= 0) {
            clearMessages();
        }
    }
}

void TransferScreen::onEnter() {
    clearMessages();
    // Pre-fill from account with user's first account
    auto currentUser = bank->getCurrentUser();
    if (currentUser && !currentUser->getAccounts().empty()) {
        fromAccountField->setText(currentUser->getAccounts()[0]->getAccountNumber());
    }
}

void TransferScreen::onExit() {
    // Clear sensitive data
    if (amountField) amountField->clearText();
    if (toAccountField) toAccountField->clearText();
    if (descriptionField) descriptionField->clearText();
}

void TransferScreen::setupUI() {
    int centerX = window->getWidth() / 2;
    int startY = 200;
    int fieldWidth = 300;
    int fieldHeight = 40;
    int buttonWidth = 120;
    int buttonHeight = 40;
    int spacing = 20;
    
    // Create text fields
    fromAccountField = std::make_shared<TextField>(centerX - fieldWidth/2, startY, fieldWidth, fieldHeight, "fromAccount", "From Account Number");
    toAccountField = std::make_shared<TextField>(centerX - fieldWidth/2, startY + fieldHeight + spacing, fieldWidth, fieldHeight, "toAccount", "To Account Number");
    amountField = std::make_shared<TextField>(centerX - fieldWidth/2, startY + 2*(fieldHeight + spacing), fieldWidth, fieldHeight, "amount", "Amount ($)");
    descriptionField = std::make_shared<TextField>(centerX - fieldWidth/2, startY + 3*(fieldHeight + spacing), fieldWidth, fieldHeight, "description", "Description (Optional)");
    
    // Create buttons
    transferButton = std::make_shared<Button>(centerX - buttonWidth - spacing/2, startY + 4*(fieldHeight + spacing), buttonWidth, buttonHeight, "Transfer", "transfer");
    backButton = std::make_shared<Button>(centerX + spacing/2, startY + 4*(fieldHeight + spacing), buttonWidth, buttonHeight, "Back", "back");
    clearButton = std::make_shared<Button>(centerX - buttonWidth/2, startY + 5*(fieldHeight + spacing), buttonWidth, buttonHeight, "Clear", "clear");
    
    // Set up callbacks
    transferButton->setOnClick([this]() { handleTransfer(); });
    backButton->setOnClick([this]() { handleBack(); });
    clearButton->setOnClick([this]() { handleClear(); });
    
    // Add UI elements
    addTextField(fromAccountField);
    addTextField(toAccountField);
    addTextField(amountField);
    addTextField(descriptionField);
    addButton(transferButton);
    addButton(backButton);
    addButton(clearButton);
}

void TransferScreen::handleTransfer() {
    if (!validateInput()) return;
    
    std::string fromAccount = fromAccountField->getText();
    std::string toAccount = toAccountField->getText();
    std::string amountStr = amountField->getText();
    std::string description = descriptionField->getText();
    
    // Parse amount
    double amount;
    try {
        amount = std::stod(amountStr);
    } catch (const std::exception& e) {
        showErrorMessage("Invalid amount format");
        return;
    }
    
    // Perform transfer
    if (bank->transfer(fromAccount, toAccount, amount)) {
        showSuccessMessage("Transfer successful! Amount: $" + std::to_string(static_cast<int>(amount)));
        amountField->clearText();
        toAccountField->clearText();
        descriptionField->clearText();
    } else {
        showErrorMessage("Transfer failed. Please check account numbers and balance.");
    }
}

void TransferScreen::handleBack() {
    auto dashboard = std::make_shared<DashboardScreen>(window, bank);
    dashboard->initialize();
    window->setScreen(dashboard);
}

void TransferScreen::handleClear() {
    fromAccountField->clearText();
    toAccountField->clearText();
    amountField->clearText();
    descriptionField->clearText();
    clearMessages();
    
    // Pre-fill from account again
    auto currentUser = bank->getCurrentUser();
    if (currentUser && !currentUser->getAccounts().empty()) {
        fromAccountField->setText(currentUser->getAccounts()[0]->getAccountNumber());
    }
}

void TransferScreen::showErrorMessage(const std::string& message) {
    errorMessage = message;
    showError = true;
    showSuccess = false;
    messageTimer = 180; // Show for 3 seconds at 60 FPS
}

void TransferScreen::showSuccessMessage(const std::string& message) {
    successMessage = message;
    showSuccess = true;
    showError = false;
    messageTimer = 180; // Show for 3 seconds at 60 FPS
}

void TransferScreen::clearMessages() {
    showError = false;
    showSuccess = false;
    errorMessage.clear();
    successMessage.clear();
}

void TransferScreen::renderMessages() {
    if (showError && !errorMessage.empty()) {
        window->renderCenteredText(errorMessage, 500, window->getFont(), window->getErrorColor());
    }
    
    if (showSuccess && !successMessage.empty()) {
        window->renderCenteredText(successMessage, 500, window->getFont(), window->getSuccessColor());
    }
}

bool TransferScreen::validateInput() {
    if (fromAccountField->getText().empty()) {
        showErrorMessage("Please enter the source account number");
        return false;
    }
    
    if (toAccountField->getText().empty()) {
        showErrorMessage("Please enter the destination account number");
        return false;
    }
    
    if (amountField->getText().empty()) {
        showErrorMessage("Please enter the transfer amount");
        return false;
    }
    
    // Check if accounts exist
    auto fromAccount = bank->getAccount(fromAccountField->getText());
    auto toAccount = bank->getAccount(toAccountField->getText());
    
    if (!fromAccount) {
        showErrorMessage("Source account not found");
        return false;
    }
    
    if (!toAccount) {
        showErrorMessage("Destination account not found");
        return false;
    }
    
    // Check if user owns the source account
    auto currentUser = bank->getCurrentUser();
    if (currentUser) {
        bool ownsAccount = false;
        for (auto& account : currentUser->getAccounts()) {
            if (account->getAccountNumber() == fromAccountField->getText()) {
                ownsAccount = true;
                break;
            }
        }
        
        if (!ownsAccount) {
            showErrorMessage("You can only transfer from your own accounts");
            return false;
        }
    }
    
    return true;
} 