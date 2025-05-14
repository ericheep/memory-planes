//
//  Memory.cpp
//  memoryPlanes
//

#include "Memory.hpp"

Memory::Memory() {}

Memory::Memory(int _width, int _height) {
    radius = 0.0;
    theta = 0.0;
    arcDistance = 0.0;
    thickness = 0.0;
    octaveMultiplier = 0.0;
    noiseTime = 0.0;
    noiseSpeed = 1.0;
    
    targetRadius = 0.0;
    targetTheta = 0.0;
    targetArcDistance = 0.0;
    targetThickness = 0.0;
    targetOctaveMultiplier = 0.0;

    width = _width;
    height = _height;
    windowRadius = height / 2.0;
    fillState = true;
    
    leftVisibility = 0.0;
    rightVisibility = 0.0;
    
    int numAnchors = 200;
    
    centerTear = Tear(numAnchors, width, height);
    leftTear = Tear(numAnchors, width, height);
    rightTear = Tear(numAnchors, width, height);
}

void Memory::setColor(ofColor _primaryColor) {
    primaryColor = _primaryColor;
}

void Memory::setRadius(float _radius) {
    targetRadius = _radius;
}

void Memory::setTheta(float _theta) {
    targetTheta = _theta;
}

void Memory::setArcDistance(float _arcDistance) {
    targetArcDistance = _arcDistance;
}

void Memory::setThickness(float _thickness) {
    targetThickness = _thickness;
}

void Memory::setNoiseSpeed(float _noiseSpeed) {
    targetNoiseSpeed = _noiseSpeed;
}

void Memory::setOctaveMultiplier(float _octaveMultiplier) {
    targetOctaveMultiplier = _octaveMultiplier;
}

void Memory::setFill(bool _fillState) {
    fillState = _fillState;
}

void Memory::setLeftVisibility(float _leftVisibility) {
    targetLeftVisibility = _leftVisibility;
}

void Memory::setRightVisibility(float _rightVisibility) {
    targetRightVisibility = _rightVisibility;
}

void Memory::update() {
    radius = ofLerp(radius, targetRadius, 0.05);
    theta = ofLerp(theta, targetTheta, 0.05);
    arcDistance = ofLerp(arcDistance, targetArcDistance, 0.05);
    thickness = ofLerp(thickness, targetThickness, 0.05);
    noiseSpeed = ofLerp(noiseSpeed, targetNoiseSpeed, 0.05);
    octaveMultiplier = ofLerp(octaveMultiplier, targetOctaveMultiplier, 0.05);
    leftVisibility = ofLerp(leftVisibility, targetLeftVisibility, 0.05);
    rightVisibility = ofLerp(rightVisibility, targetRightVisibility, 0.05);
    noiseTime += (ofGetLastFrameTime() * noiseSpeed);
        
    updateTear(centerTear, 1.0);
    updateTear(leftTear, leftVisibility);
    updateTear(rightTear, rightVisibility);
}

void Memory::updateTear(Tear &tear, float visibility) {
    tear.setRadius(radius);
    tear.setTheta(theta);
    tear.setArcDistance(arcDistance * visibility);
    tear.setThickness(thickness * visibility);
    tear.setFill(fillState);
    tear.setNoiseTime(noiseTime);
    tear.setOctaveMultiplier(octaveMultiplier);
    tear.update();
}

void Memory::drawCenter() {
    centerTear.draw();
}
