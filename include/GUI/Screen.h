#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <vector>
#include <string>

class Window;
class Button;
class TextField;

class Screen {
protected:
    std::shared_ptr<Window> window;
    std::vector<std::shared_ptr<Button>> buttons;
    std::vector<std::shared_ptr<TextField>> textFields;
    std::string title;
    bool isActive;

public:
    Screen(std::shared_ptr<Window> win, const std::string& screenTitle = "");
    virtual ~Screen() = default;
    
    // Screen lifecycle
    virtual void initialize() = 0;
    virtual void render();
    virtual void handleEvent(const SDL_Event& event);
    virtual void update();
    
    // Screen management
    void setActive(bool active) { isActive = active; }
    bool getIsActive() const { return isActive; }
    std::string getTitle() const { return title; }
    
    // UI element management
    void addButton(std::shared_ptr<Button> button);
    void addTextField(std::shared_ptr<TextField> textField);
    void clearUI();
    
    // Navigation
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onBack() {}
    
    // Utility
    std::shared_ptr<Window> getWindow() const { return window; }
    
protected:
    // Common UI rendering
    void renderTitle();
    void renderBackground();
    void renderUI();
    
    // Event handling helpers
    void handleMouseClick(int x, int y);
    void handleKeyPress(SDL_Keycode key);
    void handleTextInput(const std::string& text);
    
    // Utility methods
    bool isPointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH) const;
    void centerText(const std::string& text, int y, TTF_Font* font = nullptr);
}; 