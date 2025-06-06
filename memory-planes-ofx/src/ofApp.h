#pragma once

#define _USE_MATH_DEFINES
 
#include <cmath>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxQuadWarp.h"
#include "ofxGui.h"

#include "MemoryPlane.hpp"
#include "StarField.hpp"
#include "Emanations.hpp"

#define RECEIVING_PORT 6543
#define SENDING_PORT 7654

#define DEV_MODE false

class ofApp : public ofBaseApp{
public:
    void setup() override;
    void setupWarpers();
    void setupGui();
    void setupWarper(ofxQuadWarp &warper, int width, int height);
    
    void update() override;
    void updateState();
    void updateInnerFBO();
    void updateOuterFBO();
    void updateOverallFBO();
    
    void draw() override;
    void drawInnerWindow(ofEventArgs & args);
    void drawOuterWindow(ofEventArgs & args);
    
    void drawFps();
    void drawWarpPoints(ofxQuadWarp warper, ofMatrix4x4 warpMatrix);
    
    void exit() override;
    void saveWarp(const string& path, ofxQuadWarp warper);
    void loadWarp(const string& path, ofxQuadWarp &warper);
    void keyPressed(int key) override;
    
    void setLeftBoundsScale(float &leftBoundsScale);
    void setRightBoundsScale(float &rightBoundsScale);
    void setBackBoundsScale(float &backBoundsScale);
    void setFrontBoundsScale(float &frontBoundsScale);

    void onMouseMoved(ofMouseEventArgs& mouseArgs);
    void onMouseDragged(ofMouseEventArgs& mouseArgs);
    void onMouseReleased(ofMouseEventArgs& mouseArgs);
    
    ofxQuadWarp innerWarper;
    ofxQuadWarp outerWarper;
    
    bool innerWarperActive;
    bool outerWarperActive;
    
    ofxPanel gui;
    ofParameterGroup simulationSettings;
    ofParameterGroup emanationSettings;
    ofParameterGroup boundarySettings;
    
    ofParameter<float> scale;
    ofParameter<float> innerBlurAmount;
    ofParameter<float> outerBlurAmount;
    ofParameter<float> overallBlurAmount;
    ofParameter<float> innerNoiseAmount;
    ofParameter<float> outerNoiseAmount;
    ofParameter<float> innerRadialNoiseAmount;
    ofParameter<float> outerRadialNoiseAmount;
    ofParameter<float> defaultOctaveMultiplier;
    ofParameter<float> defaultNoiseSpeed;
    
    ofParameter<float> leftBoundsScale;
    ofParameter<float> rightBoundsScale;
    ofParameter<float> frontBoundsScale;
    ofParameter<float> backBoundsScale;
        
    enum State { idle, firefly, converge, memory };
    State state;
private:
    void updateOsc();
   
    ofShader innerBlur;
    ofShader innerNoise;
    ofShader innerRadialNoise;
    
    ofShader outerBlur;
    ofShader outerNoise;
    ofShader outerRadialNoise;
    
    ofShader overallBlur;
    ofShader overallNoise;
    ofShader overallRadialNoise;
    
    ofFbo fboInnerWindow;
    ofFbo fboInnerBlur;
    ofFbo fboInnerNoise;
    ofFbo fboInnerRadialNoise;
    
    ofFbo fboOuterWindow;
    ofFbo fboOuterBlur;
    ofFbo fboOuterNoise;
    ofFbo fboOuterRadialNoise;
    
    ofFbo fboOverallWindow;
    ofFbo fboOverallBlur;
    ofFbo fboOverallNoise;
    ofFbo fboOverallRadialNoise;
    
    ofxOscSender oscSender;
    ofxOscReceiver oscReceiver;
    
    ofColor primaryColor;
    ofColor backgroundColor;
    
    ofPoint points[10];
    
    MemoryPlane memoryPlane;
    StarField starField;
    Emanations emanations;
    
    float width, height, innerWidth, innerHeight;
    float radius, innerNoiseTime, outerNoiseTime;
    float overallBlurRadius;
    bool guiActive;
};
