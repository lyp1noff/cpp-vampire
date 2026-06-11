#include "App.hpp"

#include <cmath>
#include <iostream>

int App::run() {
    if (!initialize()) {
        return 1;
    }

    while (running_) {
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

bool App::initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window_ = SDL_CreateWindow("cpp-vampire", kWindowWidth, kWindowHeight, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_) {
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

void App::shutdown() const {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void App::configureRenderer() const {
    if (!SDL_SetRenderLogicalPresentation(renderer_, kWindowWidth, kWindowHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        std::cerr << "SDL_SetRenderLogicalPresentation failed: " << SDL_GetError() << "\n";
    }
#ifdef __APPLE__
    if (!SDL_SetRenderVSync(renderer_, 1)) {
        std::cerr << "SDL_SetRenderVSync failed: " << SDL_GetError() << "\n";
    }
#endif
}

void App::processEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running_ = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
            running_ = false;
        }
    }
}

void resolveEnemySeparation(Rect &A, Rect &B) {
    Vec2 centerA = A.Center();
    Vec2 centerB = B.Center();
    Vec2 delta = {centerB.x - centerA.x, centerB.y - centerA.y};

    float radiusA = A.w * 0.5f;
    float radiusB = B.w * 0.5f;
    float minDistance = radiusA + radiusB;

    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    if (distance >= minDistance)
        return;

    if (distance < 0.001f)
        return;

    float overlap = minDistance - distance;
    Vec2 normal = {delta.x / distance, delta.y / distance};

    float push = overlap * 0.5f;
    A.x -= normal.x * push;
    A.y -= normal.y * push;
    B.x += normal.x * push;
    B.y += normal.y * push;
}

void App::update(float deltaTime) {
    const bool *keys = SDL_GetKeyboardState(nullptr);

    float dx = 0.0f;
    float dy = 0.0f;

    if (keys[SDL_SCANCODE_W]) {
        dy -= 1.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        dy += 1.0f;
    }
    if (keys[SDL_SCANCODE_A]) {
        dx -= 1.0f;
    }
    if (keys[SDL_SCANCODE_D]) {
        dx += 1.0f;
    }

    const float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0.0f) {
        dx /= length;
        dy /= length;
    }

    player_.rect.x += dx * player_.speed * deltaTime;
    player_.rect.y += dy * player_.speed * deltaTime;

    Vec2 playerCenter = player_.rect.Center();

    camera_.position.x = playerCenter.x - kWindowWidth * 0.5f;
    camera_.position.y = playerCenter.y - kWindowHeight * 0.5f;

    for (auto &enemy: enemies_) {
        Vec2 enemyCenter = enemy.rect.Center();
        Vec2 toPlayer = {playerCenter.x - enemyCenter.x, playerCenter.y - enemyCenter.y};

        const float toPlayerLength = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
        if (toPlayerLength > 0.0f) {
            toPlayer.x /= toPlayerLength;
            toPlayer.y /= toPlayerLength;
        }
        enemy.rect.x += toPlayer.x * enemy.speed * deltaTime;
        enemy.rect.y += toPlayer.y * enemy.speed * deltaTime;

        // std::cout << checkCollision(player_.rect, enemy.rect) << std::endl;
    }

    for (int i = 0; i < enemies_.size(); i++) {
        for (int j = i + 1; j < enemies_.size(); j++) {
            resolveEnemySeparation(enemies_[i].rect, enemies_[j].rect);
        }
    }
}

Vec2 worldToScreen(Vec2 pos, Vec2 cameraPosition) {
    return {pos.x - cameraPosition.x, pos.y - cameraPosition.y};
}

void App::addEnemies() {
    for (int i = 0; i < 5; i++) {
        Enemy enemy = {
            .rect = {
                static_cast<float>(i) * 50 + 50,
                40.0f,
                40.0f,
                40.0f,
            }
        };
        enemies_.push_back(enemy);
    }
}

void App::render() const {
    SDL_SetRenderDrawColor(renderer_, 15, 15, 20, 255);
    SDL_RenderClear(renderer_);

    Vec2 playerScreenPos = worldToScreen(player_.rect.Position(), camera_.position);
    const SDL_FRect playerRect{
        playerScreenPos.x,
        playerScreenPos.y,
        player_.rect.w,
        player_.rect.h
    };

    SDL_SetRenderDrawColor(renderer_, 80, 180, 255, 255);
    SDL_RenderFillRect(renderer_, &playerRect);

    for (const auto &enemy: enemies_) {
        Vec2 enemyScreenPos = worldToScreen(enemy.rect.Position(), camera_.position);
        const SDL_FRect screenRect{
            enemyScreenPos.x,
            enemyScreenPos.y,
            enemy.rect.w,
            enemy.rect.h
        };
        SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer_, &screenRect);
    }

    SDL_RenderPresent(renderer_);
}
