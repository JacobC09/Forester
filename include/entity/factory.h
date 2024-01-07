#pragma once
#include "game.h"
#include "components.h"

class EntityBuilder {
public:
    EntityBuilder(entt::registry& reg) : registry(reg), entity(reg.create()) {}

    template<typename Component, typename... Args>
    inline EntityBuilder& add(Args&&... args);
    inline entt::entity build();

private:
    entt::registry& registry;
    entt::entity entity;
};

namespace factory
{
    entt::entity newPlayer(GameState &game);
    entt::entity newRock(GameState &game, float x, float y);
}
