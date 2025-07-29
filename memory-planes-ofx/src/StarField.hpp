//
//  StarField.hpp
//

#ifndef StarField_hpp
#define StarField_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxQuadWarp.h"
#include "FluidSystem2D.hpp"
#include "Presence.hpp"

class StarField {
public:
    StarField();
    
    void update();
    void draw();
    void setWarp(ofxQuadWarp innerWarper, ofxQuadWarp outerWarper);
    void setSize(float width, float height);
    void setInnerSize(float innerWidth, float innerHeight);
    void setInvertBounds(bool isInverted);
        
    ofPolyline innerBounds;
    FluidSystem2D fluidSystem;
    
    float width, height;
    float innerWidth, innerHeight;
    float leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale;
    
    ofParameter<int> drawMode;
    ofParameter<int> numberParticles;
    ofParameter<float> timeScalar;
    ofParameter<float> influenceRadius;
    ofParameter<float> gravityMultiplier;
    ofParameter<float> targetDensity;
    ofParameter<float> pressureMultiplier;
    ofParameter<float> nearPressureMultiplier;
    ofParameter<float> gravityTheta;
    
    ofParameter<int> boundsWidth, boundsHeight;
    ofParameter<int> borderOffset;
    
    ofParameter<float> velocityCurve;
    ofParameter<float> minVelocity, maxVelocity;
    ofParameter<float> minSize, maxSize;
    ofParameter<float> connectionRadius;
    
    void setDrawMode(int& drawMode);
    void setNumberParticles(int& numberParticles);
    void setTimeScalar(float& timeScalar);
    void setInfluenceRadius(float& influenceRadius);
    void setGravityMultiplier(float& gravityMultiplier);
    void setTargetDensity(float& targetDensity);
    void setPressureMultiplier(float& pressureMultiplier);
    void setNearPressureMultiplier(float& nearPressureMultiplier);
    void setGravityTheta(float& gravityTheta);
    void setCoolColor(ofColor& coolColor);
    void setHotColor(ofColor& hotColor);
    
    // gui graphic listener functions
    void setVelocityCurve(float& velocityCurve);
    void setMinVelocity(float& minVelocity);
    void setMaxVelocity(float& maxVelocity);
    void setMinSize(float& minSize);
    void setMaxSize(float& maxSize);
    void setLineThickness(float& lineThickness);
    
    void setConnectionRadius(float& connectionRadius);
    void setSkew(float leftBoundsScale, float rightBoundsScale, float backBoundsScale, float frontBoundsScale);
    
    void setPresence(int index, float x, float y, float width);
    int getPresenceVectorIndex(int index);
    void addPresence(int index, float x, float y, float blobWidth);
    
    void setBoundaryState(int boundaryState);
    vector<Presence> presences;
};

#endif /* StarField_hpp */
