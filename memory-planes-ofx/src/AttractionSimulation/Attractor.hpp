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
    
    void update();
    void draw();
    
    void setPosition(ofVec2f position);
    
    ofVec2f position;
    int numberParticles, maxNumberParticles;
    
    bool canAddParticle;
    float spawnTimer, spawnTime;
};

#endif /* Attractor_hpp */
