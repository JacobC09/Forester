#pragma once
#include "game.h"



namespace render {
    void drawPlayer(GameState &game, entt::entity entity);
    void drawRock(GameState &game, entt::entity entity);
    void drawTree(GameState& game, entt::entity entity);
    void drawShadow(GameState& game, entt::entity entity, Rectangle dest);
}