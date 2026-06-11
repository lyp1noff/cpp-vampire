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
    addEnemies();
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

    player_.position.x += dx * player_.speed * deltaTime;
    player_.position.y += dy * player_.speed * deltaTime;

    float playerCenterX = player_.position.x + player_.w * 0.5f;
    float playerCenterY = player_.position.y + player_.h * 0.5f;

    camera_.position.x = playerCenterX - kWindowWidth / 2;
    camera_.position.y = playerCenterY - kWindowHeight / 2;

    // std::cout << std::format("Cam X:{} Y:{} | Player X:{} Y:{}", camera_.position.x, camera_.position.y, player_.position.x, player_.position.y) << std::endl;
}

Vec2 worldToScreen(Vec2 pos, Vec2 cameraPosition)
{
    return {pos.x - cameraPosition.x, pos.y - cameraPosition.y};
}

void App::addEnemies()
{
    for (int i = 0; i < 5; i++)
    {
        Enemy en = {
            .position = {
                static_cast<float>(i) * 50 + 50,
                40},
        };
        enemies_.push_back(en);
    }
}

void App::render()
{
    SDL_SetRenderDrawColor(renderer_, 15, 15, 20, 255);
    SDL_RenderClear(renderer_);

    Vec2 playerScreenPos = worldToScreen(player_.position, camera_.position);
    const SDL_FRect playerRect{
        playerScreenPos.x,
        playerScreenPos.y,
        player_.w,
        player_.h};

    SDL_SetRenderDrawColor(renderer_, 80, 180, 255, 255);
    SDL_RenderFillRect(renderer_, &playerRect);

    for (const auto &enemy : enemies_)
    {
        Vec2 objectScreenPos = worldToScreen({enemy.position.x, enemy.position.y}, camera_.position);
        const SDL_FRect screenRect{
            objectScreenPos.x,
            objectScreenPos.y,
            enemy.w,
            enemy.h};

        SDL_RenderFillRect(renderer_, &screenRect);
    }

    SDL_RenderPresent(renderer_);
}
