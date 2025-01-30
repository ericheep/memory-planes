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
    Memory(int width, int sideHeight, int mainHeight, float theta);
    
    void update();
    void updateTear(Tear &tear, float visibility);

    void drawLeftSide();
    void drawRightSide();
    void drawFresnel();
    
    void setColor(ofColor primaryColor);
    
    void setDistance(float distance);
    void setRadius(float radius);
    void setTheta(float theta);
    void setArcDistance(float arcDistance);
    void setThickness(float thickness);
    void setNoiseTime(float noiseTime);
    void keepAlive();
    void setFill(bool fillState);
    void setLeftVisibility(float visibility);
    void setRightVisibility(float visibility);
    void setNoiseSpeed(float speed);
    void setOctaveMultiplier(float octaveMultiplier);
    
    int index;
    float lifetime;

private:
    void drawShape(ofPolyline &polygon);
    
    float windowRadius, noiseTime;
    
    float leftVisibility, rightVisibility;
    float distance, arcDistance, theta, radius;
    float length, thickness, noiseSpeed, octaveMultiplier;

    float targetLeftVisibility, targetRightVisibility;
    float targetDistance, targetArcDistance, targetTheta, targetRadius;
    float targetThickness, targetNoiseSpeed, targetOctaveMultiplier;
    
    bool fillState;
    
    int width, sideHeight, mainHeight;
    ofColor primaryColor;
    
    Tear mainTear, leftTear, rightTear;
};

#endif /* Memory_hpp */
