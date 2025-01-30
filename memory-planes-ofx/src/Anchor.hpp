//
//  Anchor.hpp
//  memoryPlanes
//

#ifndef Anchor_hpp
#define Anchor_hpp

#include <stdio.h>
#include "ofMain.h"

class Anchor {
public:
    Anchor(int numAnchors);
  
    void update();
    
    void setDirection(int direction);
    void setTheta(float theta);
    void setRadius(float radius);
    void setThickness(float thickness);
    void setDeviation(float deviation);
    void setNoiseTime(float noiseTime);
    void setOctaveMultiplier(float setOctaveMultiplier);
    
    ofVec2f getPoint();
    ofVec2f getHeading();
    
private:
    ofVec2f originPoint, originHeading;
    ofVec2f currentPoint, currentHeading;
    
    int numAnchors, direction;
    float theta, radius, velocity, deviation, thickness, arcDistance, octaveMultiplier;
    float nM, nT, nH, nO, noiseTime;
    
};

#endif /* Anchor_hpp */
