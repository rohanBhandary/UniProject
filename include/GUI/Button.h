#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

class Button {
private:
    int x, y, width, height;
    std::string text;
    std::string id;
    
    SDL_Color normalColor;
    SDL_Color hoverColor;
    SDL_Color pressedColor;
    SDL_Color textColor;
    SDL_Color disabledColor;
    
    bool isHovered;
    bool isPressed;
    bool isEnabled;
    bool isVisible;
    
    std::function<void()> onClickCallback;
    std::function<void()> onHoverCallback;
    std::function<void()> onLeaveCallback;

public:
    Button(int buttonX, int buttonY, int buttonWidth, int buttonHeight, 
           const std::string& buttonText, const std::string& buttonId = "");
    
    // Rendering
    void render(SDL_Renderer* renderer, TTF_Font* font);
    
    // Event handling
    void handleMouseMove(int mouseX, int mouseY);
    void handleMouseClick(int mouseX, int mouseY);
    void handleMouseRelease(int mouseX, int mouseY);
    
    // State management
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setVisible(bool visible) { isVisible = visible; }
    void setText(const std::string& newText) { text = newText; }
    
    // Callbacks
    void setOnClick(std::function<void()> callback) { onClickCallback = callback; }
    void setOnHover(std::function<void()> callback) { onHoverCallback = callback; }
    void setOnLeave(std::function<void()> callback) { onLeaveCallback = callback; }
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::string getText() const { return text; }
    std::string getId() const { return id; }
    bool getIsHovered() const { return isHovered; }
    bool getIsPressed() const { return isPressed; }
    bool getIsEnabled() const { return isEnabled; }
    bool getIsVisible() const { return isVisible; }
    
    // Utility
    bool isPointInside(int pointX, int pointY) const;
    void setPosition(int newX, int newY);
    void setSize(int newWidth, int newHeight);
    
private:
    SDL_Color getCurrentColor() const;
    void updateState(bool hovered, bool pressed);
}; 