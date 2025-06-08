//
//  Fragment.cpp
//  memory-planes-ofx
//

#include "Fragment.hpp"

Fragment::Fragment() {
}

Fragment::Fragment(float _radius, float _theta, float _arcLength, float _instability) {
    isAlive = true;
    lifetime = 0.0;
    instability = _instability;
    
    size = 2;
    totalLifetime = 1.0;
    totalLifetime = ofMap(instability, 0.0, 1.0, 1.0, 5.0);
    lastFrameTime = 0.0;
    speed = 1.0;
    velocity = ofRandom(0.0, 1.0);
    
    radius = _radius;
    travelRadius = _radius;
    
    theta = _theta;
    arcLength = _arcLength;
    direction = ofRandom(1.0) < 0.5 ? -1 : 1;
    weird = ofRandom(1.0) < 0.5 ? -1 : 1;

    travelTheta = theta + (arcLength / 4.0 * direction);
}

void Fragment::setLastFrameTime(float _lastFrameTime) {
    lastFrameTime = _lastFrameTime;
}

void Fragment::update() {
    float lifetimeScalar = 1.0 - lifetime / totalLifetime;
    
    float thetaInc = lastFrameTime * speed;

    travelTheta += thetaInc * direction * velocity;
    travelRadius += instability * weird * 0.05;
    
    position.x = cos(travelTheta) * travelRadius;
    position.y = sin(travelTheta) * travelRadius;
    
    lifetime += lastFrameTime;
    travelSize = lifetimeScalar * size;
    
    if (lifetime > totalLifetime) {
        isAlive = false;
    }
}

void Fragment::draw() {
    ofDrawCircle(position.x, position.y, travelSize);
}
