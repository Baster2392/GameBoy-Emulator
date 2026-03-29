#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <atomic>
#include <string>
#include "UI.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void render(const uint8_t* framebuffer);

    SDL_Window* getWindow() { return window; }

    std::vector<UIButton> buttons;
    std::atomic<int> mouseX{ 0 }, mouseY{ 0 };
    std::atomic<bool> paused{ true };
    std::atomic<bool> romLoaded{ false };

private:
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* texture;
    int screenWidth = 160, screenHeight = 144, scale = 3;
};
