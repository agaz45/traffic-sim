#include "SimulationManager.h"
#include <chrono>
#include <thread>

int main() {
    SimulationManager sim(10, 10, 3);
    sim.initialize();

    const float deltaTime = 0.5f; // 0.5 sec per tick

    while (sim.isRunning()) {
        sim.update(deltaTime);
        sim.render();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
