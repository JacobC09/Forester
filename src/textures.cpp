#include <map>
#include "textures.h"

std::map<Textures, Texture2D> textures;
std::map<Textures, const char*> texturePaths = {
    {Textures::PlayerIdle, "assets/Player/idle/base_idle_strip9.png"},
    {Textures::PlayerRun, "assets/Player/run/base_run_strip8.png"}
};

void LoadTextures() {
    for (auto &[texture, path] : texturePaths) {
        textures[texture] = LoadTexture(path);
    }
}

void UnloadTextures() {
    for (auto &[_, texture] : textures) {
        UnloadTexture(texture);
    }
}

Texture2D GetTexture(Textures texture) {
    return textures[texture];
}