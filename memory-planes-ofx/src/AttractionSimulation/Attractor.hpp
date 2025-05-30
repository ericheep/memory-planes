//
//  Attractor.hpp
//  memory-planes-ofx
//

#ifndef Attractor_hpp
#define Attractor_hpp

#include <stdio.h>
#include "ofMain.h"

class Attractor {
public:
    Attractor();
    Attractor(int index);
    
    void update();
    void draw();
    
    void setPosition(ofVec3f position);
    void setLastFrameTime(float lastFrameTime);
    
    int index;
    float lifetime;
    float totalLifetime;
    float lastFrameTime;
    
    ofVec3f position;
    int numberParticles, maxNumberParticles;
    
    bool canAddParticle, isAlive;
    float spawnTimer, spawnTime;
};

#endif /* Attractor_hpp */
