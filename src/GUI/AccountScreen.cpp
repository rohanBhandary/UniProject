#include "GUI/AccountScreen.h"
#include "GUI/Window.h"
#include "Bank.h"
#include <iostream>

AccountScreen::AccountScreen(std::shared_ptr<Window> window, std::shared_ptr<Bank> bankSystem)
    : Screen(window, "Account Management"), bank(bankSystem) {
}

void AccountScreen::initialize() {
    // Implementation for account screen
}

void AccountScreen::render() {
    Screen::render();
    window->renderCenteredText("Account Management - Coming Soon!", 300, window->getFont(), window->getAccentColor());
}

void AccountScreen::handleEvent(const SDL_Event& event) {
    Screen::handleEvent(event);
}

void AccountScreen::update() {
    // Update logic
} 