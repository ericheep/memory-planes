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
}

void MemoryPlane::update() {
    float lastFrameTime = ofGetLastFrameTime();
    
    for (int i = 0; i < memories.size(); i++) {
        memories[i].lastFrameTime = lastFrameTime;
        memories[i].update();
    }
    
    memories.erase(std::remove_if(memories.begin(), memories.end(), [](Memory& memory) {
        return !memory.isAlive;
    }), memories.end());
}

void MemoryPlane::draw() {
    for (int i = 0; i < memories.size(); i++) {
        memories[i].draw();
    }
}

void MemoryPlane::flip(int index, float _theta) {
    int vectorIndex = getMemoryVectorIndex(index);
    if (vectorIndex > -1) memories[vectorIndex].flip(_theta);
}

void MemoryPlane::fragment(int index) {
    int vectorIndex = getMemoryVectorIndex(index);
    if (vectorIndex > -1) memories[vectorIndex].fragment();
}

void MemoryPlane::setInstability(int index, float instability) {
    int vectorIndex = getMemoryVectorIndex(index);
    if (vectorIndex > -1) memories[vectorIndex].setInstability(instability);
}


int MemoryPlane::getMemoryVectorIndex(int index) {
    for (int i = 0; i < memories.size(); i++) {
        if (memories[i].index == index) return i;
    }
    
    return -1;
}

void MemoryPlane::setMemory(int index, float radius, float theta, float arcDistance, float thickness, float minFollow, float maxFollow, float noiseSpeed, float octaveMultiplier) {
    int vectorIndex = getMemoryVectorIndex(index);

    if (vectorIndex > -1) {
        memories[vectorIndex].setRadius(radius);
        memories[vectorIndex].setTheta(theta);
        memories[vectorIndex].setArcDistance(arcDistance);
        memories[vectorIndex].setThickness(thickness);
        memories[vectorIndex].setFollow(minFollow, maxFollow);
        memories[vectorIndex].setNoiseSpeed(noiseSpeed);
        memories[vectorIndex].setOctaveMultiplier(octaveMultiplier);
        memories[vectorIndex].lifetime = 0;
    } else {
        Memory memory = Memory(width, height);
        
        memory.index = index;
        memory.setRadius(radius);
        memory.setTheta(theta);
        memory.setArcDistance(arcDistance);
        memory.setThickness(thickness);
        memory.setFollow(minFollow, maxFollow);
        memory.setNoiseSpeed(noiseSpeed);
        memory.setOctaveMultiplier(octaveMultiplier);
        memory.lifetime = 0;

        memories.push_back(memory);
    }
}
