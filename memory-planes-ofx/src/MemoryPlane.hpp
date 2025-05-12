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
    
    void drawCenterWindow();
    void drawLeftWindow();
    void drawRightWindow();
    void drawMemories();
    void drawMemories(float distanceRate);
    void drawSides();
    
    void setColor(ofColor color);
    void setRadius(float radius);
    void setFresnelRadius(float fresnelRadius);
    void setCalibrationMode(bool calibrationMode);
    
    void setCenterMute(bool centerMute);
    void setLeftMute(bool leftMute);
    void setRightMute(bool rightMute);
    void setNoiseSpeed(float noiseSpeed);
    void setOctaveMultiplier(float octaveMultiplier);
    
    void setMemory(int index, float radius, float theta, float length, float thickness, float leftVisibility, float rightVisibility, float noiseSpeed, float octaveMultiplier);
    
private:
    bool calibrationMode;
    int width, height;
    
    float fresnelRadius, radius;
    float noiseTime, noiseSpeed, octaveMultiplier;
    
    vector <Memory> memories;
    vector <Memory> calibrationMemories;
    ofColor primaryColor;
    
    bool centerMute, leftMute, rightMute;
};

#endif /* MemoryPlane_hpp */
