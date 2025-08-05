#include "GUI/Screen.h"
#include "GUI/Window.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include <SDL2/SDL.h>
#include <iostream>

Screen::Screen(std::shared_ptr<Window> win, const std::string& screenTitle)
    : window(win), title(screenTitle), isActive(false) {
}

void Screen::render() {
    renderBackground();
    renderTitle();
    renderUI();
}

void Screen::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                handleMouseClick(event.button.x, event.button.y);
            }
            break;
        case SDL_KEYDOWN:
            handleKeyPress(event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT:
            handleTextInput(event.text.text);
            break;
    }
}

void Screen::update() {
    // Base implementation - can be overridden by derived classes
}

void Screen::addButton(std::shared_ptr<Button> button) {
    buttons.push_back(button);
}

void Screen::addTextField(std::shared_ptr<TextField> textField) {
    textFields.push_back(textField);
}

void Screen::clearUI() {
    buttons.clear();
    textFields.clear();
}

void Screen::renderTitle() {
    if (!title.empty()) {
        window->renderCenteredText(title, 50, window->getLargeFont(), window->getTextColor());
    }
}

void Screen::renderBackground() {
    // Default background rendering
    window->drawRect(0, 0, window->getWidth(), window->getHeight(), window->getBackgroundColor(), true);
    
    // Add some basic shapes to make sure rendering is working
    window->drawRect(50, 50, 200, 100, {255, 0, 0, 255}, true);  // Red rectangle
    window->drawRect(300, 50, 200, 100, {0, 255, 0, 255}, true);  // Green rectangle
}

void Screen::renderUI() {
    std::cout << "Screen::renderUI() - Rendering " << buttons.size() << " buttons and " << textFields.size() << " text fields" << std::endl;
    
    // Render all buttons
    for (auto& button : buttons) {
        if (button->getIsVisible()) {
            button->render(window->getRenderer(), window->getFont());
        }
    }
    
    // Render all text fields
    for (auto& textField : textFields) {
        if (textField->getIsVisible()) {
            textField->render(window->getRenderer(), window->getFont());
        }
    }
}

void Screen::handleMouseClick(int x, int y) {
    // Check button clicks
    for (auto& button : buttons) {
        if (button->getIsVisible() && button->getIsEnabled()) {
            button->handleMouseClick(x, y);
        }
    }
    
    // Check text field clicks
    for (auto& textField : textFields) {
        if (textField->getIsVisible() && textField->getIsEnabled()) {
            textField->handleMouseClick(x, y);
        }
    }
}

void Screen::handleKeyPress(SDL_Keycode key) {
    // Handle global key presses
    switch (key) {
        case SDL_SCANCODE_ESCAPE:
            onBack();
            break;
        case SDL_SCANCODE_TAB:
            // Handle tab navigation between text fields
            break;
    }
    
    // Pass key events to focused text fields
    for (auto& textField : textFields) {
        if (textField->getIsFocused()) {
            textField->handleKeyPress(key);
            break;
        }
    }
}

void Screen::handleTextInput(const std::string& text) {
    // Pass text input to focused text fields
    for (auto& textField : textFields) {
        if (textField->getIsFocused()) {
            textField->handleTextInput(text);
            break;
        }
    }
}

bool Screen::isPointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH) const {
    return x >= rectX && x < rectX + rectW && y >= rectY && y < rectY + rectH;
}

void Screen::centerText(const std::string& text, int y, TTF_Font* font) {
    window->renderCenteredText(text, y, font);
} 