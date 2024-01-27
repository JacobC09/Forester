#pragma once
#include "common.h"
#include "tmx.h"

struct Cam {
    float x, y, zoom;

    Cam() = default;
    Cam(float x, float y, float zoom=1.0f) : x(x), y(y), zoom(zoom) {};

    void track(Vector2 pos, float delayFactor=1.0f);
    void maxView(float minX, float minY, float maxWidth, float maxHeight);

    float viewWidth() const;
    float viewHeight() const;

    Rectangle getScreenRect() const;
    Rectangle toScreen(Rectangle rect) const;
    Vector2 toScreen(Vector2 pos) const;
    Rectangle toRel(Rectangle rect) const;
    Vector2 toRel(Vector2 pos) const;
};

std::vector<Rectangle> GetCollidingTiles(Rectangle rect, Tilemap* map);
Rectangle GetVisableMapArea(Tilemap* map, const Cam& cam);
void DrawLayer(Tilemap* map, int layerId, const Cam& cam, int timer=0, bool drawCollisions=false);