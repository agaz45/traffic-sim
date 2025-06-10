#pragma once

enum class LightState {
    Red,
    Green
};

class TrafficLight {
private:
    LightState state;
    float timer;         // seconds passed
    float greenDuration; // how long green lasts
    float redDuration;   // how long red lasts

public:
    TrafficLight(float greenDuration = 5.0f, float redDuration = 5.0f);

    void update(float deltaTime);
    LightState getState() const;
};
