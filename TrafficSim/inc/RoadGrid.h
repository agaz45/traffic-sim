#pragma once

#include "Direction.h"

#include <vector>
#include <string>

enum class TileType {
    Empty,
    RoadHorizontal,
    RoadVertical,
    Intersection
};

char tileToChar(TileType type);

class RoadGrid {
private:
    int width, height;
    std::vector<std::vector<TileType>> grid;

public:
    RoadGrid(int width, int height);
    void setTile(int x, int y, TileType type);
    TileType getTile(int x, int y) const;
    void render() const;

    int getWidth() const;
    int getHeight() const;

    bool canMove(int x, int y, Direction dir) const;

};
