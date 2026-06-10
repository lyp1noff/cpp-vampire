#include "App.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <format>

int App::run()
{
    if (!initialize())
    {
        return 1;
    }

    while (running_)
    {
        const Uint64 currentTime = SDL_GetTicks();
        const float deltaTime = static_cast<float>(currentTime - previousTime_) / 1000.0f;
        previousTime_ = currentTime;

        processEvents();
        update(deltaTime);
        render();
    }

    shutdown();
    return 0;
}

bool App::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window_ = SDL_CreateWindow("cpp-vampire", kWindowWidth, kWindowHeight, 0);
    if (!window_)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
        return false;
    }

    configureRenderer();
    previousTime_ = SDL_GetTicks();
    return true;
}

void App::shutdown()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void App::configureRenderer()
{
#ifdef __APPLE__
    if (!SDL_SetRenderVSync(renderer_, 1))
    {
        std::cerr << "SDL_SetRenderVSync failed: " << SDL_GetError() << "\n";
    }
#endif
}

void App::processEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running_ = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
        {
            running_ = false;
        }
    }
}

void App::update(float deltaTime)
{
    const bool *keys = SDL_GetKeyboardState(nullptr);

    float dx = 0.0f;
    float dy = 0.0f;

    if (keys[SDL_SCANCODE_W])
    {
        dy -= 1.0f;
    }
    if (keys[SDL_SCANCODE_S])
    {
        dy += 1.0f;
    }
    if (keys[SDL_SCANCODE_A])
    {
        dx -= 1.0f;
    }
    if (keys[SDL_SCANCODE_D])
    {
        dx += 1.0f;
    }

    const float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f)
    {
        dx /= length;
        dy /= length;
    }

    player_.x += dx * player_.speed * deltaTime;
    player_.y += dy * player_.speed * deltaTime;

    // float playerCenterX = player_.x + player_.w * 0.5f;
    // float playerCenterY = player_.y + player_.h * 0.5f;

    // camera_.x = playerCenterX - kWindowWidth / 2;
    // camera_.y = playerCenterY - kWindowHeight / 2;

    // std::cout << std::format("Cam X:{} Y:{} | Player X:{} Y:{}", camera_.x, camera_.y, player_.x, player_.y) << std::endl;

    // player_.x = std::clamp(player_.x, 0.0f, static_cast<float>(kWindowWidth) - player_.w);
    // player_.y = std::clamp(player_.y, 0.0f, static_cast<float>(kWindowHeight) - player_.h);
}

void App::render()
{
    SDL_SetRenderDrawColor(renderer_, 15, 15, 20, 255);
    SDL_RenderClear(renderer_);

    const SDL_FRect playerRect{
        player_.x,
        player_.y,
        player_.w,
        player_.h};

    SDL_SetRenderDrawColor(renderer_, 80, 180, 255, 255);
    SDL_RenderFillRect(renderer_, &playerRect);
    SDL_RenderPresent(renderer_);
}
