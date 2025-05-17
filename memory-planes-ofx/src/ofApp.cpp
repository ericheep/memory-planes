#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(TRUE);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    
    if (DEV_MODE) {
        width = 800;
        height = 600;
        
        innerWidth = 400;
        innerHeight = 400;
    } else {
        width = ofGetWidth();
        height = ofGetHeight();
        
        innerWidth = height * 0.6;
        innerHeight = height * 0.6;
    }
    
    // gl settings
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    // osc settings
    oscSender.setup("localhost", SENDING_PORT);
    oscReceiver.setup(RECEIVING_PORT);
    
    // load shaders
    blur.load("shaders/blur");
    innerRadialNoise.load("shaders/radialNoise");
    outerRadialNoise.load("shaders/radialNoise");
    
    fboInnerWindow.allocate(innerWidth, innerHeight, GL_RGB32F_ARB);
    fboInnerBlur.allocate(innerWidth, innerHeight, GL_RGB32F_ARB);
    fboInnerNoise.allocate(innerWidth, innerHeight, GL_RGB32F_ARB);
    
    fboOuterWindow.allocate(width, height, GL_RGB32F_ARB);
    fboOuterBlur.allocate(width, height, GL_RGB32F_ARB);
    fboOuterNoise.allocate(width, height, GL_RGB32F_ARB);
    
    // initialize colors
    backgroundColor = ofColor::black;
    primaryColor = ofColor::white;
    
    // plane
    memoryPlane = MemoryPlane(innerWidth, innerHeight);
    
    starField.setInnerSize(innerWidth, innerHeight);
    starField.setSize(width, height);
    
    setupWarpers();
    setupGui();
    innerNoiseTime = 0.0f;
    outerNoiseTime = 0.0f;
}

void ofApp::setupWarpers() {
    setupWarper(innerWarper, innerWidth, innerHeight);
    // loadWarp("innerWarper.xml", innerWarper);
    innerWarper.hide();
    innerWarper.disableKeyboardShortcuts();
    
    setupWarper(outerWarper, width, height);
    // loadWarp("outerWarper.xml", outerWarper);
    outerWarper.hide();
    outerWarper.disableKeyboardShortcuts();
}

void ofApp::setupGui() {
    guiActive = false;
    
    // main gui setup
    gui.setup("memory planes");
    gui.setSize(140, 12);
    gui.setDefaultWidth(120);
    gui.setDefaultHeight(12);
    
    calibrationMode.addListener(this, &ofApp::setCalibrationMode);
    
    gui.add(scale.set("scale", 1.0, 0.0, 3.0));
    gui.add(blurAmount.set("blur", 1.0, 0.0, 1.0));
    gui.add(innerNoiseAmount.set("inner noise", 1.0, 0.0, 1.0));
    gui.add(outerNoiseAmount.set("outer noise", 1.0, 0.0, 1.0));
    gui.add(calibrationMode.set("calibration mode", false));
    
    // general gui settings
    gui.add(starField.numberParticles.set("number", 100, 50, 5000));
    gui.add(starField.influenceRadius.set("influence radius", 10.0, 0.5, 75.0));
    gui.add(starField.timeScalar.set("time scalar", 1.0, 0.25, 4.0));
    gui.add(starField.gravityMultiplier.set("gravity multiplier", 0.0, 0.0, 5.0));
    
    // graphic gui settings
    gui.add(starField.connectionRadius.set("connection radius", 0, 0, 200.0));
    gui.add(starField.drawMode.set("draw mode", 0, 0, 5));
    gui.add(starField.velocityCurve.set("velocity curve", 1.0, 0.0, 3.0));
    gui.add(starField.minVelocity.set("min velocity", 0.0, 0.0, 100.0));
    gui.add(starField.maxVelocity.set("max velocity", 50.0, 0.0, 250.0));
    gui.add(starField.minSize.set("min size", 1.0, 0.0, 50.0));
    gui.add(starField.maxSize.set("max size", 25.0, 0.0, 100.0));

    simulationSettings.setName("sim settings");
    simulationSettings.add(starField.targetDensity.set("density", 1.0, 0.125, 3.0));
    simulationSettings.add(starField.pressureMultiplier.set("pressure", 100, 0, 1000.0));
    simulationSettings.add(starField.nearPressureMultiplier.set("near pressure", 100, 0.0, 1000.0));
    gui.add(simulationSettings);
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
    updateOsc();
    memoryPlane.update();
    
    starField.setWarp(innerWarper, outerWarper);
    starField.update();
    
    updateInnerFBO();
    updateOuterFBO();
    
    innerNoiseTime += innerNoiseAmount * 0.06;
    outerNoiseTime += outerNoiseAmount * 0.06;
}

//--------------------------------------------------------------
void ofApp::drawFps() {
    std::stringstream strm;
    strm << setprecision(3) << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

void ofApp::draw() {
    ofBackground(backgroundColor);
    ofMatrix4x4 innerMatrix = innerWarper.getMatrix();
    ofMatrix4x4 outerMatrix = outerWarper.getMatrix();

    ofPushMatrix();
    ofMultMatrix(outerMatrix);
    ofSetColor(ofColor::white);
    fboOuterBlur.draw(0, 0);
    ofPopMatrix();
        
    ofPushMatrix();
    ofMultMatrix(innerMatrix);
    ofSetColor(ofColor::white);
    fboInnerBlur.draw(0, 0);
    ofPopMatrix();
        
    drawWarpPoints(innerWarper, innerWarper.getMatrix());
    drawWarpPoints(outerWarper, outerWarper.getMatrix());
    
    drawFps();
    
    if (!guiActive) {
        ofHideCursor();
        return;
    } else {
        ofShowCursor();
    }
    
    gui.draw();
}

void ofApp::updateInnerFBO() {
    // inner fbo
    fboInnerWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(innerWidth / 2.0, innerHeight / 2.0, 0);
    ofScale(scale);
    memoryPlane.draw();
    ofPopMatrix();
    fboInnerWindow.end();
    
    // noise fbo
    fboInnerNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    innerRadialNoise.begin();
    innerRadialNoise.setUniform1f("u_distortion", innerNoiseAmount);
    innerRadialNoise.setUniform1f("u_time", innerNoiseTime);
    innerRadialNoise.setUniform2f("u_resolution", innerWidth, innerHeight);
    fboInnerWindow.draw(0, 0);
    innerNoise.end();
    fboInnerNoise.end();
    
    // blur fbo
    fboInnerBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    blur.begin();
    blur.setUniform1f("u_blurMix", blurAmount);
    fboInnerNoise.draw(0, 0);
    blur.end();
    fboInnerBlur.end();
}

void ofApp::updateOuterFBO() {
    // inner fbo
    fboOuterWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    starField.draw();
    ofPopMatrix();
    fboOuterWindow.end();
    
    // noise fbo
    fboOuterNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    outerRadialNoise.begin();
    outerRadialNoise.setUniform1f("u_distortion", outerNoiseAmount);
    outerRadialNoise.setUniform1f("u_time", outerNoiseTime);
    outerRadialNoise.setUniform2f("u_resolution", width, height);
    fboOuterWindow.draw(0, 0);
    outerNoise.end();
    fboOuterNoise.end();
    
    // blur fbo
    fboOuterBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    blur.begin();
    blur.setUniform1f("u_blurMix", blurAmount);
    fboOuterNoise.draw(0, 0);
    blur.end();
    fboOuterBlur.end();
}

void ofApp::keyPressed(int key) {
    switch(key) {
        case 'l':
            loadWarp("innerWarper.xml", innerWarper);
            loadWarp("outerWarper.xml", outerWarper);
            break;
        case 's':
            saveWarp("innerWarper.xml", innerWarper);
            saveWarp("outerWarper.xml", outerWarper);
            gui.saveToFile("settings.xml");
            break;
        case 'i':
            innerWarper.enableKeyboardShortcuts();
            innerWarper.show();
            outerWarper.disableKeyboardShortcuts();
            outerWarper.hide();
            break;
        case 'o':
            innerWarper.disableKeyboardShortcuts();
            innerWarper.hide();
            outerWarper.enableKeyboardShortcuts();
            outerWarper.show();
            break;
        case 'h':
            innerWarper.disableKeyboardShortcuts();
            innerWarper.hide();
            outerWarper.disableKeyboardShortcuts();
            outerWarper.hide();
            break;
        case 'g':
            guiActive = !guiActive;
            break;
    }
}

void ofApp::exit() {
    saveWarp("innerWarper.xml", innerWarper);
    saveWarp("outerWarper.xml", outerWarper);
}

void ofApp::setupWarper(ofxQuadWarp &warper, int width, int height) {
    float halfWidth = width * 0.5;
    float halfHeight = height * 0.5;
    
    ofPoint center = ofPoint(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
    
    // top left, top right, bottom left, bottom right
    ofPoint p1 = center + ofPoint(-halfWidth, -halfHeight);
    ofPoint p2 = center + ofPoint(halfWidth, -halfHeight);
    ofPoint p3 = center + ofPoint(-halfWidth, halfHeight);
    ofPoint p4 = center + ofPoint(halfWidth, halfHeight);
    
    warper.setSourceRect(ofRectangle(0, 0, width, height));
    warper.setTopLeftCornerPosition(p1);
    warper.setTopRightCornerPosition(p2);
    warper.setBottomLeftCornerPosition(p3);
    warper.setBottomRightCornerPosition(p4);
    
    warper.setup();
}

void ofApp::drawWarpPoints(ofxQuadWarp warper, ofMatrix4x4 warpMatrix) {
    ofSetLineWidth(2);
    ofSetColor(ofColor::cyan);
    
    for(int i = 0; i < 9; i++) {
        int j = i + 1;
        
        ofVec3f p1 = warpMatrix.preMult(ofVec3f(points[i].x, points[i].y, 0));
        ofVec3f p2 = warpMatrix.preMult(ofVec3f(points[j].x, points[j].y, 0));
        
        ofDrawLine(p1.x, p1.y, p2.x, p2.y);
    }
    
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
}

//----------------------------------------------------- save / load.
void ofApp::saveWarp(const string& path, ofxQuadWarp warper) {
    ofXml xml;
    xml.appendChild("quadwarp");
    
    ofXml src = xml.getChild("quadwarp").appendChild("src");
    for (int i = 0; i < 4; i++) {
        auto t = src.appendChild("point");
        t.setAttribute("x", ofToString(warper.srcPoints[i].x));
        t.setAttribute("y", ofToString(warper.srcPoints[i].y));
    }
    
    ofXml dst = xml.getChild("quadwarp").appendChild("dst");
    for (int i = 0; i < 4; i++) {
        auto t = dst.appendChild("point");
        t.setAttribute("x", ofToString(warper.dstPoints[i].x));
        t.setAttribute("y", ofToString(warper.dstPoints[i].y));
    }
    
    xml.save(path);
}

void ofApp::loadWarp(const string& path, ofxQuadWarp &warper) {
    ofXml xml;
    if (!xml.load(path)) return;
    
    auto src = xml.getChild("quadwarp").getChild("src");
    int i = 0;
    for (auto it = src.getChildren().begin(); it != src.getChildren().end(); it++) {
        warper.srcPoints[i].x = it->getAttribute("x").getFloatValue();
        warper.srcPoints[i].y = it->getAttribute("y").getFloatValue();
        i++;
    }
    
    auto dst = xml.getChild("quadwarp").getChild("dst");
    i = 0;
    for (auto it = dst.getChildren().begin(); it != dst.getChildren().end(); it++) {
        warper.dstPoints[i].x = it->getAttribute("x").getFloatValue();
        warper.dstPoints[i].y = it->getAttribute("y").getFloatValue();
        i++;
    }
}

void ofApp::setCalibrationMode(bool &calibrationMode) {
    memoryPlane.setCalibrationMode(calibrationMode);
}

void ofApp::updateOsc() {
    while(oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        if (m.getAddress() == "/m") {
            int index = m.getArgAsInt(0);
            float radius = m.getArgAsFloat(1);
            float theta = m.getArgAsFloat(2);
            float arcDistance = m.getArgAsFloat(3);
            float thickness = m.getArgAsFloat(4);
            float noiseSpeed = m.getArgAsFloat(5);
            float octaveMultiplier = m.getArgAsFloat(6);
                        
            memoryPlane.setMemory(index, radius, theta, arcDistance, thickness, noiseSpeed, octaveMultiplier);
        }
        
        if (m.getAddress() == "/mouseTest") {
            float x = m.getArgAsFloat(0);
            float width = m.getArgAsFloat(1);
            starField.setPresence(x, width);
        }
        
        if (m.getAddress() == "/flip") {
            int index = m.getArgAsInt(0);
            float theta = m.getArgAsFloat(1);
            memoryPlane.flip(index, theta);
        }
    }
}

