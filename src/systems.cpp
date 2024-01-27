#include "debug.h"
#include "systems.h"
#include "components.h"
#include "tmx.h"

std::vector<entt::entity> GetCollidingEntities(Rectangle rect, entt::view<entt::get_t<Position, Collider>> view) {
    std::vector<entt::entity> collisions;
    
    for (auto [e, pos, collider] : view.each()) {
        if (CheckCollisionRecs(rect, {pos.x + collider.x, pos.y + collider.y, (float) collider.width, (float) collider.height})) {
            collisions.push_back(e);
        }
    }

    return collisions;
}

inline int GetDepth(const entt::registry& reg, const entt::entity& e) {
    int depth = reg.get<Position>(e).y;

    if (reg.all_of<Collider>(e)) {
        auto cldr = reg.get<Collider>(e);
        depth += cldr.y + cldr.height;
    }

    return depth;
}

void systems::render(GameState &game) {
    Rectangle screenRect = game.cam.getScreenRect();
    auto view = game.reg.view<Position, Render>();
    std::vector<entt::entity> visible;

    for (auto [entity, pos, render] : view.each()) {
        Rectangle rect = {
            pos.x - 64,
            pos.y - 64,
            128,
            128
        };
        
        if (CheckCollisionRecs(screenRect, rect))
            visible.push_back(entity);
    }

    int calls = 0;
    entt::insertion_sort{}(visible.begin(), visible.end(), [&](entt::entity& a, entt::entity& b) {
        calls++;
        return GetDepth(game.reg, a) < GetDepth(game.reg, b);
    });

    for (auto entity : visible) {
        auto [position, render] = view.get(entity);
        render.renderFunction(game, entity);
    }
}

void systems::movement(GameState &game) {
    auto view = game.reg.view<Player, Physics>();

    for (auto [e, player, physics] : view.each()) {
        physics.xVel = physics.yVel = 0;

        bool canMove = player.animations.state != Player::Hit;

        if (!canMove)
            continue;
        
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

        int nextState = player.animations.state;

        float magnitude = std::sqrt(physics.xVel * physics.xVel + physics.yVel * physics.yVel);
        if (magnitude != 0.0) {
            physics.xVel = (physics.xVel / magnitude) * player.speed;
            physics.yVel = (physics.yVel / magnitude) * player.speed;

            nextState = Player::Run;
        } else {
            nextState = Player::Idle;
        }

        if (physics.atWall && player.animations.state == Player::Run && !(physics.xVel && physics.yVel)) {
            nextState = Player::Idle;
        }

        player.animations.setState(nextState);
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

            for (auto e : GetCollidingEntities(rect, colliders)) {
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

            for (auto rect2 : GetCollidingTiles(rect, game.map)) {
                if (!CheckCollisionRecs(rect, rect2)) continue;

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
        
            for (auto e : GetCollidingEntities(rect, colliders)) {
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

            for (auto rect2 : GetCollidingTiles(rect, game.map)) {
                if (!CheckCollisionRecs(rect, rect2)) continue;

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
        pos.x = std::round(rect.x - collider.x);
        pos.y = std::round(rect.y - collider.y);
    }
}

void systems::hit(GameState &game) {
    auto view = game.reg.view<Player>();

    for (auto [entity, player] : view.each()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            player.animations.setState(Player::Hit);
        } 

        if (player.animations.state == Player::Hit) {
            if (player.animations.getAnimation()->isFinished()) {
                player.animations.setState(Player::Idle);
            }
        }
    }
}
