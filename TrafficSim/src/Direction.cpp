#include "Direction.h"

Direction turnLeft(Direction dir) {
    switch (dir) {
    case Direction::North: return Direction::West;
    case Direction::West:  return Direction::South;
    case Direction::South: return Direction::East;
    case Direction::East:  return Direction::North;
    }
    return dir; // default fallback
}

Direction turnRight(Direction dir) {
    switch (dir) {
    case Direction::North: return Direction::East;
    case Direction::East:  return Direction::South;
    case Direction::South: return Direction::West;
    case Direction::West:  return Direction::North;
    }
    return dir; // default fallback
}