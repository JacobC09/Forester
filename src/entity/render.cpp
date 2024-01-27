#include "entity/render.h"
#include "components.h"
#include "debug.h"

// Player

Textures GetHairTexture(Player &player) {
    switch (player.animations.state) {
        case Player::Idle:
            switch (player.hairtype) {
                case Player::HairType::Bowl:
                    return Textures::PlayerIdleBowlHair;
                default: break;
            }
            break;

        case Player::Run:
            switch (player.hairtype) {
                case Player::HairType::Bowl:
                    return Textures::PlayerRunBowlHair;
                default: break;

            }
            break;

        case Player::Hit:
            switch (player.hairtype) {
                case Player::HairType::Bowl:
                    return Textures::PlayerHitBowlHair;
                default: break;

            }
            break;

        default:
            break;
    }

    return (Textures) NULL;
}

void render::drawPlayer(GameState &game, entt::entity entity) {
    auto [pos, player, collider] = game.reg.get<Position, Player, Collider>(entity);

    Animation* animation = player.animations.getAnimation();
    Texture2D texture = GetTexture(animation->texture);
    Rectangle source = animation->getFrame();
    Rectangle dest = game.cam.toScreen({
        pos.x, 
        pos.y,
        (float) source.width, 
        (float) source.height
    });

    if (player.left)
        source.width = -source.width;

    if (player.animations.state == Player::Run)
        DrawTexturePro(GetTexture(Textures::PlayerDust), source, dest, {0, 0}, 0, WHITE);

    render::drawShadow(game, entity, dest);

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
    DrawTexturePro(GetTexture(GetHairTexture(player)), source, dest, {0, 0}, 0, WHITE);

    animation->update();
}

void render::drawRock(GameState &game, entt::entity entity) {
    auto [rock, pos] = game.reg.get<Rock, Position>(entity);
    Rectangle source = {960.0f + (rock.type % 3) * 16, std::floor(rock.type / 3.0f), 16, 16};
    Rectangle dest = game.cam.toScreen(Rectangle {pos.x, pos.y, (float) source.width, (float) source.height});

    DrawTexturePro(GetTexture(Textures::Tiles), source, dest, {0, 0}, 0, WHITE);
}

void render::drawTree(GameState &game, entt::entity entity) {
    auto pos = game.reg.get<Position>(entity);
    Rectangle source = {880, 32, 32, 32};
    Rectangle dest = game.cam.toScreen(Rectangle {pos.x, pos.y, (float) source.width, (float) source.height});

    DrawTexturePro(GetTexture(Textures::Tiles), source, dest, {0, 0}, 0, WHITE);
}

void render::drawShadow(GameState& game, entt::entity entity, Rectangle dest) {
    auto shadow = game.reg.get<Shadow>(entity);
    auto texture = GetTexture(shadow.texture);

    Rectangle source = {0, 0, (float) texture.width, (float) texture.height};

    DrawTexturePro(texture, source, dest, {0, 0}, 0, Color {0, 0, 0, shadow.transparency});
}
