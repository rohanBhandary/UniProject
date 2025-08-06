#pragma once
#include "Screen.h"
#include <memory>

class Bank;
class TextField;
class Button;

class AdminPanel : public Screen {
private:
    std::shared_ptr<Bank> bank;
    std::shared_ptr<TextField> searchField;
    std::shared_ptr<Button> searchButton;
    std::shared_ptr<Button> createUserButton;
    std::shared_ptr<Button> createAccountButton;
    std::shared_ptr<Button> backButton;
    std::shared_ptr<Button> refreshButton;
    
    std::string searchQuery;
    std::string message;
    bool messageVisible;
    int messageTimer;

public:
    AdminPanel(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    
    void onEnter() override;
    void onExit() override;
    
private:
    void setupUI();
    void handleSearch();
    void handleCreateUser();
    void handleCreateAccount();
    void handleBack();
    void handleRefresh();
    void showMessage(const std::string& msg);
    void clearMessage();
    void renderMessage();
    void renderSystemStats();
    void renderUserList();
    void renderAccountList();
}; 