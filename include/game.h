#pragma once
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "map.h"

const Color waterColor = {60, 163, 178, 255};

enum GameLayers {
    Water,
    Ground,
    Decor
};

struct GameState {
    Cam cam;
    Tilemap* map;
    entt::registry reg;
    entt::entity player;
    float time;
    
    GameState();
};

class Game {
public:
    RenderTexture2D target;
    GameState game;

    int waterTimeLoc;

    Game();

    void Init();
    void Update();
    void Draw();
};