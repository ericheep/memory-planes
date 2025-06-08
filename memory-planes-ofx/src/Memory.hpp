//
//  Memory.hpp
//  memoryPlanes
//

#ifndef Memory_hpp
#define Memory_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Tear.hpp"
#include "Fragments.hpp"

class Memory {
public:
    Memory();
    Memory(int width, int height);
    
    void update();
    void updateTear(Tear &tear, float visibility);
    void draw();
    
    void setColor(ofColor primaryColor);
    
    void setRadius(float radius);
    void setTheta(float theta);
    void setArcDistance(float arcDistance);
    void setThickness(float thickness);
    void setFill(bool fillState);
    void setNoiseSpeed(float speed);
    void setOctaveMultiplier(float octaveMultiplier);
    void setFollow(float minFollow, float maxFollow);
    void setInstability(float instability);
    
    // actions
    void flip(float theta);
    void fragment();
    
    int index;
    float lifetime, totalLifetime;
    float birthTime, birthTimer;
    float lastFrameTime;
    float instability;
    bool isAlive;
private:
    void drawShape(ofPolyline &polygon);
    
    float windowRadius, noiseTime, follow;
    
    float arcDistance, theta, radius;
    float length, thickness, noiseSpeed, octaveMultiplier;
    
    float targetArcDistance, targetTheta, targetRadius;
    float targetThickness, targetNoiseSpeed, targetOctaveMultiplier;
    
    bool isFilled;
    
    int width, height;
    ofColor primaryColor;
    
    Tear tear;
    Fragments fragments;
};

#endif /* Memory_hpp */
