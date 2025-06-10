#pragma once

#include "Road.h"
#include "Vehicle.h"
#include "Intersection.h"

#include <vector>

class SimulationManager {
private:
    RoadGrid roadGrid;
    int width;
    int height;
    int numVehicles;

    std::vector<Vehicle> vehicles;
    std::vector<Intersection> intersections;

    bool running;
    float totalSimTime;

public:
    SimulationManager(int w, int h, int numVeh);
    void initialize();
    void update(float deltaTime);
    void render() const;
    bool isRunning() const;

    const std::vector<Vehicle>& getVehicles() const { return vehicles; }
    const std::vector<Intersection>& getIntersections() const { return intersections; }
    const RoadGrid& getRoadGrid() const { return roadGrid; }

    void addIntersection(int x, int y, bool withLight);
    std::pair<int, int> getRandomRoadPosition() const;
    bool isTileOccupied(int x, int y, const Vehicle* self) const;
};
