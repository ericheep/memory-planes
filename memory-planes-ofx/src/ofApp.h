#pragma once

#define _USE_MATH_DEFINES
 
#include <cmath>
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxQuadWarp.h"
#include "ofxGui.h"

#include "MemoryPlane.hpp"

#define SENDING_PORT 6543
#define RECEIVING_PORT 5432

#define DEV_MODE false

class ofApp : public ofBaseApp{
public:
    void setup();
    void setupWarpers();
    void setupGui();
    void setupWarper(ofxQuadWarp &warper, int width, int height);
    
    void update();
    void updateCenterFBO();
    void updateLeftFBO();
    void updateRightFBO();
    
    void draw();
    void drawCenterMindow(ofEventArgs & args);
    void drawLeftWindow(ofEventArgs & args);
    void drawRightWindow(ofEventArgs & args);
    
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
    
    ofxQuadWarp centerWarper;
    ofxQuadWarp leftWarper;
    ofxQuadWarp rightWarper;
    
    Boolean centerWarperActive;
    Boolean leftWarperActive;
    Boolean rightWarperActive;
    
    ofxPanel gui;
    
    ofParameter<float> scale;
    ofParameter<float> blurAmount;
    ofParameter<float> shaderNoiseAmount;
    ofParameter<float> defaultOctaveMultiplier;
    ofParameter<float> defaultNoiseSpeed;
    
    ofParameter<bool> calibrationMode;
    ofParameter<bool> centerMute;
    ofParameter<bool> leftMute;
    ofParameter<bool> rightMute;

private:
    void updateOsc();
   
    ofShader blur;
    ofShader noise;
          
    ofFbo fboCenterWindow;
    ofFbo fboCenterBlur;
    ofFbo fboCenterNoise;
    
    ofFbo fboLeftWindow;
    ofFbo fboLeftBlur;
    ofFbo fboLeftNoise;
    
    ofFbo fboRightWindow;
    ofFbo fboRightBlur;
    ofFbo fboRightNoise;
    
    ofCamera cam;
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    ofColor primaryColor;
    ofColor backgroundColor;
    
    ofPoint points[10];
    
    MemoryPlane memoryPlane;
    
    float width, height;
    float radius, shaderNoiseTime;
    bool guiActive;
};
