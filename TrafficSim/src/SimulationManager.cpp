#include "SimulationManager.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

SimulationManager::SimulationManager(int w, int h, int numVeh)
    : roadGrid(w, h), width(w), height(h), numVehicles(numVeh), running(true), totalSimTime(0.0f) {}

void SimulationManager::initialize() {
    // Create horizontal roads on specific rows (parameterize these too if desired)

    for (int x = 0; x < width; ++x) {
        for (int x = 1; x < width - 1; ++x) {
            roadGrid.setTile(x, 0, TileType::RoadHorizontal);
            roadGrid.setTile(x, height - 1, TileType::RoadHorizontal);
        }
    }

    for (int y = 0; y < height; ++y) {
        for (int y = 2; y < height - 1; ++y) {
            roadGrid.setTile(0, y, TileType::RoadVertical);
            roadGrid.setTile(width - 1, y, TileType::RoadVertical);
        }
    }

    std::vector<int> horizontalRoadRows = { height / 4, height / 2, 3 * height / 4 };
    for (int row : horizontalRoadRows) {
        for (int x = 1; x < width - 1; ++x) {
            roadGrid.setTile(x, row, TileType::RoadHorizontal);
        }
    }

    std::vector<int> verticalRoadCols = { width / 4, width / 2, 3 * width / 4 };
    for (int col : verticalRoadCols) {
        for (int y = 1; y < height - 1; ++y) {
            roadGrid.setTile(col, y, TileType::RoadVertical);
        }
    }

    for (int row : horizontalRoadRows) {
        for (int col : verticalRoadCols) {
            roadGrid.setTile(col, row, TileType::Intersection);
            addIntersection(col, row, true);
        }
    }

    for (int i = 0; i < numVehicles; ++i) {
        int startRow = horizontalRoadRows[i % horizontalRoadRows.size()];
        int startX = 1;
        int destX = width - 2;
        int destY = verticalRoadCols[i % verticalRoadCols.size()];

        vehicles.emplace_back(startX, startRow, destX, destY);
    }
}

void SimulationManager::update(float deltaTime) {
    totalSimTime += deltaTime;

    for (auto& vehicle : vehicles) {
        vehicle.update(deltaTime, roadGrid, *this);
    }

    // Exit after 10 seconds for now
    if (totalSimTime >= 10.0f)
        running = false;
}


void SimulationManager::render() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // 1. Build a 2D char grid from the road
    int w = roadGrid.getWidth();
    int h = roadGrid.getHeight();
    std::vector<std::vector<char>> display(h, std::vector<char>(w, ' '));

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            display[y][x] = tileToChar(roadGrid.getTile(x, y));
        }
    }

    // 2. Overlay vehicles
    for (const auto& v : vehicles) {
        char symbol = 'V';
        switch (v.getDirection()) {
        case Direction::North: symbol = '^'; break;
        case Direction::South: symbol = 'v'; break;
        case Direction::East:  symbol = '>'; break;
        case Direction::West:  symbol = '<'; break;
        }

        int vx = v.getX();
        int vy = v.getY();
        if (vx >= 0 && vx < w && vy >= 0 && vy < h) {
            display[vy][vx] = symbol;
        }
    }

    // 3. Print the grid
    for (const auto& row : display) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << '\n';
    }

    // 4. Optional stats
    std::cout << "Simulation time: " << totalSimTime << "s\n";

    for (const auto& car: vehicles) {
        std::cout << "Car " << car.getId() << " Position(" << car.getY() << ", " << car.getX() << ") Dest (" << car.getDestY() << ", " << car.getDestX() << ")\n";
    }
}

bool SimulationManager::isRunning() const {
    return running;
}

bool SimulationManager::isTileOccupied(int x, int y, const Vehicle* self) const {
    for (const auto& v : vehicles) {
        if (&v != self && v.getX() == x && v.getY() == y) {
            return true;
        }
    }
    return false;
}

std::pair<int, int> SimulationManager::getRandomRoadPosition() const {
    std::vector<std::pair<int,int>> roadPositions;
    for (int x = 0; x < roadGrid.getWidth(); ++x) {
        for (int y = 0; y < roadGrid.getHeight(); ++y) {
            TileType t = roadGrid.getTile(x, y);
            if (t == TileType::RoadHorizontal || t == TileType::RoadVertical || t == TileType::Intersection) {
                roadPositions.emplace_back(x, y);
            }
        }
    }
    if (roadPositions.empty()) throw std::runtime_error("No roads available!");
    int idx = rand() % roadPositions.size();
    return roadPositions[idx];
}

void SimulationManager::addIntersection(int x, int y, bool withLight) {
    Intersection i(x, y, withLight);
    i.addConnection(Direction::North);
    i.addConnection(Direction::South);
    i.addConnection(Direction::East);
    i.addConnection(Direction::West);
    intersections.push_back(i);
}