#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(TRUE);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    
    if (DEV_MODE) {
        width = 608;
        height = 280;
        fresnelHeight = 280;
    } else {
        width = 1920;
        height = BENQ_HEIGHT;
        fresnelHeight = PANASONIC_HEIGHT;
    }
    
    // gl settings
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    
    // osc settings
    oscSender.setup("localhost", SENDING_PORT);
    oscReceiver.setup(RECEIVING_PORT);
    
    // load shaders
    blur.load("shaders/blur");
    noise.load("shaders/noise");
    
    fboFresnel.allocate(width, fresnelHeight, GL_RGB32F_ARB);
    fboFresnelBlur.allocate(width, fresnelHeight, GL_RGB32F_ARB);
    fboFresnelNoise.allocate(width, fresnelHeight, GL_RGB32F_ARB);
    
    fboLeftWindow.allocate(width, height, GL_RGB32F_ARB);
    fboLeftBlur.allocate(width, height, GL_RGB32F_ARB);
    fboLeftNoise.allocate(width, height, GL_RGB32F_ARB);
    
    fboRightWindow.allocate(width, height, GL_RGB32F_ARB);
    fboRightBlur.allocate(width, height, GL_RGB32F_ARB);
    fboRightNoise.allocate(width, height, GL_RGB32F_ARB);
    
    // initialize colors
    backgroundColor = ofColor::black;
    primaryColor = ofColor::white;
    
    // plane
    memoryPlane = MemoryPlane(width, height, fresnelHeight);
    memoryPlane.setColor(primaryColor);
    
    setupWarpers();
    setupGui();
    shaderNoiseTime = 0.0f;
}

void ofApp::setupWarpers() {
    setupWarper(fresnelWarper, width, fresnelHeixxght);
    // loadWarp("fresnelWarper.xml", fresnelWarper);
    fresnelWarper.hide();
    fresnelWarper.disableKeyboardShortcuts();
    
    setupWarper(leftWarper, width, height);
    // loadWarp("leftWarper.xml", leftWarper);
    leftWarper.hide();
    leftWarper.disableKeyboardShortcuts();
    
    setupWarper(rightWarper, width, height);
    // loadWarp("rightWarper.xml", rightWarper);
    rightWarper.hide();
    rightWarper.disableKeyboardShortcuts();
}

void ofApp::setupGui() {
    guiActive = false;
    
    // main gui setup
    gui.setup("memory planes");
    gui.setSize(140, 12);
    gui.setDefaultWidth(120);
    gui.setDefaultHeight(12);
    
    calibrationMode.addListener(this, &ofApp::setCalibrationMode);
    fresnelMute.addListener(this, &ofApp::setFresnelMute);
    leftMute.addListener(this, &ofApp::setLeftMute);
    rightMute.addListener(this, &ofApp::setRightMute);
    
    gui.add(scale.set("scale", 1.0, 0.0, 3.0));
    gui.add(blurAmount.set("blur", 1.0, 0.0, 1.0));
    gui.add(shaderNoiseAmount.set("shader noise", 1.0, 0.0, 1.0));
    gui.add(defaultOctaveMultiplier.set("octave multiplier", 1.0, 0.0, 10.0));
    gui.add(defaultNoiseSpeed.set("noise speed", 1.0, 0.01, 1.0));
    
    gui.add(calibrationMode.set("calibration mode", false));
    gui.add(fresnelMute.set("fresnel mute", false));
    gui.add(leftMute.set("left mute", false));
    gui.add(rightMute.set("right mute", false));
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
    // updateOsc();
    memoryPlane.setOctaveMultiplier(defaultOctaveMultiplier);
    memoryPlane.setNoiseSpeed(defaultNoiseSpeed);
    memoryPlane.setRadius(radius);
    memoryPlane.update();
    
    updateMainFBO();
    updateLeftFBO();
    updateRightFBO();
    
    shaderNoiseTime = ofGetElapsedTimef() * 0.6;
}

//--------------------------------------------------------------
void ofApp::drawFps() {
    std::stringstream strm;
    strm << setprecision(3) << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

void ofApp::draw() {
    
    ofBackground(backgroundColor);
    ofMatrix4x4 fresnelMatrix = fresnelWarper.getMatrix();
    
    // warp
    ofPushMatrix();
    ofMultMatrix(fresnelMatrix);
    ofSetColor(ofColor::white);
    fboFresnelBlur.draw(0, 0);
    ofPopMatrix();
    
    drawWarpPoints(fresnelWarper, fresnelMatrix);
    
    drawWarpPoints(fresnelWarper, fresnelWarper.getMatrix());
    drawWarpPoints(leftWarper, leftWarper.getMatrix());
    drawWarpPoints(rightWarper, rightWarper.getMatrix());
    
    drawFps();
    
    if (!guiActive) {
        ofHideCursor();
        return;
    } else {
        ofShowCursor();
    }
    
    gui.draw();
}

void ofApp::updateMainFBO() {
    // fresnel fbo
    fboFresnel.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(width / 2.0, fresnelHeight / 2.0, 0);
    ofScale(scale);
    memoryPlane.drawMainWindow();
    ofPopMatrix();
    fboFresnel.end();
    
    // noise fbo
    fboFresnelNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    noise.begin();
    noise.setUniform1f("u_distortion", shaderNoiseAmount);
    noise.setUniform1f("u_time", shaderNoiseTime);
    fboFresnel.draw(0, 0);
    noise.end();
    fboFresnelNoise.end();
    
    // blur fbo
    fboFresnelBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    blur.begin();
    blur.setUniform1f("u_blurMix", blurAmount);
    fboFresnelNoise.draw(0, 0);
    blur.end();
    fboFresnelBlur.end();
}

void ofApp::updateLeftFBO() {
    // fresnel fbo
    fboLeftWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(width / 2.0, height / 2.0, 0);
    ofScale(scale);
    memoryPlane.drawLeftWindow();
    ofPopMatrix();
    fboLeftWindow.end();
    
    // noise fbo
    fboLeftNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    noise.begin();
    noise.setUniform1f("u_distortion", shaderNoiseAmount);
    noise.setUniform1f("u_time", shaderNoiseTime);
    fboLeftWindow.draw(0, 0);
    noise.end();
    fboLeftNoise.end();
    
    // blur fbo
    fboLeftBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    blur.begin();
    blur.setUniform1f("u_blurMix", blurAmount);
    fboLeftNoise.draw(0, 0);
    blur.end();
    fboLeftBlur.end();
}

void ofApp::updateRightFBO() {
    // fresnel fbo
    fboRightWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(width / 2.0, height / 2.0, 0);
    ofScale(scale);
    memoryPlane.drawRightWindow();
    ofPopMatrix();
    fboRightWindow.end();
    
    // noise fbo
    fboRightNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    noise.begin();
    noise.setUniform1f("u_distortion", shaderNoiseAmount);
    noise.setUniform1f("u_time", shaderNoiseTime);
    fboRightWindow.draw(0, 0);
    noise.end();
    fboRightNoise.end();
    
    // blur fbo
    fboRightBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    blur.begin();
    blur.setUniform1f("u_blurMix", blurAmount);
    fboRightNoise.draw(0, 0);
    blur.end();
    fboRightBlur.end();
}

void ofApp::drawLeftWindow(ofEventArgs &args) {
    ofBackground(ofColor::black);
    ofMatrix4x4 leftMatrix = leftWarper.getMatrix();
    
    // warp
    ofPushMatrix();
    ofMultMatrix(leftMatrix);
    ofSetColor(ofColor::white);
    fboLeftBlur.draw(0, 0);
    ofPopMatrix();
    
    drawWarpPoints(leftWarper, leftMatrix);
}

void ofApp::drawRightWindow(ofEventArgs &args) {
    ofBackground(ofColor::black);
     ofMatrix4x4 rightMatrix = rightWarper.getMatrix();
     
     // warp
     ofPushMatrix();
     ofMultMatrix(rightMatrix);
     ofSetColor(ofColor::white);
     fboRightBlur.draw(0, 0);
     ofPopMatrix();
     
     drawWarpPoints(rightWarper, rightMatrix);
}

void ofApp::keyPressed(int key) {
    switch(key) {
        case 'l':
            loadWarp("fresnelWarper.xml", fresnelWarper);
            loadWarp("leftWarper.xml", leftWarper);
            loadWarp("rightWarper.xml", rightWarper);
            break;
        case 's':
            saveWarp("fresnelWarper.xml", fresnelWarper);
            saveWarp("leftWarper.xml", leftWarper);
            saveWarp("rightWarper.xml", rightWarper);
            gui.saveToFile("settings.xml");
            break;
        case 'a':
            fresnelWarper.enableKeyboardShortcuts();
            fresnelWarper.show();
            leftWarper.disableKeyboardShortcuts();
            leftWarper.hide();
            rightWarper.disableKeyboardShortcuts();
            rightWarper.hide();
            break;
        case 'b':
            fresnelWarper.disableKeyboardShortcuts();
            fresnelWarper.hide();
            leftWarper.enableKeyboardShortcuts();
            leftWarper.show();
            rightWarper.disableKeyboardShortcuts();
            rightWarper.hide();
            break;
        case 'c':
            fresnelWarper.disableKeyboardShortcuts();
            fresnelWarper.hide();
            leftWarper.disableKeyboardShortcuts();
            leftWarper.hide();
            rightWarper.enableKeyboardShortcuts();
            rightWarper.show();
            break;
        case 'h':
            fresnelWarper.disableKeyboardShortcuts();
            fresnelWarper.hide();
            leftWarper.disableKeyboardShortcuts();
            leftWarper.hide();
            rightWarper.disableKeyboardShortcuts();
            rightWarper.hide();
            break;
        case 'g':
            guiActive = !guiActive;
            break;
    }
}

void ofApp::exit() {
    saveWarp("fresnelWarper.xml", fresnelWarper);
    saveWarp("leftWarper.xml", leftWarper);
    saveWarp("rightWarper.xml", rightWarper);
}

void ofApp::setupWarper(ofxQuadWarp &warper, int width, int height) {
    // top left, top right, bottom left, bottom right
    float w = width;
    float h = height;
    float offset = 15;
    
    ofPoint p1 = ofPoint(offset, offset);
    ofPoint p2 = ofPoint(w - offset, offset);
    ofPoint p3 = ofPoint(offset, h - offset);
    ofPoint p4 = ofPoint(w - offset, h - offset);
    
    warper.setSourceRect(ofRectangle(0, 0, w, h));
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

void ofApp::setFresnelMute(bool &fresnelMute) {
    memoryPlane.setFresnelMute(fresnelMute);
}

void ofApp::setLeftMute(bool &leftMute) {
    memoryPlane.setLeftMute(leftMute);
}

void ofApp::setRightMute(bool &rightMute) {
    memoryPlane.setRightMute(rightMute);
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
            float leftVisibility = m.getArgAsFloat(7);
            float rightVisibility = m.getArgAsFloat(8);
                        
            memoryPlane.setMemory(index, radius, theta, arcDistance, thickness, leftVisibility, rightVisibility, noiseSpeed, octaveMultiplier);
        }
    }
}
