#include <iostream>
#include "Rendering/Renderer.h"
#include "Simulation/World.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Rendering/SimulationInteractions.h"
#include "config.h"

int frameCount{ 0 };
Uint64 pastTime{ 0 };
float fps{ 0.0f };

int frame_rate() {
    frameCount++;

    Uint64 currentTime = SDL_GetTicks();
    if (currentTime - pastTime >= 1000) {
        fps = frameCount / ((currentTime - pastTime) / 1000.0f);
        pastTime = currentTime;
        frameCount = 0;
    }

    return 0;
}

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

        float mouseX{ 0.0f };
        float mouseY{ 0.0f };

        float mouseScrollY{ 0.0f };

        while (quit == false) {
            frame_rate();
            mouseScrollY = 0;
            while (SDL_PollEvent(&e) == true) {
                Renderer::handle_input(e);
                SimulationInteractions::mouse_interaction(&e, mouseX, mouseY);
                switch (e.type) {
                    case SDL_EVENT_QUIT: {
                        quit = true;
                        break;
                    }
                    case SDL_EVENT_MOUSE_MOTION: {
                        mouseX = e.motion.x;
                        mouseY = e.motion.y;
                        break;
                    }
                    case SDL_EVENT_MOUSE_WHEEL: {
                        mouseScrollY = e.wheel.y;
                        break;
                    }
                }
            }
            Renderer::present(mouseX, mouseY, mouseScrollY, fps);
            World::simulation_step(true);
        }
    }

    Renderer::close();

    return exitCode;
}
