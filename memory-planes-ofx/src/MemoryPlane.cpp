//
//  MemoryPlane.cpp
//  memoryPlanes
//

#include "MemoryPlane.hpp"

MemoryPlane::MemoryPlane() {
}

MemoryPlane::MemoryPlane(int _width, int _sideHeight, int _mainHeight) {
    width = _width;
    sideHeight = _sideHeight;
    mainHeight = _mainHeight;
    calibrationMode = false;
    
    fresnelMute = false;
    leftMute = false;
    rightMute = false;
    
    // memories
    for (int i = 0; i < 6; i++) {
        Memory memory = Memory(width, sideHeight, mainHeight, PI * i);
        memory.setDistance(0.0);
        memory.setRadius(0.0);
        memory.setArcDistance(0);
        memory.setThickness(0);
        memory.setFill(true);
        memory.setLeftVisibility(0.0);
        memory.setRightVisibility(0.0);
        memory.setNoiseSpeed(1.0);
        memory.setOctaveMultiplier(1.0);
        
        memories.push_back(memory);
    }
    
    // calibration memories
    for (int i = 0; i < 2; i++) {
        Memory memory = Memory(width, sideHeight, mainHeight, PI * i);
        memory.setDistance(0.0);
        memory.setRadius(sideHeight / 2.0);
        memory.setArcDistance(PI / 2);
        memory.setThickness(50);
        memory.setFill(true);
        memory.setLeftVisibility(1.0);
        memory.setRightVisibility(1.0);
        memory.setNoiseSpeed(1.0);
        memory.setOctaveMultiplier(1.0);
        
        calibrationMemories.push_back(memory);
    }
}

void MemoryPlane::update() {
    for (int i = 0; i < memories.size(); i++) {
        memories[i].setNoiseTime(noiseTime);
        memories[i].setNoiseSpeed(noiseSpeed);
        memories[i].setOctaveMultiplier(octaveMultiplier);

        memories[i].update();
        if (memories[i].lifetime < 0) {
            memories.erase(memories.begin() + i);
        }
    }
    
    if (!calibrationMode) return;
    
    for (int i = 0; i < calibrationMemories.size(); i++) {
        calibrationMemories[i].setNoiseTime(noiseTime);
        calibrationMemories[i].setNoiseSpeed(noiseSpeed);
        calibrationMemories[i].setOctaveMultiplier(octaveMultiplier);

        calibrationMemories[i].update();
    }
}

void MemoryPlane::drawMainWindow() {
    if (fresnelMute) return;
    
    for (int i = 0; i < memories.size(); i++) {
        memories[i].drawFresnel();
    }
    
    if (!calibrationMode) return;
    
    for (int i = 0; i < calibrationMemories.size(); i++) {
        calibrationMemories[i].drawFresnel();
    }
}

void MemoryPlane::drawLeftWindow() {
    if (leftMute) return;
    
    for (int i = 0; i < memories.size(); i++) {
        memories[i].drawLeftSide();
    }
    
    if (!calibrationMode) return;
    
    for (int i = 0; i < calibrationMemories.size(); i++) {
        calibrationMemories[i].drawLeftSide();
    }
}

void MemoryPlane::drawRightWindow() {
    if (rightMute) return;
    
    for (int i = 0; i < memories.size(); i++) {
        memories[i].drawRightSide();
    }
    
    if (!calibrationMode) return;
    
    for (int i = 0; i < calibrationMemories.size(); i++) {
        calibrationMemories[i].drawRightSide();
    }
}

void MemoryPlane::setColor(ofColor _primaryColor) {
    primaryColor = _primaryColor;
}

void MemoryPlane::setCalibrationMode(bool _calibrationMode) {
    calibrationMode = _calibrationMode;
}

void MemoryPlane::setFresnelMute(bool _fresnelMute) {
    fresnelMute = _fresnelMute;
}

void MemoryPlane::setLeftMute(bool _leftMute) {
    leftMute = _leftMute;
}

void MemoryPlane::setRightMute(bool _rightMute) {
    rightMute = _rightMute;
}

void MemoryPlane::setNoiseTime(float _noiseTime) {
    noiseTime = _noiseTime;
}

void MemoryPlane::setNoiseSpeed(float _noiseSpeed) {
    noiseSpeed = _noiseSpeed;
}

void MemoryPlane::setOctaveMultiplier(float _octaveMultiplier) {
    octaveMultiplier = _octaveMultiplier;
}

void MemoryPlane::setRadius(float _radius) {
    radius = _radius;
}

void MemoryPlane::setMemory(int index, float distance, float radius, float theta, float arcDistance, float thickness) {
    
    bool found = false;
    int foundIndex = -1;
    
    for (int i = 0; i < memories.size(); i++) {
        if (memories[i].index == index) {
            found = true;
            foundIndex = i;
        } else {
            return;
        }
    }
    
    if (found) {
        memories[foundIndex].setDistance(distance);
        memories[foundIndex].setRadius(radius);
        memories[foundIndex].setTheta(theta);
        memories[foundIndex].setArcDistance(arcDistance);
        memories[foundIndex].setThickness(thickness);
        memories[foundIndex].setLeftVisibility(0.15);
        memories[foundIndex].setRightVisibility(0.5);
        memories[foundIndex].setOctaveMultiplier(octaveMultiplier);
        memories[foundIndex].keepAlive();
    } else {
        /*Memory m = Memory(width, height, theta);
        m.index = index;
        m.distance = distance;
        m.radius = radius;
        m.arcDistance = arcDistance;
        m.thickness = thickness;
        
        memories.push_back(m);*/
    }
}
