#include "Road.h"

#include "Direction.h"
#include <iostream>

RoadGrid::RoadGrid(int width, int height)
    : width(width), height(height), grid(height, std::vector<TileType>(width, TileType::Empty)) {}

void RoadGrid::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = type;
    }
}

TileType RoadGrid::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return TileType::Empty;
}

void RoadGrid::render() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch (grid[y][x]) {
            case TileType::Empty:         std::cout << ' '; break;
            case TileType::RoadHorizontal:std::cout << '-'; break;
            case TileType::RoadVertical:  std::cout << '|'; break;
            case TileType::Intersection:  std::cout << '+'; break;
            }
        }
        std::cout << '\n';
    }
}

int RoadGrid::getWidth() const {
    return width;
}

int RoadGrid::getHeight() const {
    return height;
}

char tileToChar(TileType type) {
    switch (type) {
    case TileType::Empty: return ' ';
    case TileType::RoadHorizontal: return '-';
    case TileType::RoadVertical:   return '|';
    case TileType::Intersection:   return '+';
    }
    return '?';
}

bool RoadGrid::canMove(int x, int y, Direction dir) const {
    int nextX = x;
    int nextY = y;

    switch (dir) {
    case Direction::North: nextY -= 1; break;
    case Direction::South: nextY += 1; break;
    case Direction::East:  nextX += 1; break;
    case Direction::West:  nextX -= 1; break;
    }

    if (nextX < 0 || nextY < 0 || nextX >= width || nextY >= height) {
        return false; // Out of bounds
    }

    TileType nextTile = getTile(nextX, nextY);
    return nextTile == TileType::RoadHorizontal ||
           nextTile == TileType::RoadVertical ||
           nextTile == TileType::Intersection;
}