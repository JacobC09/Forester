#include "entity/factory.h"
#include "entity/render.h"

template<typename Component, typename... Args>
inline EntityBuilder& EntityBuilder::add(Args&&... args) {
    registry.emplace<Component>(entity, std::forward<Args>(args)...);
    return *this;
}

inline entt::entity EntityBuilder::build() {
    return entity;
}

// Player

entt::entity factory::newPlayer(GameState &game, float x, float y) {
    return EntityBuilder(game.reg)
        .add<Player>()
        .add<Position>(x, y)
        .add<Collider>(42, 32, 14, 10)
        .add<Physics>(0.0f, 0.0f)
        .add<Shadow>(Textures::Shadow, (unsigned char) 100)
        .add<Render>(&render::drawPlayer)
        .build();
}   

entt::entity factory::newRock(GameState &game, float x, float y) {
    return EntityBuilder(game.reg)
        .add<Rock>(GetRandomValue(0, Rock::maxTypes))
        .add<Position>(x, y)
        .add<Collider>(0, 8, 16, 8)
        .add<Render>(&render::drawRock)
        .build();
}   

entt::entity factory::newTree(GameState &game, float x, float y) {
    return EntityBuilder(game.reg)
        .add<Tree>()
        .add<Position>(x, y)
        .add<Collider>(10, 24, 12, 8)
        .add<Render>(&render::drawTree)
        .build();
}   

