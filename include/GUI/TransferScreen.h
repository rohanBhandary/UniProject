#pragma once
#include "Screen.h"
#include <memory>

class Bank;
class TextField;
class Button;

class TransferScreen : public Screen {
private:
    std::shared_ptr<Bank> bank;
    std::shared_ptr<TextField> fromAccountField;
    std::shared_ptr<TextField> toAccountField;
    std::shared_ptr<TextField> amountField;
    std::shared_ptr<TextField> descriptionField;
    std::shared_ptr<Button> transferButton;
    std::shared_ptr<Button> backButton;
    std::shared_ptr<Button> clearButton;
    
    std::string errorMessage;
    std::string successMessage;
    bool showError;
    bool showSuccess;
    int messageTimer;

public:
    TransferScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    
    void onEnter() override;
    void onExit() override;
    
private:
    void setupUI();
    void handleTransfer();
    void handleBack();
    void handleClear();
    void showErrorMessage(const std::string& message);
    void showSuccessMessage(const std::string& message);
    void clearMessages();
    void renderMessages();
    bool validateInput();
}; 