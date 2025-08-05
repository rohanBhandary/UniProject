#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <vector>
#include <string>

class Screen;

class Window {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* largeFont;
    TTF_Font* smallFont;
    
    int width;
    int height;
    bool isRunning;
    
    std::shared_ptr<Screen> currentScreen;
    std::vector<std::shared_ptr<Screen>> screenStack;
    
    // Colors
    SDL_Color backgroundColor;
    SDL_Color textColor;
    SDL_Color primaryColor;
    SDL_Color secondaryColor;
    SDL_Color accentColor;
    SDL_Color errorColor;
    SDL_Color successColor;

public:
    Window(const std::string& title, int w, int h);
    ~Window();
    
    // Window management
    bool initialize();
    void run();
    void quit();
    
    // Screen management
    void pushScreen(std::shared_ptr<Screen> screen);
    void popScreen();
    void setScreen(std::shared_ptr<Screen> screen);
    std::shared_ptr<Screen> getCurrentScreen() const { return currentScreen; }
    
    // Rendering
    void clear();
    void present();
    SDL_Renderer* getRenderer() const { return renderer; }
    
    // Text rendering
    void renderText(const std::string& text, int x, int y, TTF_Font* font = nullptr, SDL_Color color = {0, 0, 0, 255});
    void renderCenteredText(const std::string& text, int y, TTF_Font* font = nullptr, SDL_Color color = {0, 0, 0, 255});
    void renderRightAlignedText(const std::string& text, int x, int y, TTF_Font* font = nullptr, SDL_Color color = {0, 0, 0, 255});
    
    // Drawing utilities
    void drawRect(int x, int y, int w, int h, SDL_Color color, bool filled = true);
    void drawRoundedRect(int x, int y, int w, int h, int radius, SDL_Color color, bool filled = true);
    void drawLine(int x1, int y1, int x2, int y2, SDL_Color color, int thickness = 1);
    void drawCircle(int centerX, int centerY, int radius, SDL_Color color, bool filled = true);
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    TTF_Font* getFont() const { return font; }
    TTF_Font* getLargeFont() const { return largeFont; }
    TTF_Font* getSmallFont() const { return smallFont; }
    
    // Colors
    SDL_Color getBackgroundColor() const { return backgroundColor; }
    SDL_Color getTextColor() const { return textColor; }
    SDL_Color getPrimaryColor() const { return primaryColor; }
    SDL_Color getSecondaryColor() const { return secondaryColor; }
    SDL_Color getAccentColor() const { return accentColor; }
    SDL_Color getErrorColor() const { return errorColor; }
    SDL_Color getSuccessColor() const { return successColor; }
    
    // Event handling
    void handleEvent(const SDL_Event& event);
    
private:
    bool loadFonts();
    void initializeColors();
}; 