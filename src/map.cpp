#include "map.h"
#include "debug.h"

void Cam::track(Vector2 pos, float delayFactor) {
	if (delayFactor < 1)
		return;

	x += (pos.x - viewWidth() / 2 - x) / delayFactor;
	y += (pos.y - viewHeight() / 2 - y) / delayFactor;
}

void Cam::maxView(float minX, float minY, float maxWidth, float maxHeight) {
    x = std::max(minX, std::min(x, maxWidth - viewWidth()));
    y = std::max(minY, std::min(y, maxHeight - viewHeight()));
}

float Cam::viewWidth() const {
	return (float) GetScreenWidth() / zoom;
}

float Cam::viewHeight() const {
	return (float) GetScreenHeight() / zoom;
}

Rectangle Cam::getScreenRect() const {
	return {x, y, viewWidth(), viewHeight()};
}

Rectangle Cam::toScreen(Rectangle rect) const {
	return {std::floor((rect.x - x) * zoom), std::floor((rect.y - y) * zoom), std::floor(rect.width * zoom), std::floor(rect.height * zoom)};
}

Vector2 Cam::toScreen(Vector2 pos) const {
	return {std::floor((pos.x - x) * zoom), std::floor((pos.y - y) * zoom)};
}

Rectangle Cam::toRel(Rectangle rect) const {
	return {std::floor(rect.x / zoom + x), std::floor(rect.y / zoom + y), std::floor(rect.width / zoom), std::floor(rect.height / zoom)};
}
Vector2 Cam::toRel(Vector2 pos) const {
	return {std::floor(pos.x / zoom + x), std::floor(pos.y / zoom + y)};
}

std::vector<Rectangle> GetCollidingTiles(Rectangle rect, Tilemap* map) {
    std::vector<Rectangle> rects;

    int startX = std::floor(rect.x / map->tileWidth);
    int startY = std::floor(rect.y / map->tileHeight);
    int endX = startX + std::ceil(rect.width / map->tileWidth) + 1;
    int endY = startY + std::ceil(rect.height / map->tileHeight) + 1;

    for (auto &[id, layer] : map->layers) {
        for (int y = startY; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                Tile tile = layer.GetAt(x, y);

                if (tile.gid == 0) continue;

                auto [tilesetId, tileset] = map->GetTileset(tile.gid);
                auto otherRects = tileset->GetCollisionShapes(tilesetId);
                for (auto &otherRect : otherRects) {
                    Rectangle worldRect = Rectangle {
                        x * map->tileWidth + otherRect.x,
                        y * map->tileHeight + otherRect.y,
                        otherRect.width,
                        otherRect.height
                    };

                    if (CheckCollisionRecs(rect, worldRect))
                        rects.push_back(worldRect);
                }
            }
        }
    }

    return rects;
}

Rectangle GetVisableMapArea(Tilemap* map, const Cam& cam) {
    int startx = std::max(std::min((int) (cam.x / map->tileWidth), map->width), 0);
    int starty = std::max(std::min((int) (cam.y / map->tileHeight), map->height), 0);

    int endx = std::max(std::min((int) (cam.viewWidth() + cam.x) / map->tileWidth + 1, map->width), 0);
    int endy = std::max(std::min((int) (cam.viewHeight() + cam.y) / map->tileHeight + 1, map->height), 0);

    return {(float) startx, (float) starty, (float) endx, (float) endy};
}

void DrawLayer(Tilemap* map, int layerId, const Cam &cam, int timer, bool drawCollisions) {
    Layer* layer = map->GetLayer(layerId);
    Rectangle area = GetVisableMapArea(map, cam);

    for (int x = area.x; x < area.width; x++) {
		for (int y = area.y; y < area.height; y++) {
			Tile tile = layer->GetAt(x, y);

			if (tile.gid == 0) continue;

            auto [tilesetId, tileset] = map->GetTileset(tile.gid);
			TileAnimation* animation = tileset->GetAnimation(tilesetId);

			if (animation != nullptr) {
				tilesetId = animation->GetCurrent(timer);
			}

			Rectangle source = tileset->GetSourceRect(tilesetId);
    		Rectangle dest = cam.toScreen(Rectangle {(float) x * map->tileWidth, (float) y * map->tileHeight, (float) map->tileWidth, (float) map->tileHeight}); 
    		DrawTexturePro(tileset->texture, source, dest, Vector2{0, 0}, 0, WHITE);
            
            if (drawCollisions) {
                for (const Rectangle& rect : tileset->GetCollisionShapes(map->GetLocalTilesetId(tile.gid))) {
                    Rectangle collision = {
                        dest.x + rect.x * cam.zoom,
                        dest.y + rect.y * cam.zoom, 
                        rect.width * cam.zoom,
                        rect.height * cam.zoom
                    };

                    DrawRectangleRec(collision, {255, 0, 0, 150});
                }
            }
		}
	}
}