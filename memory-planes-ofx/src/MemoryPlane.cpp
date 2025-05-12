//
//  MemoryPlane.cpp
//  memoryPlanes
//

#include "MemoryPlane.hpp"

MemoryPlane::MemoryPlane() {
}

MemoryPlane::MemoryPlane(int _width, int _height) {
    width = _width;
    height = _height;
    calibrationMode = false;
    
    centerMute = false;
    leftMute = false;
    rightMute = false;
    
    // memories
    for (int i = 0; i < 3; i++) {
        Memory memory = Memory(width, height);
        memory.setRadius(0.0);
        memory.setTheta(PI * i);
        memory.setArcDistance(0);
        memory.setThickness(0);
        memory.setFill(true);
        memory.setNoiseSpeed(1.0);
        memory.setOctaveMultiplier(1.0);
        memory.setLeftVisibility(1.0);
        memory.setRightVisibility(1.0);
        
        memories.push_back(memory);
    }
    
    // calibration memories
    for (int i = 0; i < 2; i++) {
        Memory memory = Memory(width, height);
        memory.setRadius(1.0);
        memory.setTheta(i * PI);
        memory.setArcDistance(PI);
        memory.setThickness(0.25);
        memory.setFill(true);
        memory.setNoiseSpeed(1.0);
        memory.setOctaveMultiplier(0.5);
        memory.setLeftVisibility(1.0);
        memory.setRightVisibility(1.0);
        
        calibrationMemories.push_back(memory);
    }
}

void MemoryPlane::update() {
    if (!calibrationMode) {
        for (int i = 0; i < memories.size(); i++) {
            memories[i].update();
        }
    } else {
        for (int i = 0; i < calibrationMemories.size(); i++) {
            calibrationMemories[i].setNoiseSpeed(noiseSpeed);
            calibrationMemories[i].setOctaveMultiplier(octaveMultiplier);
            calibrationMemories[i].update();
        }
    }
}

void MemoryPlane::drawCenterWindow() {
    if (centerMute) return;
    
    if (!calibrationMode) {
        for (int i = 0; i < memories.size(); i++) {
            memories[i].drawCenter();
        }
    } else {
        for (int i = 0; i < calibrationMemories.size(); i++) {
            calibrationMemories[i].drawCenter();
        }
    }
}

void MemoryPlane::drawLeftWindow() {
    if (leftMute) return;
    
    if (!calibrationMode) {
        for (int i = 0; i < memories.size(); i++) {
            memories[i].drawLeftSide();
        }
    } else {
        for (int i = 0; i < calibrationMemories.size(); i++) {
            calibrationMemories[i].drawLeftSide();
        }
    }
}

void MemoryPlane::drawRightWindow() {
    if (rightMute) return;
    
    if (!calibrationMode) {
        for (int i = 0; i < memories.size(); i++) {
            memories[i].drawRightSide();
        }
    } else {
        for (int i = 0; i < calibrationMemories.size(); i++) {
            calibrationMemories[i].drawRightSide();
        }
    }
}

void MemoryPlane::setColor(ofColor _primaryColor) {
    primaryColor = _primaryColor;
}

void MemoryPlane::setCalibrationMode(bool _calibrationMode) {
    calibrationMode = _calibrationMode;
}

void MemoryPlane::setCenterMute(bool _centerMute) {
    centerMute = _centerMute;
}

void MemoryPlane::setLeftMute(bool _leftMute) {
    leftMute = _leftMute;
}

void MemoryPlane::setRightMute(bool _rightMute) {
    rightMute = _rightMute;
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

void MemoryPlane::setMemory(int index, float radius, float theta, float arcDistance, float thickness, float leftVisibility, float rightVisibility, float noiseSpeed, float octaveMultiplier) {
    
    index -= 1;
    
    if (index >= 0 && index < memories.size()) {
        memories[index].setRadius(radius);
        memories[index].setTheta(theta);
        memories[index].setArcDistance(arcDistance);
        memories[index].setThickness(thickness);
        memories[index].setNoiseSpeed(noiseSpeed);
        memories[index].setOctaveMultiplier(octaveMultiplier);
        memories[index].setLeftVisibility(leftVisibility);
        memories[index].setRightVisibility(rightVisibility);
    }
}
