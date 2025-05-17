//
//  MemoryPlane.hpp
//  memoryPlanes
//

#ifndef MemoryPlane_hpp
#define MemoryPlane_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Memory.hpp"

class MemoryPlane {
public:
    MemoryPlane();
    MemoryPlane(int width, int height);
    
    void update();
    void draw();

    void setCalibrationMode(bool calibrationMode);
    void flip(int index, float theta);
    
    void setMemory(int index, float radius, float theta, float length, float thickness, float noiseSpeed, float octaveMultiplier);
    
private:
    bool calibrationMode;
    int width, height;

    vector <Memory> memories;
    vector <Memory> calibrationMemories;
    ofColor primaryColor;
};

#endif /* MemoryPlane_hpp */
