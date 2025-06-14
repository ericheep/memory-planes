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
    
    int numAnchors = 70;
    
    tear = Tear(numAnchors, width, height);
    lifetime = 0;
    totalLifetime = 1.0;
    lastFrameTime = 0.016;
    
    birthTimer = 0.0;
    birthTime = 1.0;
    
    isAlive = true;
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

void Memory::fragment() {
    if (radius > 0.01) {
        float scaledRadius = radius * height / 3.0;;
        fragments.addFragment(scaledRadius, theta, arcDistance, instability);
    }
}

void Memory::setInstability(float _instability) {
    instability = _instability;
}

void Memory::update() {
    float birthScalar = ofMap(birthTimer, 0.0, birthTime, 0.0, 1.0, true);
    
    float lifetimeScalar = 1.0 - lifetime / totalLifetime;
    lifetimeScalar = ofMap(lifetimeScalar, 0.0, 0.9, 0.0, 1.0, true);
    
    radius = ofLerp(radius, targetRadius, follow);
    theta = ofLerp(theta, targetTheta, follow);
    arcDistance = ofLerp(arcDistance, targetArcDistance, follow);
    thickness = ofLerp(thickness, targetThickness, follow);
    noiseSpeed = ofLerp(noiseSpeed, targetNoiseSpeed, follow);
    octaveMultiplier = ofLerp(octaveMultiplier, targetOctaveMultiplier, follow);
    noiseTime += (ofGetLastFrameTime() * noiseSpeed);
    
    radius *= lifetimeScalar * birthScalar;
    arcDistance *= lifetimeScalar * birthScalar;
    thickness *= lifetimeScalar * birthScalar;
        
    updateTear(tear, 1.0);
    fragments.update();
    
    lifetime += lastFrameTime;
    birthTimer += lastFrameTime;
    fragments.setLastFrameTime(lastFrameTime);
    
    if (lifetime > totalLifetime) {
        isAlive = false;
    }
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
    fragments.draw();
}
