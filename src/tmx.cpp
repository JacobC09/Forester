#include <sstream>
#include "common.h"
#include "tmx.h"
#include "debug.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void PrintTmxError(const char* msg) {
    std::cout << "TMX Error: " << msg << std::endl;
}

void Project::LoadTilemap(int id, const char* path) {
    std::filesystem::path currentPath = path;
    std::unique_ptr<Tilemap> map = std::make_unique<Tilemap>();

    XMLDocument doc;
    if (doc.LoadFile(path) != XML_SUCCESS) {
        PrintTmxError("Error loading the file");
        return;
    }

    XMLElement* mapEl = doc.FirstChildElement("map");
    if (mapEl == NULL) {
        PrintTmxError("No Map Node");
        return;
    }

    map->width = mapEl->IntAttribute("width");
    map->height = mapEl->IntAttribute("height");
    map->tileWidth = mapEl->IntAttribute("tilewidth");
    map->tileHeight = mapEl->IntAttribute("tileheight");

    for (XMLElement* setEl = mapEl->FirstChildElement("tileset"); setEl; setEl = setEl->NextSiblingElement("tileset")) {
        std::filesystem::path tilesetPath = currentPath.parent_path() / setEl->Attribute("source");

        bool found = false;
        for (auto &[id, tileset] : tilesets) {
            if (tilesetPath.compare(tileset->path) != 0) {
                found = true;
                map->tilesets[setEl->IntAttribute("firstgid")] = tileset.get();
                break;
            }
        }

        if (!found) {
            const char* filename = tilesetPath.filename().string().c_str();
            PrintTmxError(TextFormat("Tileset dependancy %s hasn't been loaded yet", filename));
        }
    }

    int layerId = 0;
    for (XMLElement* layerEl = mapEl->FirstChildElement("layer"); layerEl; layerEl = layerEl->NextSiblingElement("layer")) {
        Layer &layer = map->layers[layerId++];

        layer.width = layerEl->IntAttribute("width");
        layer.height = layerEl->IntAttribute("height");

        int gid = 0;
        std::stringstream csvss (layerEl->FirstChildElement("data")->FirstChild()->Value());
        while (csvss >> gid) {
            if (csvss.peek() == ',' || csvss.peek() == '\n')
                csvss.ignore();

            layer.tiles.push_back(Tile {gid});
        }
    }

    for (XMLElement* objGel = mapEl->FirstChildElement("objectgroup"); objGel; objGel = objGel->NextSiblingElement("objectgroup")) {
        for (XMLElement* objEl = objGel->FirstChildElement("object"); objEl; objEl = objEl->NextSiblingElement("object")) {
            map->objects.push_back(Object {
                objEl->FloatAttribute("x"),
                objEl->FloatAttribute("y"),
                objEl->Attribute("name"),
            });
        }
    }

    tilemaps[id] = std::move(map);
}

void Project::LoadTileset(int id, const char* path, Texture2D texture) {

    std::filesystem::path currentPath = path;
    std::unique_ptr<Tileset> set = std::make_unique<Tileset>();

    XMLDocument doc;
    if (doc.LoadFile(path) != XML_SUCCESS) {
        PrintTmxError("Error loading the file");
        return;
    }

    XMLElement* setElement = doc.FirstChildElement("tileset");
    if (setElement == NULL) {
        PrintTmxError("No Tileset Node");
        return;
    }

    set->texture = texture;
    set->totalTiles = setElement->IntAttribute("tilecount");
    set->tileWidth = setElement->IntAttribute("tilewidth");
    set->tileHeight = setElement->IntAttribute("tileheight");
    set->width = texture.width / set->tileWidth;
    set->height = texture.height / set->tileHeight;

    XMLElement *imageEl = setElement->FirstChildElement("image");
    if (imageEl->IntAttribute("width") != texture.width || imageEl->IntAttribute("height") != texture.height) {
        PrintTmxError("Provided texture does not match image in tileset");
        return;
    }

    for (XMLElement* tileEl = setElement->FirstChildElement("tile"); tileEl; tileEl = tileEl->NextSiblingElement("tile")) {
        int id = tileEl->IntAttribute("id");

        XMLElement* objectGroupEl = tileEl->FirstChildElement("objectgroup");
        if (objectGroupEl != NULL) {
            std::vector<Rectangle> &collisionRects = set->collisionShapes[id];
            for (XMLElement* objEl = objectGroupEl->FirstChildElement("object"); objEl; objEl = objEl->NextSiblingElement("object")) {
                collisionRects.push_back(Rectangle {
                    objEl->FloatAttribute("x"),
                    objEl->FloatAttribute("y"),
                    objEl->FloatAttribute("width"),
                    objEl->FloatAttribute("height")
                });
            }
        }

        XMLElement* animationEl = tileEl->FirstChildElement("animation");
        if (animationEl != NULL) {
            TileAnimation &animation = set->animations[id];
            for (XMLElement* frameEl = animationEl->FirstChildElement("frame"); frameEl; frameEl = frameEl->NextSiblingElement("frame")) {
                int duration = frameEl->IntAttribute("duration");
                animation.totalDuration += duration;
                animation.frames.push_back(TileAnimation::Frame {frameEl->IntAttribute("tileid"), duration});
            }

            if (!animation.frames.size()) {
                PrintTmxError(TextFormat("Animation for tile %s doesn't have any frames", std::to_string(id).c_str()));
            }
        }
    }

    tilesets[id] = std::move(set);
}

Tilemap* Project::GetTilemap(int id) {
    return tilemaps[id].get();
}

Tileset* Project::GetTileset(int id) {
    return tilesets[id].get();
}

int TileAnimation::GetCurrent(int elapsedMiliSeconds) {
    int elapsed = elapsedMiliSeconds % totalDuration;
    int total = 0;

    for (auto &frame : frames) {
        total += frame.duration;
        if (elapsed <= total)
            return frame.id;
    }

    return frames.front().id;
}

Tile Layer::GetAt(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        return tiles[y * width + x];
    return Tile { 0 };
}

void Layer::SetAt(int x, int y, Tile tile) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        tiles[y * width + x] = tile;
}

Rectangle Tileset::GetSourceRect(int id) {
    return Rectangle {
        (float) (id % width) * tileWidth,
        (float) (id / width) * tileHeight, 
        (float) tileWidth, 
        (float) tileHeight
    };
}

std::vector<Rectangle> &Tileset::GetCollisionShapes(int id) {
    return collisionShapes[id];
}

TileAnimation* Tileset::GetAnimation(int id) {
    auto pair = animations.find(id);
    if (pair == animations.end())
        return nullptr;
    return &pair->second;
}

Layer* Tilemap::GetLayer(int id) {
    return &layers[id];
}

Rectangle Tilemap::GetSourceRect(int gid) {
    for (auto [firstgid, tileset] : tilesets) {
        if (gid < firstgid + tileset->totalTiles)
            return tileset->GetSourceRect(gid - firstgid);
    }

    return Rectangle {};
}

std::pair<int, Tileset*> Tilemap::GetTileset(int gid) {
    for (auto &[firstgid, tileset] : tilesets) {
        if (gid < firstgid + tileset->totalTiles)
            return {gid - firstgid, tileset};
    }

    return {};
}

int Tilemap::GetLocalTilesetId(int gid) {
    for (auto &[firstgid, tileset] : tilesets) {
        if (gid < firstgid + tileset->totalTiles)
            return gid - firstgid;
    }

    return -1;
}