#include "TrafficLight.h"

TrafficLight::TrafficLight(float greenDuration, float redDuration)
    : state(LightState::Green), timer(0.0f), greenDuration(greenDuration), redDuration(redDuration)
{}

void TrafficLight::update(float deltaTime) {
    timer += deltaTime;

    if (state == LightState::Green && timer >= greenDuration) {
        state = LightState::Red;
        timer = 0.0f;
    }
    else if (state == LightState::Red && timer >= redDuration) {
        state = LightState::Green;
        timer = 0.0f;
    }
}

LightState TrafficLight::getState() const {
    return state;
}
