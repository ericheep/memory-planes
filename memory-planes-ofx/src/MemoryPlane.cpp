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
    
    // memories
    for (int i = 0; i < 5; i++) {
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
}

void MemoryPlane::update() {
    for (int i = 0; i < memories.size(); i++) {
        memories[i].update();
    }
}

void MemoryPlane::draw() {
    for (int i = 0; i < memories.size(); i++) {
        memories[i].draw();
    }
}

void MemoryPlane::flip(int index, float _theta) {
    index -= 1;
    if (index >= 0 && index < memories.size()) {
        memories[index].flip(_theta);
    }
}

void MemoryPlane::setMemory(int index, float radius, float theta, float arcDistance, float thickness, float minFollow, float maxFollow, float noiseSpeed, float octaveMultiplier) {
    index -= 1;
    
    if (index >= 0 && index < memories.size()) {
        memories[index].setRadius(radius);
        memories[index].setTheta(theta);
        memories[index].setArcDistance(arcDistance);
        memories[index].setThickness(thickness);
        memories[index].setFollow(minFollow, maxFollow);
        memories[index].setNoiseSpeed(noiseSpeed);
        memories[index].setOctaveMultiplier(octaveMultiplier);
    }
}
