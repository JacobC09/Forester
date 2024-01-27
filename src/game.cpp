#include "game.h"
#include "entity/factory.h"
#include "entity/render.h"
#include "systems.h"
#include "debug.h"

GameState::GameState() {
    cam = Cam(0, 0, 3.0f);
    map = GetTilemap(Tilemaps::Main);
    reg = entt::registry{};
    time = 0;
    
}

Game::Game() {
    target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    waterTimeLoc = GetShaderLocation(GetShader(Shaders::Water), "time");
};

void Game::Init() {
    game = GameState();

    for (auto &obj : game.map->objects) {
        if (obj.name == "player") {
            game.player = factory::newPlayer(game, obj.x, obj.y);
        } else if (obj.name == "rock") {
            factory::newRock(game, obj.x, obj.y);
        } else if (obj.name == "tree") {
            factory::newTree(game, obj.x, obj.y);
        }
    }
}

void Game::Update() {
    game.time++;

    SetShaderValue(GetShader(Shaders::Water), waterTimeLoc, &game.time, SHADER_UNIFORM_FLOAT);

    systems::movement(game);
    systems::collision(game);
    systems::hit(game);

    auto [pos, collider] = game.reg.get<Position, Collider>(game.player);
    Vector2 centerPos = {pos.x + collider.x + collider.width / 2, pos.y + collider.y + collider.height / 2};
    game.cam.track(centerPos, 20.0f);
    game.cam.maxView(0, 0, game.map->width * game.map->tileWidth, game.map->height * game.map->tileHeight);
}

void Game::Draw() {
    BeginTextureMode(target);
        ClearBackground(waterColor);
        DrawLayer(game.map, Water, game.cam, game.time, false);
    EndTextureMode();

    BeginShaderMode(GetShader(Shaders::Water));
        DrawTextureRec(target.texture, {0, 0, (float) target.texture.width, (float) -target.texture.height}, {0, 0}, WHITE);
    EndShaderMode();

    DrawLayer(game.map, Ground, game.cam, game.time, false);
    DrawLayer(game.map, Decor, game.cam, game.time, false);
    
    systems::render(game);
}