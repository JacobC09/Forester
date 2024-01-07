#pragma once
#include "animation.h"
#include "textures.h"

// Game Objects

struct Player {
    enum Animations {
        Idle,
        Run
    };

    float speed;
    AnimationPlayer animations;
    bool left;
};

struct Rock {};

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

struct Render {
    std::function<void(GameState&, entt::entity entity)> renderFunction;
};
