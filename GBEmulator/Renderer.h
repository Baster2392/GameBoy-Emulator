#include <SDL.h>
#include <iostream>
#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void render(const uint8_t* framebuffer);
    bool handleEvents();

private:
    SDL_Window* window;
    SDL_Renderer* sdl_renderer;
    SDL_Texture* texture;
    int screenWidth = 160, screenHeight = 144, scale = 3;
    uint8_t fb[160 * 144];
};
