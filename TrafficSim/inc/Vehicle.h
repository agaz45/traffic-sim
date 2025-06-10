#pragma once

#include "Road.h"  // for TileType
#include "Intersection.h" // for Direction

class SimulationManager;
class RoadGrid; // forward declaration

class Vehicle {
private:
    static int nextId;
    int id;
    float speed;      // tiles per second
    int x, y;
    int destX, destY;
    Direction direction;
    float movementAccumulator;

public:
    Vehicle(int x, int y, int destX, int destY);

    void update(float deltaTime, const RoadGrid& grid, const SimulationManager& simMgr);
    void render() const;

    int getX() const;
    int getY() const;
    Direction getDirection() const;
    int getDestX() const;
    int getDestY() const;
    int getId() const;

    void maybeTurn(const Intersection& intersection);
    void setNewDestination(int newDestX, int newDestY);
    bool hasReachedDestination() const;
};
