//
//  Glow.hpp
//  memory-planes-ofx
//

#ifndef Glow_hpp
#define Glow_hpp

#include <stdio.h>
#include "ofMain.h"

class Glow {
public:
    Glow();
    Glow(float width, float height);
    
    void update();
    void draw();
  
    void setPosition(ofVec2f position);
    
    ofVec2f position;
};

#endif /* Glow_hpp */
