#include <filesystem>
#include "assets.h"
#include "tmx.h"
#include "debug.h"

std::map<Textures, Texture2D> textures;
std::map<Shaders, Shader> shaders;
Project project;

void LoadAssets() {
    // Idle
    textures[Textures::PlayerIdleBase] = LoadTexture("assets/Graphics/Player/idle/base_idle_strip9.png");
    textures[Textures::PlayerIdleBowlHair] = LoadTexture("assets/Graphics/Player/idle/bowlhair_idle_strip9.png");

    // Run
    textures[Textures::PlayerRunBase] = LoadTexture("assets/Graphics/Player/run/base_run_strip8.png");
    textures[Textures::PlayerRunBowlHair] = LoadTexture("assets/Graphics/Player/run/bowlhair_run_strip8.png");

    // Hit
    textures[Textures::PlayerHitBase] = LoadTexture("assets/Graphics/Player/hit/base_hit_strip10.png");
    textures[Textures::PlayerHitBowlHair] = LoadTexture("assets/Graphics/Player/hit/bowlhair_hit_strip10.png");

    textures[Textures::PlayerDust] = LoadTexture("assets/Graphics/dust.png");
    textures[Textures::Shadow] = LoadTexture("assets/Graphics/shadow.png");
    textures[Textures::Tiles] = LoadTexture("assets/Graphics/Tilesets/tiles.png");

    shaders[Shaders::Water] = LoadShader("", "assets/water.fs");

    project.LoadTileset((int) Tilesets::Tiles, "assets/Map/tiles.tsx", GetTexture(Textures::Tiles));
    project.LoadTilemap((int) Tilemaps::Main, "assets/Map/main.tmx");
}

void UnloadAssets() {
    for (auto &[_, texture] : textures) {
        UnloadTexture(texture);
    }
}

Texture2D GetTexture(Textures texture) {
    return textures[texture];
}

Shader GetShader(Shaders shader) {
    return shaders[shader];
}

Tilemap* GetTilemap(Tilemaps tilemap) {
    return project.GetTilemap((int) tilemap);
}

Tileset* GetTileset(Tilesets tileset) {
    return project.GetTileset((int) tileset);
}