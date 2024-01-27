#pragma once
#include <map>
#include <filesystem>
#include <memory>
#include "assets.h"
#include "common.h"
#include "raylib.h"

struct Tile {
    int gid = 0;
};

class TileAnimation {
public:
    struct Frame {
        int id, duration;
    };

    int totalDuration;
    std::vector<Frame> frames;

    int GetCurrent(int elapsedMiliSeconds);
};

struct Object {
    float x, y;
    std::string name;
};

class Tileset {
public:
    int width, height, tileWidth, tileHeight, totalTiles;
    Texture2D texture;
    std::filesystem::path path;
    std::map<int, std::vector<Rectangle>> collisionShapes;
    std::map<int, TileAnimation> animations;

    Rectangle GetSourceRect(int id);
    std::vector<Rectangle>& Tileset::GetCollisionShapes(int id);
    TileAnimation* GetAnimation(int id);
};

class Layer {
public:
    int index, width, height;
    std::vector<Tile> tiles;

    Tile GetAt(int x, int y);
    void SetAt(int x, int y, Tile tile);
};

class Tilemap {
public:
    int width, height, tileWidth, tileHeight;
    std::map<int, Layer> layers;
    std::map<int, Tileset*> tilesets;
    std::vector<Object> objects;

    std::pair<int, Tileset*> GetTileset(int gid);
    int GetLocalTilesetId(int gid);
    Rectangle GetSourceRect(int gid);
    Layer* GetLayer(int id);
};

class Project {
public:
    void LoadTilemap(int id, const char* path);
    void LoadTileset(int id, const char* path, Texture2D texture);

    Tilemap* GetTilemap(int id);
    Tileset* GetTileset(int id);

private:
    std::map<int, std::unique_ptr<Tilemap>> tilemaps;
    std::map<int, std::unique_ptr<Tileset>> tilesets;
};