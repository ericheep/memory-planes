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
    isFilled = true;
    
    int numAnchors = 100;
    
    tear = Tear(numAnchors, width, height);
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

void Memory::setFill(bool _isFilled) {
    isFilled = _isFilled;
}

void Memory::setFollow(float _minFollow, float _maxFollow) {
    tear.setFollow(_minFollow, _maxFollow);
    follow = (_minFollow + _maxFollow) / 2.0;    
}

void Memory::flip(float _theta) {
    theta = _theta;
    targetTheta = _theta;
    
    tear.flip();
}

void Memory::update() {
    radius = ofLerp(radius, targetRadius, follow);
    theta = ofLerp(theta, targetTheta, follow);
    arcDistance = ofLerp(arcDistance, targetArcDistance, follow);
    thickness = ofLerp(thickness, targetThickness, follow);
    noiseSpeed = ofLerp(noiseSpeed, targetNoiseSpeed, follow);
    octaveMultiplier = ofLerp(octaveMultiplier, targetOctaveMultiplier, follow);
    noiseTime += (ofGetLastFrameTime() * noiseSpeed);
        
    updateTear(tear, 1.0);
}

void Memory::updateTear(Tear &tear, float visibility) {
    tear.setRadius(radius);
    tear.setTheta(theta);
    tear.setArcDistance(arcDistance * visibility);
    tear.setThickness(thickness * visibility);
    tear.setFill(isFilled);
    tear.setNoiseTime(noiseTime);
    tear.setOctaveMultiplier(octaveMultiplier);
    tear.update();
}

void Memory::draw() {
    tear.draw();
}
