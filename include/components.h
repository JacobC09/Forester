#pragma once
#include "game.h"
#include "animation.h"
#include "assets.h"
#include <functional>

// Game Objects

struct Player {
    enum Animations {
        Idle,
        Run,
        Hit
    };

    enum HairType {
        Bowl
    };
    
    Player();

    bool left;
    float speed;
    HairType hairtype;
    AnimationPlayer animations;
};

struct Rock {
    int type;

    static const int maxTypes = 9;
};

struct Tree {};

// Components

struct Position {
    float x, y;
};

struct Physics {
    float xVel, yVel;
    bool atWall = false;
};

struct Collider {
    int x, y, width, height;
};

struct Sprite {
    Textures texture;
};

using RenderFunction = std::function<void(GameState&, entt::entity entity)>;

struct Render {
    RenderFunction renderFunction;
};

struct Shadow {
    Textures texture;
    unsigned char transparency = 255;
};