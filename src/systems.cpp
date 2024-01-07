#include "debug.h"
#include "systems.h"
#include "components.h"

std::vector<entt::entity> getColliding(Rectangle rect, entt::view<entt::get_t<Position, Collider>> view) {
    std::vector<entt::entity> collisions;
    
    for (auto [e, pos, collider] : view.each()) {
        if (CheckCollisionRecs(rect, {pos.x + collider.x, pos.y + collider.y, (float) collider.width, (float) collider.height})) {
            collisions.push_back(e);
        }
    }

    return collisions;
}

void systems::render(GameState &game) {
    game.reg.sort<Position>([](const auto &left, const auto &right) {
        return right.y < right.y;
    });

    auto view = game.reg.view<Render>();

    for (auto [entity, render] : view.each()) {
        render.renderFunction(game, entity);
    }
}

void systems::movement(GameState &game) {
    auto view = game.reg.view<Player, Physics>();

    for (auto [e, player, physics] : view.each()) {
        physics.xVel = physics.yVel = 0;

        if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) {
            physics.xVel += player.speed;
            player.left = false;
        } else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
            physics.xVel -= player.speed;
            player.left = true;
        }

        if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) {
            physics.yVel -= player.speed;
        } else if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)) {
            physics.yVel += player.speed;
        }

        float magnitude = std::sqrt(physics.xVel * physics.xVel + physics.yVel * physics.yVel);
        if (magnitude != 0.0) {
            physics.xVel /= magnitude;
            physics.yVel /= magnitude;

            player.animations.setState(Player::Run);
        } else {
            player.animations.setState(Player::Idle);
        }

        if (physics.atWall && player.animations.state == Player::Run && !(physics.xVel && physics.yVel)) {
            player.animations.setState(Player::Idle);
        }
    }
}

void systems::collision(GameState &game) {
    auto colliders = game.reg.view<Position, Collider>();
    auto targets = game.reg.view<Physics, Position, Collider>();

    // Loop through all the targets

    for (auto [target, physics, pos, collider] : targets.each()) {
        physics.atWall = false;
        Rectangle rect = {pos.x + collider.x, pos.y + collider.y, (float) collider.width, (float) collider.height};

        // Add x velocity

        if (physics.xVel) {
            rect.x += physics.xVel;

            // Loop through all other colliding entities

            for (auto e : getColliding(rect, colliders)) {
                if (e == target) continue;

                auto [pos2, collider2] = game.reg.get<Position, Collider>(e);
                Rectangle rect2 = {pos2.x + collider2.x, pos2.y + collider2.y, (float) collider2.width, (float) collider2.height};

                // Adjust position depending on velocity

                if (physics.xVel > 0 && rect.x + rect.width > rect2.x) {
                    rect.x = rect2.x - rect.width;
                    physics.atWall = true;
                } else if (physics.xVel < 0 && rect.x < rect2.x + rect2.width) {
                    rect.x = rect2.x + rect2.width;
                    physics.atWall = true;
                }
            }
        }

        // Add y velocity

        if (physics.yVel) {
            rect.y += physics.yVel;

            // Loop through all other colliding entities
        
            for (auto e : getColliding(rect, colliders)) {
                if (e == target) continue;

                auto [pos2, collider2] = game.reg.get<Position, Collider>(e);
                Rectangle rect2 = {pos2.x + collider2.x, pos2.y + collider2.y, (float) collider2.width, (float) collider2.height};

                // Adjust position depending on velocity

                if (physics.yVel > 0 && rect.y + rect.height > rect2.y) {
                    rect.y = rect2.y - rect.height;
                    physics.atWall = true;
                } else if (physics.yVel < 0 && rect.y < rect2.y + rect2.height) {
                    rect.y = rect2.y + rect2.height;
                    physics.atWall = true;
                }
            }
        }

        // Re assign position to new position
        pos.x = rect.x - collider.x;
        pos.y = rect.y - collider.y;
    }
}


