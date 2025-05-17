#pragma once

#define _USE_MATH_DEFINES
 
#include <cmath>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxQuadWarp.h"
#include "ofxGui.h"

#include "MemoryPlane.hpp"
#include "StarField.hpp"

#define SENDING_PORT 6543
#define RECEIVING_PORT 5432

#define DEV_MODE true

class ofApp : public ofBaseApp{
public:
    void setup();
    void setupWarpers();
    void setupGui();
    void setupWarper(ofxQuadWarp &warper, int width, int height);
    
    void update();
    void updateInnerFBO();
    void updateOuterFBO();
    
    void draw();
    void drawInnerWindow(ofEventArgs & args);
    void drawOuterWindow(ofEventArgs & args);
    
    void drawFps();
    void drawWarpPoints(ofxQuadWarp warper, ofMatrix4x4 warpMatrix);

    void setCalibrationMode(bool &calibrationMode);
    void setCenterMute(bool &fresnelMute);
    void setLeftMute(bool &leftMute);
    void setRightMute(bool &rightMute);
    
    void exit();
    void saveWarp(const string& path, ofxQuadWarp warper);
    void loadWarp(const string& path, ofxQuadWarp &warper);
    void keyPressed(int key);
    
    ofxQuadWarp innerWarper;
    ofxQuadWarp outerWarper;
    
    Boolean innerWarperActive;
    Boolean outerWarperActive;
    
    ofxPanel gui;
    ofParameterGroup simulationSettings;
    ofParameterGroup boundarySettings;
    
    ofParameter<float> scale;
    ofParameter<float> blurAmount;
    ofParameter<float> innerNoiseAmount;
    ofParameter<float> outerNoiseAmount;
    ofParameter<float> defaultOctaveMultiplier;
    ofParameter<float> defaultNoiseSpeed;
    
    ofParameter<bool> calibrationMode;

private:
    void updateOsc();
   
    ofShader blur;
    ofShader innerNoise;
    ofShader innerRadialNoise;
    ofShader outerNoise;
    ofShader outerRadialNoise;
    
    ofFbo fboInnerWindow;
    ofFbo fboInnerBlur;
    ofFbo fboInnerNoise;
    ofFbo fboInnerRadialNoise;
    
    ofFbo fboOuterWindow;
    ofFbo fboOuterBlur;
    ofFbo fboOuterNoise;
    ofFbo fboOuterRadialNoise;
    
    ofCamera cam;
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    ofColor primaryColor;
    ofColor backgroundColor;
    
    ofPoint points[10];
    
    MemoryPlane memoryPlane;
    StarField starField;
    
    float width, height, innerWidth, innerHeight;
    float radius, innerNoiseTime, outerNoiseTime;
    bool guiActive;
};
