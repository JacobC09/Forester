#pragma once
#include "raylib.h"

enum class Textures {
    PlayerIdle,
    PlayerRun
};

void LoadTextures();
void UnloadTextures();

Texture2D GetTexture(Textures texture);
