#include "GUI/Window.h"
#include "GUI/Screen.h"
#include "GUI/Button.h"
#include "GUI/TextField.h"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

Window::Window(const std::string& title, int w, int h)
    : window(nullptr), renderer(nullptr), font(nullptr), largeFont(nullptr), smallFont(nullptr),
      width(w), height(h), isRunning(false) {
    initializeColors();
}

Window::~Window() {
    if (font) TTF_CloseFont(font);
    if (largeFont) TTF_CloseFont(largeFont);
    if (smallFont) TTF_CloseFont(smallFont);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Window::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow("Banking System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (!loadFonts()) {
        return false;
    }
    
    isRunning = true;
    return true;
}

void Window::run() {
    SDL_Event event;
    std::cout << "Window::run() started" << std::endl;
    
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (currentScreen) {
                currentScreen->handleEvent(event);
            }
        }
        
        if (currentScreen) {
            currentScreen->update();
        }
        
        clear();
        
        if (currentScreen) {
            currentScreen->render();
        }
        
        present();
        
        SDL_Delay(16); // ~60 FPS
    }
    
    std::cout << "Window::run() ended" << std::endl;
}

void Window::quit() {
    isRunning = false;
}

void Window::pushScreen(std::shared_ptr<Screen> screen) {
    if (currentScreen) {
        currentScreen->onExit();
        screenStack.push_back(currentScreen);
    }
    currentScreen = screen;
    if (currentScreen) {
        currentScreen->onEnter();
    }
}

void Window::popScreen() {
    if (currentScreen) {
        currentScreen->onExit();
    }
    
    if (!screenStack.empty()) {
        currentScreen = screenStack.back();
        screenStack.pop_back();
        if (currentScreen) {
            currentScreen->onEnter();
        }
    } else {
        currentScreen = nullptr;
    }
}

void Window::setScreen(std::shared_ptr<Screen> screen) {
    if (currentScreen) {
        currentScreen->onExit();
    }
    currentScreen = screen;
    if (currentScreen) {
        currentScreen->onEnter();
    }
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);
}

void Window::present() {
    SDL_RenderPresent(renderer);
}

void Window::renderText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    TTF_Font* renderFont = font ? font : this->font;
    if (!renderFont) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(renderFont, text.c_str(), color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Window::renderCenteredText(const std::string& text, int y, TTF_Font* font, SDL_Color color) {
    TTF_Font* renderFont = font ? font : this->font;
    if (!renderFont) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(renderFont, text.c_str(), color);
    if (!surface) return;
    
    int x = (width - surface->w) / 2;
    renderText(text, x, y, font, color);
    
    SDL_FreeSurface(surface);
}

void Window::renderRightAlignedText(const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    TTF_Font* renderFont = font ? font : this->font;
    if (!renderFont) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(renderFont, text.c_str(), color);
    if (!surface) return;
    
    int adjustedX = x - surface->w;
    renderText(text, adjustedX, y, font, color);
    
    SDL_FreeSurface(surface);
}

void Window::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    
    if (filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Window::drawRoundedRect(int x, int y, int w, int h, int radius, SDL_Color color, bool filled) {
    // Simple rounded rectangle implementation
    drawRect(x + radius, y, w - 2 * radius, h, color, filled);
    drawRect(x, y + radius, w, h - 2 * radius, color, filled);
    
    // Draw corner circles
    drawCircle(x + radius, y + radius, radius, color, filled);
    drawCircle(x + w - radius, y + radius, radius, color, filled);
    drawCircle(x + radius, y + h - radius, radius, color, filled);
    drawCircle(x + w - radius, y + h - radius, radius, color, filled);
}

void Window::drawLine(int x1, int y1, int x2, int y2, SDL_Color color, int thickness) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Window::drawCircle(int centerX, int centerY, int radius, SDL_Color color, bool filled) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    if (filled) {
        for (int y = -radius; y <= radius; y++) {
            for (int x = -radius; x <= radius; x++) {
                if (x * x + y * y <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
                }
            }
        }
    } else {
        // Simple circle drawing algorithm
        for (int angle = 0; angle < 360; angle++) {
            double rad = angle * M_PI / 180.0;
            int x = centerX + radius * cos(rad);
            int y = centerY + radius * sin(rad);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void Window::handleEvent(const SDL_Event& event) {
    if (currentScreen) {
        currentScreen->handleEvent(event);
    }
}

bool Window::loadFonts() {
    // Try to load fonts - you'll need to provide actual font files
    font = TTF_OpenFont("assets/fonts/arial.ttf", 16);
    largeFont = TTF_OpenFont("assets/fonts/arial.ttf", 24);
    smallFont = TTF_OpenFont("assets/fonts/arial.ttf", 12);
    
    // If fonts fail to load, try system fonts
    if (!font) {
        std::cerr << "Warning: Could not load font files. Trying system fonts..." << std::endl;
        std::cerr << "TTF Error: " << TTF_GetError() << std::endl;
        
        // Try common system font paths
        const char* fontPaths[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/System/Library/Fonts/Arial.ttf",
            "/usr/share/fonts/TTF/arial.ttf"
        };
        
        for (const char* path : fontPaths) {
            font = TTF_OpenFont(path, 16);
            if (font) {
                std::cout << "Successfully loaded font from: " << path << std::endl;
                largeFont = TTF_OpenFont(path, 24);
                smallFont = TTF_OpenFont(path, 12);
                break;
            }
        }
    }
    
    if (!font) {
        std::cerr << "Failed to load any fonts. Text rendering will not work." << std::endl;
        return false;
    }
    
    std::cout << "Fonts loaded successfully!" << std::endl;
    return true;
}

void Window::initializeColors() {
    backgroundColor = {240, 240, 240, 255};  // Light gray
    textColor = {50, 50, 50, 255};           // Dark gray
    primaryColor = {0, 120, 215, 255};       // Blue
    secondaryColor = {100, 100, 100, 255};   // Gray
    accentColor = {255, 165, 0, 255};        // Orange
    errorColor = {220, 53, 69, 255};         // Red
    successColor = {40, 167, 69, 255};       // Green
} 