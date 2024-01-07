#include "entity/render.h"
#include "components.h"
#include "debug.h"

// Player

void render::drawPlayer(GameState &game, entt::entity entity) {
    auto [pos, player, collider] = game.reg.get<Position, Player, Collider>(entity);

    Animation* animation = player.animations.getAnimation();
    Texture2D texture = GetTexture(animation->texture);
    Rectangle source = animation->getFrame();
    Rectangle dest = game.toScreen({pos.x, pos.y, (float) source.width, (float) source.height});

    if (player.left)
        source.width = -source.width;

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
    animation->update();
}

void render::drawRock(GameState &game, entt::entity entity) {
    auto [pos, rect] = game.reg.get<Position, Collider>(entity);

    DrawRectangleRec(game.toScreen({pos.x + rect.x, pos.y + rect.y, (float) rect.width, (float) rect.height}), GRAY);
}