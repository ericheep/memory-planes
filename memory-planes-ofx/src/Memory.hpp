//
//  Memory.hpp
//  memoryPlanes
//

#ifndef Memory_hpp
#define Memory_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Tear.hpp"

class Memory {
public:
    Memory();
    Memory(int width, int height);
    
    void update();
    void updateTear(Tear &tear, float visibility);

    void drawLeftSide();
    void drawRightSide();
    void drawCenter();
    
    void setColor(ofColor primaryColor);
    
    void setRadius(float radius);
    void setTheta(float theta);
    void setArcDistance(float arcDistance);
    void setThickness(float thickness);
    void setFill(bool fillState);
    void setLeftVisibility(float visibility);
    void setRightVisibility(float visibility);
    void setNoiseSpeed(float speed);
    void setOctaveMultiplier(float octaveMultiplier);
    
    int index;
private:
    void drawShape(ofPolyline &polygon);
    
    float windowRadius, noiseTime;
    
    float arcDistance, theta, radius;
    float length, thickness, noiseSpeed, octaveMultiplier;
    float leftVisibility, rightVisibility;

    float targetArcDistance, targetTheta, targetRadius;
    float targetThickness, targetNoiseSpeed, targetOctaveMultiplier;
    float targetLeftVisibility, targetRightVisibility;
    
    bool fillState;
    
    int width, height;
    ofColor primaryColor;
    
    Tear centerTear, leftTear, rightTear;
};

#endif /* Memory_hpp */
