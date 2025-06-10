#pragma once

#include "Direction.h"
#include "TrafficLight.h"

#include <set>

class Intersection {
public:
    Intersection(int x, int y);

    void addConnection(Direction dir);
    bool isConnected(Direction dir) const;

    int getX() const;
    int getY() const;

    Intersection(int x, int y, bool withLight = false)
        : x(x), y(y), hasTrafficLight(withLight), trafficLight()
    {}

    void update(float deltaTime) {
        if (hasTrafficLight) trafficLight.update(deltaTime);
    }

    LightState getLightState() const {
        if (hasTrafficLight)
            return trafficLight.getState();
        else
            return LightState::Green; // No light means "go"
    }

    bool hasLight() const { return hasTrafficLight; }

private:
    int x, y;
    std::set<Direction> connections;
    TrafficLight trafficLight;
    bool hasTrafficLight;
};
