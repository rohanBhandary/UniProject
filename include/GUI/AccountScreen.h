#pragma once
#include "Screen.h"
#include <memory>

class Bank;

class AccountScreen : public Screen {
private:
    std::shared_ptr<Bank> bank;

public:
    AccountScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem);
    
    void initialize() override;
    void render() override;
    void handleEvent(const SDL_Event& event) override;
    void update() override;
}; 