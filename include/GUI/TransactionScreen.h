#pragma once
#include "Screen.h"
#include <memory>

class Bank;
class Button;

class TransactionScreen : public Screen {
private:
    std::shared_ptr<Bank> bank;
    std::shared_ptr<Button> backButton;
    std::shared_ptr<Button> refreshButton;

public:
    TransactionScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    
    void onEnter() override;
    void onExit() override;
    
private:
    void setupUI();
    void renderTransactionList();
    void renderTransactionSummary();
    void handleBack();
    void handleRefresh();
}; 