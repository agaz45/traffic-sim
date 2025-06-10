#include "Vehicle.h"

#include "SimulationManager.h"
#include "Direction.h"
#include "Road.h"
#include <iostream>
#include <random>

int Vehicle::nextId = 1;

Direction calculateInitialDirection(int startX, int startY, int destX, int destY) {
    if (startX < destX) return Direction::East;
    if (startX > destX) return Direction::West;
    if (startY < destY) return Direction::South;
    if (startY > destY) return Direction::North;

    // Start and destination are the same — fallback direction
    return Direction::North;
}

// With destination
Vehicle::Vehicle(int startX, int startY, int destX, int destY) :
    id(nextId++),
    speed(2.0f),
    x(startX),
    y(startY),
    direction(calculateInitialDirection(startX, startY, destX, destY)),
    destX(destX),
    destY(destY),
    movementAccumulator(0.0f) {}

void Vehicle::update(float deltaTime, const RoadGrid& grid, const SimulationManager& simMgr) {
    movementAccumulator += speed * deltaTime;

    // If we reached destination, assign a new one
    if (x == destX && y == destY) {
        auto [newDestX, newDestY] = simMgr.getRandomRoadPosition();
        setNewDestination(newDestX, newDestY);
    }

    while (movementAccumulator >= 1.0f) {
        // At intersection: decide direction (straight, left, right) toward destination
        if (grid.getTile(x, y) == TileType::Intersection) {
            std::vector<Direction> candidates = {
                direction,
                turnLeft(direction),
                turnRight(direction)
            };

            Direction bestDir = direction;
            float bestDist = std::numeric_limits<float>::max();

            for (Direction d : candidates) {
                int nx = x, ny = y;
                switch (d) {
                    case Direction::North: ny -= 1; break;
                    case Direction::South: ny += 1; break;
                    case Direction::East:  nx += 1; break;
                    case Direction::West:  nx -= 1; break;
                }

                if (grid.canMove(x, y, d)) {
                    float dist = std::hypot(destX - nx, destY - ny);
                    if (dist < bestDist) {
                        bestDist = dist;
                        bestDir = d;
                    }
                }
            }

            direction = bestDir;
        }
        else {
            // Not intersection, must continue straight if possible
            if (!grid.canMove(x, y, direction)) {
                // Blocked — cannot turn around, stop moving this update
                break;
            }
        }

        // Calculate next tile based on chosen direction
        int nextX = x;
        int nextY = y;
        switch (direction) {
            case Direction::North: nextY -= 1; break;
            case Direction::South: nextY += 1; break;
            case Direction::East:  nextX += 1; break;
            case Direction::West:  nextX -= 1; break;
        }

        // Check if next tile is road or intersection
        TileType nextTile = grid.getTile(nextX, nextY);
        if (nextTile == TileType::Empty) {
            // Can't move into empty tile
            break;
        }

        // Check traffic lights on intersection tiles
        if (nextTile == TileType::Intersection) {
            const Intersection* intersectionPtr = nullptr;
            for (const auto& intersection : simMgr.getIntersections()) {
                if (intersection.getX() == nextX && intersection.getY() == nextY) {
                    intersectionPtr = &intersection;
                    break;
                }
            }
            if (intersectionPtr && intersectionPtr->hasLight() && intersectionPtr->getLightState() == LightState::Red) {
                // Red light, stop here
                break;
            }
        }

        // Check collisions with other vehicles
        if (simMgr.isTileOccupied(nextX, nextY, this)) {
            // Blocked by vehicle
            break;
        }

        // Move vehicle forward one tile
        x = nextX;
        y = nextY;
        movementAccumulator -= 1.0f;

        // If reached destination after moving, pick a new one and stop moving further this update
        if (x == destX && y == destY) {
            auto [newDestX, newDestY] = simMgr.getRandomRoadPosition();
            setNewDestination(newDestX, newDestY);
            break;
        }
    }
}


void Vehicle::render() const {
    std::cout << "Vehicle " << id
        << " | Pos: (" << x << ", " << y << ")"
        << " | Dir: ";

    switch (direction) {
    case Direction::North: std::cout << "N"; break;
    case Direction::South: std::cout << "S"; break;
    case Direction::East:  std::cout << "E"; break;
    case Direction::West:  std::cout << "W"; break;
    }

    std::cout << "\n";
}

int Vehicle::getX() const { return x; }
int Vehicle::getY() const { return y; }
Direction Vehicle::getDirection() const { return direction; }
int Vehicle::getDestX() const { return destX; }
int Vehicle::getDestY() const { return destY; }
int Vehicle::getId() const { return id; }

void Vehicle::maybeTurn(const Intersection& intersection) {
    // Only turn if we're on the same position
    if (x != intersection.getX() || y != intersection.getY()) return;

    // Random choice: 0 = left, 1 = straight, 2 = right
    static std::default_random_engine rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 2);
    int choice = dist(rng);

    Direction newDir = direction;
    if (choice == 0) newDir = turnLeft(direction);
    else if (choice == 2) newDir = turnRight(direction);

    // Only apply turn if intersection allows that direction
    if (intersection.isConnected(newDir)) {
        direction = newDir;
    }
}

void Vehicle::setNewDestination(int newDestX, int newDestY) {
    destX = newDestX;
    destY = newDestY;
}

bool Vehicle::hasReachedDestination() const {
    return x == destX && y == destY;
}
