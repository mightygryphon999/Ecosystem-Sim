#include <iostream>
#include "Rendering/Renderer.h"
#include "Simulation/World.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main() {
    std::cout << "Starting Program" << std::endl;

    int exitCode{ 0 };

    if (World::init() == 1) {
        exitCode = 2;
    }
    else if (Renderer::init() == false) {
        SDL_Log("Unable to activate program\n");
        exitCode = 1;
    }
    else {
        bool quit{ false };

        SDL_Event e;
        SDL_zero(e);

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }
            Renderer::present();
        }
    }

    Renderer::close();

    return exitCode;
}
