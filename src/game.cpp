#include "game.h"
#include "entity/factory.h"
#include "entity/render.h"
#include "systems.h"
#include "debug.h"

Rectangle GameState::toScreen(Rectangle rect) {
    return {(rect.x - scrollX) * zoom, (rect.y - scrollY) * zoom, rect.width * zoom, rect.height * zoom};
}

void Game::Init() {
    game = GameState {
        entt::registry{},
        0,
        0,
        3
    };

    factory::newPlayer(game);
    factory::newRock(game, 16, 32);
    factory::newRock(game, 128, 128);
}

void Game::Update() {
    systems::movement(game);
    systems::collision(game);
}

void Game::Draw() {
    systems::render(game);
}