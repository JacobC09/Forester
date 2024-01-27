#include "components.h"

Player::Player() {
    animations = AnimationPlayer({
        {Player::Idle, Animation {Textures::PlayerIdleBase, {0, 0, 96, 64}, 8, 9}},
        {Player::Run, Animation {Textures::PlayerRunBase, {0, 0, 96, 64}, 4, 8}},
        {Player::Hit, Animation {Textures::PlayerHitBase, {0, 0, 96, 64}, 4, 10, false}},
    }, Player::Idle);

    animations.setState(Animations::Idle);
    hairtype = Bowl;
    speed = 1.0f;
    left = false;
}