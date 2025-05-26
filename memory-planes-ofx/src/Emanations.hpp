//
//  Emanations.hpp
//  memory-planes-ofx
//

#ifndef Emanations_hpp
#define Emanations_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Glow.hpp"

class Emanations {
public:
    Emanations();
    Emanations(float width, float height);
    
    void update();
    void draw();
    
    void setEmanation(int index, float x, float y, float width);
    void addGlow();
    
    float width, height, depth;
    float rectangleWidth;
    ofMesh mesh;
    ofVec3f position;
    Glow glow;
    
    ofParameter<float> leftBound;
    ofParameter<float> rightBound;
    ofParameter<float> frontBound;
    ofParameter<float> backBound;
};

#endif /* Emanations_hpp */
