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
    index -= 1;
    if (index >= 0 && index < memories.size()) {
        memories[index].flip(_theta);
    }
}

int MemoryPlane::getMemoryVectorIndex(int index) {
    for (int i = 0; i < memories.size(); i++) {
        if (memories[i].index == index) return i;
    }
    
    return -1;
}

void MemoryPlane::setMemory(int index, float radius, float theta, float arcDistance, float thickness, float minFollow, float maxFollow, float noiseSpeed, float octaveMultiplier) {
    index -= 1;
    
    int vectorIndex = getMemoryVectorIndex(index);

    if (vectorIndex >= 0) {
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
        
        cout << "ADD" << index << endl;
    }
}
