#include "Intersection.h"

Intersection::Intersection(int x, int y) : x(x), y(y) {}

void Intersection::addConnection(Direction dir) {
    connections.insert(dir);
}

bool Intersection::isConnected(Direction dir) const {
    return connections.count(dir) > 0;
}

int Intersection::getX() const {
    return x;
}

int Intersection::getY() const {
    return y;
}
