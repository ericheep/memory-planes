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
            calibrationMemories[i].update();
        }
    }
}

void MemoryPlane::draw() {    
    if (!calibrationMode) {
        for (int i = 0; i < memories.size(); i++) {
            memories[i].draw();
        }
    } else {
        for (int i = 0; i < calibrationMemories.size(); i++) {
            calibrationMemories[i].draw();
        }
    }
}

void MemoryPlane::setCalibrationMode(bool _calibrationMode) {
    calibrationMode = _calibrationMode;
}

void MemoryPlane::flip(int index, float _theta) {
    index -= 1;
    if (index >= 0 && index < memories.size()) {
        memories[index].flip(_theta);
    }
}

void MemoryPlane::setMemory(int index, float radius, float theta, float arcDistance, float thickness, float noiseSpeed, float octaveMultiplier) {
    index -= 1;
    
    if (index >= 0 && index < memories.size()) {
        memories[index].setRadius(radius);
        memories[index].setTheta(theta);
        memories[index].setArcDistance(arcDistance);
        memories[index].setThickness(thickness);
        memories[index].setNoiseSpeed(noiseSpeed);
        memories[index].setOctaveMultiplier(octaveMultiplier);
    }
}
