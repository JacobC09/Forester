#pragma once
#include "entt/entity/registry.hpp"
#include "raylib.h"

struct GameState {
    entt::registry reg;
    int scrollX, scrollY;
    float zoom;

    Rectangle toScreen(Rectangle rect);
};

class Game {
public:
    GameState game;

    void Init();
    void Update();
    void Draw();
};