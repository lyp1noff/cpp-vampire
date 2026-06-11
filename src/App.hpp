#pragma once

#include <SDL3/SDL.h>
#include <vector>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Camera.hpp"

class App
{
public:
    int run();

private:
    static constexpr int kWindowWidth = 1280;
    static constexpr int kWindowHeight = 720;

    bool initialize();
    void shutdown();
    void configureRenderer();
    void processEvents();
    void update(float deltaTime);
    void render();

    void addEnemies();

    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    bool running_ = true;
    Uint64 previousTime_ = 0;
    Player player_{};
    Camera camera_{};
    std::vector<Enemy> enemies_{};
};
