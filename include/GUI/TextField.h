#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>

class TextField {
private:
    int x, y, width, height;
    std::string text;
    std::string placeholder;
    std::string id;
    
    SDL_Color normalColor;
    SDL_Color focusColor;
    SDL_Color textColor;
    SDL_Color placeholderColor;
    SDL_Color borderColor;
    
    bool isFocused;
    bool isEnabled;
    bool isVisible;
    bool isPassword;
    
    int cursorPosition;
    int scrollOffset;
    int maxLength;
    
    std::function<void(const std::string&)> onChangeCallback;
    std::function<void()> onFocusCallback;
    std::function<void()> onBlurCallback;
    std::function<void()> onEnterCallback;

public:
    TextField(int fieldX, int fieldY, int fieldWidth, int fieldHeight, 
              const std::string& fieldId = "", const std::string& fieldPlaceholder = "");
    
    // Rendering
    void render(SDL_Renderer* renderer, TTF_Font* font);
    
    // Event handling
    void handleMouseClick(int mouseX, int mouseY);
    void handleKeyPress(SDL_Keycode key);
    void handleTextInput(const std::string& input);
    
    // State management
    void setEnabled(bool enabled) { isEnabled = enabled; }
    void setVisible(bool visible) { isVisible = visible; }
    void setFocused(bool focused);
    void setPassword(bool password) { isPassword = password; }
    void setMaxLength(int maxLen) { maxLength = maxLen; }
    
    // Text management
    void setText(const std::string& newText);
    void appendText(const std::string& newText);
    void clearText();
    void deleteCharacter();
    void backspaceCharacter();
    
    // Callbacks
    void setOnChange(std::function<void(const std::string&)> callback) { onChangeCallback = callback; }
    void setOnFocus(std::function<void()> callback) { onFocusCallback = callback; }
    void setOnBlur(std::function<void()> callback) { onBlurCallback = callback; }
    void setOnEnter(std::function<void()> callback) { onEnterCallback = callback; }
    
    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    std::string getText() const { return text; }
    std::string getPlaceholder() const { return placeholder; }
    std::string getId() const { return id; }
    bool getIsFocused() const { return isFocused; }
    bool getIsEnabled() const { return isEnabled; }
    bool getIsVisible() const { return isVisible; }
    bool getIsPassword() const { return isPassword; }
    
    // Utility
    bool isPointInside(int pointX, int pointY) const;
    void setPosition(int newX, int newY);
    void setSize(int newWidth, int newHeight);
    std::string getDisplayText() const;
    
private:
    SDL_Color getCurrentColor() const;
    void updateCursorPosition(int mouseX);
    void ensureCursorVisible();
    void notifyChange();
}; 