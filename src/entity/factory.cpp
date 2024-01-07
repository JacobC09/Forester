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

entt::entity factory::newPlayer(GameState &game) {
    AnimationPlayer animations = AnimationPlayer({
        {Player::Idle, Animation {Textures::PlayerIdle, {0, 0, 96, 64}, 8, 9}},
        {Player::Run, Animation {Textures::PlayerRun, {0, 0, 96, 64}, 4, 8}},
    }, Player::Idle);

    return EntityBuilder(game.reg)
        .add<Player>(2.0f, animations, false)
        .add<Position>(64.0f, 64.0f)
        .add<Collider>(42, 32, 14, 10)
        .add<Physics>(0.0f, 0.0f)
        .add<Render>(&render::drawPlayer)
        .build();
}   

entt::entity factory::newRock(GameState &game, float x, float y) {
    return EntityBuilder(game.reg)
        .add<Rock>()
        .add<Position>(x, y)
        .add<Collider>(0, 0, 16, 16)
        .add<Render>(&render::drawRock)
        .build();
}   
