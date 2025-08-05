#pragma once
#include "Screen.h"
#include <memory>

class Bank;

class LoginScreen : public Screen {
private:
    std::shared_ptr<Bank> bank;
    std::shared_ptr<TextField> usernameField;
    std::shared_ptr<TextField> passwordField;
    std::shared_ptr<Button> loginButton;
    std::shared_ptr<Button> registerButton;
    std::shared_ptr<Button> exitButton;
    
    std::string errorMessage;
    bool showError;
    int errorTimer;

public:
    LoginScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    
    void onEnter() override;
    void onExit() override;
    
private:
    void setupUI();
    void handleLogin();
    void handleRegister();
    void handleExit();
    void showErrorMessage(const std::string& message);
    void clearError();
    void renderError();
}; 