#pragma once
#include "raylib.h"
#include "tmx.h"

class Tilemap;
class Tileset;

enum class Textures {
    PlayerIdleBase = 0,
    PlayerIdleBowlHair,
    PlayerRunBase,
    PlayerRunBowlHair,
    PlayerHitBase,
    PlayerHitBowlHair,
    Shadow,
    PlayerDust,
    Tiles
};

enum class Shaders {
    Water = 0
};

enum class Tilemaps {
    Main = 0
};

enum class Tilesets {
    Tiles = 0
};

Texture2D GetTexture(Textures texture);
Shader GetShader(Shaders shader);
Tilemap* GetTilemap(Tilemaps tilemap);
Tileset* GetTileset(Tilesets tileset);

void LoadAssets();
void UnloadAssets();
