#include "Renderer.h"

Renderer::Renderer() {

    window = SDL_CreateWindow(
        "GameBoy Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth * scale,
        screenHeight * scale,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Nie uda³o siê stworzyæ okna: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl_renderer) {
        std::cerr << "Nie uda³o siê stworzyæ renderera: " << SDL_GetError() << std::endl;
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
        std::cerr << "Nie uda³o siê stworzyæ tekstury: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

Renderer::~Renderer() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
}

void Renderer::render(const uint8_t* framebuffer)
{
    SDL_UpdateTexture(texture, nullptr, framebuffer, screenWidth);
    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer);
}

bool Renderer::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}
