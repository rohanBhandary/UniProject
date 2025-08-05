#include "GUI/TextField.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

TextField::TextField(int fieldX, int fieldY, int fieldWidth, int fieldHeight, 
                    const std::string& fieldId, const std::string& fieldPlaceholder)
    : x(fieldX), y(fieldY), width(fieldWidth), height(fieldHeight), 
      placeholder(fieldPlaceholder), id(fieldId), isFocused(false), isEnabled(true), 
      isVisible(true), isPassword(false), cursorPosition(0), scrollOffset(0), maxLength(100) {
    
    // Initialize colors
    normalColor = {255, 255, 255, 255};    // White
    focusColor = {240, 248, 255, 255};     // Alice blue
    textColor = {0, 0, 0, 255};           // Black
    placeholderColor = {128, 128, 128, 255}; // Gray
    borderColor = {200, 200, 200, 255};   // Light gray
}

void TextField::render(SDL_Renderer* renderer, TTF_Font* font) {
    if (!isVisible) return;
    
    std::cout << "TextField::render() - Rendering field '" << id << "' at (" << x << "," << y << ")" << std::endl;
    
    // Draw background
    SDL_Color currentColor = getCurrentColor();
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_Rect fieldRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &fieldRect);
    
    // Draw border
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(renderer, &fieldRect);
    
    // Render text
    if (font) {
        std::string displayText = getDisplayText();
        SDL_Color textRenderColor = text.empty() && !isFocused ? placeholderColor : textColor;
        
        SDL_Surface* surface = TTF_RenderText_Solid(font, displayText.c_str(), textRenderColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                // Calculate text position
                int textX = x + 5;
                int textY = y + (height - surface->h) / 2;
                
                // Apply scroll offset
                textX -= scrollOffset;
                
                SDL_Rect textRect = {textX, textY, surface->w, surface->h};
                SDL_Rect clipRect = {x + 2, y + 2, width - 4, height - 4};
                
                SDL_RenderSetClipRect(renderer, &clipRect);
                SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                SDL_RenderSetClipRect(renderer, nullptr);
                
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
    
    // Draw cursor if focused
    if (isFocused && font) {
        // Calculate cursor position
        std::string textBeforeCursor = text.substr(0, cursorPosition);
        SDL_Surface* cursorSurface = TTF_RenderText_Solid(font, textBeforeCursor.c_str(), textColor);
        if (cursorSurface) {
            int cursorX = x + 5 + cursorSurface->w - scrollOffset;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, cursorX, y + 5, cursorX, y + height - 5);
            SDL_FreeSurface(cursorSurface);
        }
    }
}

void TextField::handleMouseClick(int mouseX, int mouseY) {
    if (isPointInside(mouseX, mouseY) && isEnabled) {
        setFocused(true);
        updateCursorPosition(mouseX);
    } else {
        setFocused(false);
    }
}

void TextField::handleKeyPress(SDL_Keycode key) {
    if (!isFocused || !isEnabled) return;
    
    switch (key) {
        case SDL_SCANCODE_BACKSPACE:
            backspaceCharacter();
            break;
        case SDL_SCANCODE_DELETE:
            deleteCharacter();
            break;
        case SDL_SCANCODE_LEFT:
            if (cursorPosition > 0) {
                cursorPosition--;
                ensureCursorVisible();
            }
            break;
        case SDL_SCANCODE_RIGHT:
            if (cursorPosition < text.length()) {
                cursorPosition++;
                ensureCursorVisible();
            }
            break;
        case SDL_SCANCODE_HOME:
            cursorPosition = 0;
            scrollOffset = 0;
            break;
        case SDL_SCANCODE_END:
            cursorPosition = text.length();
            ensureCursorVisible();
            break;
        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_KP_ENTER:
            if (onEnterCallback) {
                onEnterCallback();
            }
            break;
        case SDL_SCANCODE_TAB:
            // Handle tab navigation
            break;
    }
}

void TextField::handleTextInput(const std::string& input) {
    if (!isFocused || !isEnabled) return;
    
    for (char c : input) {
        if (text.length() < maxLength) {
            text.insert(cursorPosition, 1, c);
            cursorPosition++;
            ensureCursorVisible();
            notifyChange();
        }
    }
}

void TextField::setFocused(bool focused) {
    if (isFocused != focused) {
        isFocused = focused;
        if (focused && onFocusCallback) {
            onFocusCallback();
        } else if (!focused && onBlurCallback) {
            onBlurCallback();
        }
    }
}

void TextField::setText(const std::string& newText) {
    text = newText;
    cursorPosition = text.length();
    ensureCursorVisible();
    notifyChange();
}

void TextField::appendText(const std::string& newText) {
    text += newText;
    cursorPosition = text.length();
    ensureCursorVisible();
    notifyChange();
}

void TextField::clearText() {
    text.clear();
    cursorPosition = 0;
    scrollOffset = 0;
    notifyChange();
}

void TextField::deleteCharacter() {
    if (cursorPosition < text.length()) {
        text.erase(cursorPosition, 1);
        ensureCursorVisible();
        notifyChange();
    }
}

void TextField::backspaceCharacter() {
    if (cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        cursorPosition--;
        ensureCursorVisible();
        notifyChange();
    }
}

bool TextField::isPointInside(int pointX, int pointY) const {
    return pointX >= x && pointX < x + width && pointY >= y && pointY < y + height;
}

void TextField::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void TextField::setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

std::string TextField::getDisplayText() const {
    if (text.empty() && !isFocused) {
        return placeholder;
    }
    
    if (isPassword) {
        return std::string(text.length(), '*');
    }
    
    return text;
}

SDL_Color TextField::getCurrentColor() const {
    if (!isEnabled) {
        return {240, 240, 240, 255}; // Light gray
    }
    
    if (isFocused) {
        return focusColor;
    }
    
    return normalColor;
}

void TextField::updateCursorPosition(int mouseX) {
    // Simple cursor positioning - in a real implementation, you'd calculate the exact position
    int relativeX = mouseX - x - 5;
    cursorPosition = std::min(static_cast<size_t>(relativeX / 8), text.length());
    ensureCursorVisible();
}

void TextField::ensureCursorVisible() {
    // Simple scroll implementation
    int cursorPixelPos = cursorPosition * 8; // Approximate character width
    if (cursorPixelPos < scrollOffset) {
        scrollOffset = cursorPixelPos;
    } else if (cursorPixelPos > scrollOffset + width - 10) {
        scrollOffset = cursorPixelPos - width + 10;
    }
    
    if (scrollOffset < 0) scrollOffset = 0;
}

void TextField::notifyChange() {
    if (onChangeCallback) {
        onChangeCallback(text);
    }
} 