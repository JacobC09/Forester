#include "animation.h"

void Animation::update() {
    if (timer >= totalFrames * frameDur) {
        if (repeating)
            reset();
        return;
    }
    timer++;
}

void Animation::reset() {
    timer = 0;
}

bool Animation::isFinished() {
    return timer >= totalFrames * frameDur;
}

Rectangle Animation::getFrame() {
    return Rectangle {startFrame.x + startFrame.width * (timer / frameDur), startFrame.y, startFrame.width, startFrame.height};
}

void AnimationPlayer::setState(int newState) {
    if (state != -1 && newState != state)
        animations[state].reset();
    state = newState;
}

Animation* AnimationPlayer::getAnimation() {
    if (state == -1)
        return nullptr;
    return &animations[state];
}