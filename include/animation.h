#pragma once
#include <map>
#include "raylib.h"
#include "assets.h"

struct Animation {
    Animation() = default;
    Animation(Textures texture, Rectangle startFrame, int frameDur, int totalFrames, bool repeating=true)
        : texture(texture), startFrame(startFrame), frameDur(frameDur), 
          totalFrames(totalFrames), timer(0), repeating(repeating) {};

    int timer;
    int frameDur;
    int totalFrames;
    bool repeating;
    Rectangle startFrame;
    Textures texture;

    void update();
    void reset();
    bool isFinished();
    Rectangle getFrame();
};

struct AnimationPlayer {
    AnimationPlayer() = default;
    AnimationPlayer(std::map<int, Animation> animations, int initialState=-1)
        : animations(animations), state(initialState) {};

    int state;
    std::map<int, Animation> animations;

    void setState(int newState);
    Animation* getAnimation();
};