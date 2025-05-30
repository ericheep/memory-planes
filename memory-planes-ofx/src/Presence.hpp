//
//  Presence.hpp
//  memory-planes-ofx
//

#ifndef Presence_hpp
#define Presence_hpp

#include <stdio.h>
#include "ofMain.h"

class Presence {
public:
    Presence();
    
    void update();
    void setLastFrameTime(float lastFrameTime);
    void setPresence(float x, float y, float width);
    
    float x, y, blobWidth;
    int index;
    
    float lifetime, totalLifetime, lastFrameTime;
    bool isAlive;
};

#endif /* Presence_hpp */
