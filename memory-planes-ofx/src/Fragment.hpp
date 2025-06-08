//
//  Fragment.hpp
//  memory-planes-ofx
//

#ifndef Fragment_hpp
#define Fragment_hpp

#include <stdio.h>
#include "ofMain.h"

class Fragment {
public:
    Fragment();
    Fragment(float radius, float theta, float arcLength, float stability);
    
    void update();
    void draw();
    void setLastFrameTime(float lastFrameTime);
    
    bool isAlive;
    float travelSize, size, speed, direction, velocity;
    float travelRadius, radius;
    float theta, arcLength, travelTheta;
    float totalLifetime, lifetime, lastFrameTime;
    float instability;
    int weird;
    
    ofPoint position;
};

#endif /* Fragment_hpp */
