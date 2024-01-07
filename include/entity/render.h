#pragma once
#include "game.h"

namespace render {
    void drawPlayer(GameState &game, entt::entity entity);
    void drawRock(GameState &game, entt::entity entity);
}