//
//  Anchor.cpp
//  memoryPlanes
//

#include "Anchor.hpp"

Anchor::Anchor(int _numAnchors) {
    theta = 0;
    radius = 0;
    direction = 1;
    thickness = 10;
    noiseTime = 0;
    arcDistance = PI;
    numAnchors = _numAnchors;
    velocity = 0;
    octaveMultiplier = 1;
    
    // point
    originPoint.set(1.0, 0.0);
    originPoint.rotateRad(theta);
    originPoint *= radius;
    
    // bezier direction
    originHeading.set(1.0, 0.0);
    originHeading.rotateRad(theta + HALF_PI);
    originHeading *= velocity;
    
    currentPoint = originPoint;
    currentHeading = originHeading;
}

void Anchor::update() {    
    float noiseTheta = theta;
    if (direction == 1) {
        noiseTheta += TWO_PI;
    } else {
        noiseTheta += TWO_PI * 2;
    }
    
    float noiseOffset = ofNoise(noiseTheta * octaveMultiplier, noiseTime);
    nO = ofMap(pow(noiseOffset, 0.5), 0.0, 1.0, 0.0, thickness / 2.0);
    
    float offset = deviation * nO * direction;
    
    currentPoint = originPoint;
    currentPoint *= radius + offset;
    
    float noiseHeading = ofNoise(noiseTheta * octaveMultiplier, noiseTime);
    velocity = (radius + deviation * nO) / numAnchors * arcDistance / PI;
    nH = ofMap(noiseHeading, 0.0, 1.0, -PI, PI);
    
    currentHeading = originHeading;
    currentHeading.rotate(nH);
    currentHeading *= velocity * direction;
     
}

void Anchor::setNoiseTime(float _noiseTime) {
    noiseTime = _noiseTime;
}

void Anchor::setRadius(float _radius) {
    radius = _radius;
}

void Anchor::setTheta(float _theta) {
    theta = _theta;
    
    // point
    originPoint.set(1.0, 0.0);
    originPoint.rotateRad(theta);
    
    // bezier direction
    originHeading.set(1.0, 0.0);
    originHeading.rotateRad(theta + HALF_PI);
}

void Anchor::setThickness(float _thickness) {
    thickness = _thickness;
}

void Anchor::setDeviation(float _deviation) {
    deviation = _deviation;
}

void Anchor::setDirection(int _direction) {
    direction = _direction;
}

void Anchor::setOctaveMultiplier(float _octaveMultiplier)  {
    octaveMultiplier = _octaveMultiplier;
}

ofVec2f Anchor::getPoint() {
    return currentPoint;
}

ofVec2f Anchor::getHeading() {
    return currentHeading;
}
