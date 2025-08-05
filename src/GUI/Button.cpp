#include "GUI/Button.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

Button::Button(int buttonX, int buttonY, int buttonWidth, int buttonHeight, 
               const std::string& buttonText, const std::string& buttonId)
    : x(buttonX), y(buttonY), width(buttonWidth), height(buttonHeight), 
      text(buttonText), id(buttonId), isHovered(false), isPressed(false), 
      isEnabled(true), isVisible(true) {
    
    // Initialize colors
    normalColor = {70, 130, 180, 255};    // Steel blue
    hoverColor = {100, 149, 237, 255};    // Cornflower blue
    pressedColor = {25, 25, 112, 255};    // Midnight blue
    textColor = {255, 255, 255, 255};     // White
    disabledColor = {128, 128, 128, 255}; // Gray
}

void Button::render(SDL_Renderer* renderer, TTF_Font* font) {
    if (!isVisible) return;
    
    std::cout << "Button::render() - Rendering button '" << text << "' at (" << x << "," << y << ")" << std::endl;
    
    // Draw button background
    SDL_Color currentColor = getCurrentColor();
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_Rect buttonRect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &buttonRect);
    
    // Draw button border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &buttonRect);
    
    // Render text
    if (font && !text.empty()) {
        SDL_Color renderTextColor = isEnabled ? textColor : disabledColor;
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), renderTextColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                // Center text in button
                int textX = x + (width - surface->w) / 2;
                int textY = y + (height - surface->h) / 2;
                SDL_Rect textRect = {textX, textY, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
}

void Button::handleMouseMove(int mouseX, int mouseY) {
    bool wasHovered = isHovered;
    isHovered = isPointInside(mouseX, mouseY);
    
    if (isHovered != wasHovered) {
        if (isHovered && onHoverCallback) {
            onHoverCallback();
        } else if (!isHovered && onLeaveCallback) {
            onLeaveCallback();
        }
    }
}

void Button::handleMouseClick(int mouseX, int mouseY) {
    if (isPointInside(mouseX, mouseY) && isEnabled) {
        isPressed = true;
    }
}

void Button::handleMouseRelease(int mouseX, int mouseY) {
    if (isPressed && isPointInside(mouseX, mouseY) && isEnabled) {
        if (onClickCallback) {
            onClickCallback();
        }
    }
    isPressed = false;
}

bool Button::isPointInside(int pointX, int pointY) const {
    return pointX >= x && pointX < x + width && pointY >= y && pointY < y + height;
}

void Button::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Button::setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

SDL_Color Button::getCurrentColor() const {
    if (!isEnabled) {
        return disabledColor;
    }
    
    if (isPressed) {
        return pressedColor;
    }
    
    if (isHovered) {
        return hoverColor;
    }
    
    return normalColor;
}

void Button::updateState(bool hovered, bool pressed) {
    isHovered = hovered;
    isPressed = pressed;
} 