#pragma once
#include "Screen.h"
#include <memory>
#include <vector>

class Bank;
class Account;

class DashboardScreen : public Screen {
private:
    std::shared_ptr<Bank> bank;
    
    // Navigation buttons
    std::shared_ptr<Button> accountsButton;
    std::shared_ptr<Button> transactionsButton;
    std::shared_ptr<Button> transferButton;
    std::shared_ptr<Button> profileButton;
    std::shared_ptr<Button> logoutButton;
    std::shared_ptr<Button> adminButton;
    
    // Account display
    std::vector<std::shared_ptr<Account>> userAccounts;
    std::shared_ptr<Account> selectedAccount;
    int selectedAccountIndex;
    
    // UI elements
    std::shared_ptr<Button> prevAccountButton;
    std::shared_ptr<Button> nextAccountButton;
    std::shared_ptr<Button> refreshButton;
    
    // Display variables
    std::string welcomeMessage;
    std::string accountInfo;
    std::string balanceInfo;
    std::string lastLoginInfo;
    
    // Animation and effects
    int animationTimer;
    bool showBalance;
    float balanceOpacity;

public:
    DashboardScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    
    void onEnter() override;
    void onExit() override;
    
private:
    void setupUI();
    void loadUserAccounts();
    void updateAccountDisplay();
    void updateWelcomeMessage();
    void updateBalanceDisplay();
    
    // Event handlers
    void handleAccountsClick();
    void handleTransactionsClick();
    void handleTransferClick();
    void handleProfileClick();
    void handleLogoutClick();
    void handleAdminClick();
    void handlePrevAccount();
    void handleNextAccount();
    void handleRefresh();
    
    // Rendering helpers
    void renderAccountInfo();
    void renderBalanceInfo();
    void renderQuickActions();
    void renderAccountSelector();
    void renderUserInfo();
    
    // Utility
    void selectAccount(int index);
    void refreshData();
}; 