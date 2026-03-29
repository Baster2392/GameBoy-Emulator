#include "Renderer.h"

Renderer::Renderer() {

    window = SDL_CreateWindow(
        "GameBoy Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth * scale,
        screenHeight * scale + TOOLBAR_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Cannot create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_renderer) {
        std::cerr << "Cannot create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTexture(
        sdl_renderer,
        SDL_PIXELFORMAT_RGB332,
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth,
        screenHeight
    );

    if (!texture) {
        std::cerr << "Cannot create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    buttons.emplace_back(5, 5, 100, 30, "LOAD ROM", UIAction::LoadROM);
    buttons.emplace_back(110, 5, 90, 30, "PAUSE", UIAction::TogglePause);
    buttons.emplace_back(205, 5, 80, 30, "RESET", UIAction::Reset);
    buttons.emplace_back(290, 5, 70, 30, "QUIT", UIAction::Quit);
}

Renderer::~Renderer() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
}

void Renderer::render(const uint8_t* framebuffer)
{
    SDL_UpdateTexture(texture, nullptr, framebuffer, screenWidth);

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer);

    // Draw toolbar background
    SDL_SetRenderDrawColor(sdl_renderer, 40, 40, 40, 255);
    SDL_Rect toolbarRect = { 0, 0, screenWidth * scale, TOOLBAR_HEIGHT };
    SDL_RenderFillRect(sdl_renderer, &toolbarRect);

    int mx = mouseX.load();
    int my = mouseY.load();
    bool isPaused = paused.load();
    bool isRomLoaded = romLoaded.load();

    // Draw buttons
    for (const auto& btn : buttons) {
        bool hovered = btn.contains(mx, my);
        bool active = (btn.action == UIAction::TogglePause && isPaused && isRomLoaded);

        if (btn.action == UIAction::TogglePause && isPaused && isRomLoaded) {
            // Show "RESUME" label when paused
            UIButton resumeBtn = btn;
            resumeBtn.label = "RESUME";
            drawButton(sdl_renderer, resumeBtn, hovered, active);
        } else {
            drawButton(sdl_renderer, btn, hovered, active);
        }
    }

    // Draw keyboard shortcut hints
    SDL_SetRenderDrawColor(sdl_renderer, 90, 90, 90, 255);
    drawText(sdl_renderer, 370, 10, "F5 F6 F7");
    drawText(sdl_renderer, 370, 22, "ESC=Quit");

    // Draw separator line
    SDL_SetRenderDrawColor(sdl_renderer, 80, 80, 80, 255);
    SDL_RenderDrawLine(sdl_renderer, 0, TOOLBAR_HEIGHT - 1, screenWidth * scale, TOOLBAR_HEIGHT - 1);

    // Draw game framebuffer below toolbar
    SDL_Rect destRect = { 0, TOOLBAR_HEIGHT, screenWidth * scale, screenHeight * scale };
    SDL_RenderCopy(sdl_renderer, texture, nullptr, &destRect);

    // Draw pause/status overlay
    if (isPaused) {
        SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(sdl_renderer, &destRect);

        SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
        if (!isRomLoaded) {
            // "LOAD A ROM" = 10 chars * 16px = 160px
            int textX = (screenWidth * scale - 10 * 16) / 2;
            int textY = TOOLBAR_HEIGHT + (screenHeight * scale - 16) / 2;
            drawText(sdl_renderer, textX, textY, "LOAD A ROM", 2);
        } else {
            // "PAUSED" = 6 chars * 16px = 96px
            int textX = (screenWidth * scale - 6 * 16) / 2;
            int textY = TOOLBAR_HEIGHT + (screenHeight * scale - 16) / 2;
            drawText(sdl_renderer, textX, textY, "PAUSED", 2);
        }
    }

    SDL_RenderPresent(sdl_renderer);
}
