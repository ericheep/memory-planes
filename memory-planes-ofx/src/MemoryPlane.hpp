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

    void flip(int index, float theta);
    
    void setMemory(int index, float radius, float theta, float arcDistance, float thickness, float minFollow, float maxFollow, float noiseSpeed, float octaveMultiplier);
    
private:
    int width, height;

    vector <Memory> memories;
    ofColor primaryColor;
};

#endif /* MemoryPlane_hpp */
