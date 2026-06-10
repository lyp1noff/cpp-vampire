#include <SDL3/SDL.h>
#include <cmath>
#include <iostream>

struct Player
{
    float x = 640.0f;
    float y = 360.0f;
    float w = 40.0f;
    float h = 40.0f;
    float speed = 300.0f; // pixels per second
};

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "cpp-vampire",
        1280,
        720,
        0);

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    Player player;

    Uint64 previousTime = SDL_GetTicks();

    while (running)
    {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = static_cast<float>(currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                {
                    running = false;
                }
            }
        }

        const bool *keys = SDL_GetKeyboardState(nullptr);

        float dx = 0.0f;
        float dy = 0.0f;

        if (keys[SDL_SCANCODE_W])
            dy -= 1.0f;
        if (keys[SDL_SCANCODE_S])
            dy += 1.0f;
        if (keys[SDL_SCANCODE_A])
            dx -= 1.0f;
        if (keys[SDL_SCANCODE_D])
            dx += 1.0f;

        float length = std::sqrt(dx * dx + dy * dy);

        if (length > 0.0f)
        {
            dx /= length;
            dy /= length;
        }

        player.x += dx * player.speed * deltaTime;
        player.y += dy * player.speed * deltaTime;

        // Keep player inside screen
        if (player.x < 0.0f)
            player.x = 0.0f;
        if (player.y < 0.0f)
            player.y = 0.0f;
        if (player.x + player.w > 1280.0f)
            player.x = 1280.0f - player.w;
        if (player.y + player.h > 720.0f)
            player.y = 720.0f - player.h;

        SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
        SDL_RenderClear(renderer);

        SDL_FRect playerRect{
            player.x,
            player.y,
            player.w,
            player.h};

        SDL_SetRenderDrawColor(renderer, 80, 180, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}