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
    
    void setCenter(ofVec2f center);
    void setEmanation(int index, float x, float y, float width);
    void setAttractor(float x, float y, float width);
    void setAttractorY(float &attractorY);
    void setSkew(float leftBoundsScale, float rightBoundsScale, float backBoundsScale, float frontBoundsScale);

    void addGlow();
    void createMesh();
    
    float width, height, depth;
    float rectangleWidth;
    ofMesh mesh;
    ofVec3f position;
    float leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale;
    
    vector<Glow> glows;
    AttractionSystem attractionSystem;
    bool isConverging, isAttracting;
    
    ofParameter<float> velocityCurve;
    ofParameter<float> minVelocity, maxVelocity;
    ofParameter<float> minSize, maxSize;
    ofParameter<float> connectionRadius;
    ofParameter<float> attractorY;
};

#endif /* Emanations_hpp */
