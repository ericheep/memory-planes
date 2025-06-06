#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    
    if (DEV_MODE) {
        width = 800;
        height = 600;
        
        innerWidth = 300;
        innerHeight = 300;
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
    oscReceiver.setup(RECEIVING_PORT);
    oscSender.setup("127.0.0.1", SENDING_PORT);
    
    // load shaders
    innerBlur.load("shaders/blur");
    outerBlur.load("shaders/blur");
    overallBlur.load("shaders/blur");
    innerNoise.load("shaders/noise");
    outerNoise.load("shaders/noise");
    overallNoise.load("shaders/noise");
    innerRadialNoise.load("shaders/radialNoise");
    outerRadialNoise.load("shaders/radialNoise");
    overallRadialNoise.load("shaders/radialNoise");
    
    ofFbo::Settings innerSettings;
    innerSettings.width = innerWidth;
    innerSettings.height = innerHeight;
    innerSettings.internalformat = GL_RGBA;
    innerSettings.useDepth = false;
    innerSettings.useStencil = false;
    innerSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    innerSettings.numSamples = 0;
    
    ofFbo::Settings outerSettings;
    outerSettings.width = width;
    outerSettings.height = height;
    outerSettings.internalformat = GL_RGBA;
    outerSettings.useDepth = false;
    outerSettings.useStencil = false;
    outerSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    outerSettings.numSamples = 0;
    
    fboInnerWindow.allocate(innerSettings);
    fboInnerBlur.allocate(innerSettings);
    fboInnerNoise.allocate(innerSettings);
    
    fboOuterWindow.allocate(outerSettings);
    fboOuterBlur.allocate(outerSettings);
    fboOuterNoise.allocate(outerSettings);
    
    fboOverallWindow.allocate(outerSettings);
    fboOverallBlur.allocate(outerSettings);
    fboOverallNoise.allocate(outerSettings);
    
    // initialize colors
    backgroundColor = ofColor::black;
    primaryColor = ofColor::white;
    
    // plane
    memoryPlane = MemoryPlane(innerWidth, innerHeight);
    
    starField.setInnerSize(innerWidth, innerHeight);
    starField.setSize(width, height);
    
    emanations = Emanations(width, height);
    
    setupWarpers();
    setupGui();
    innerNoiseTime = 0.0f;
    outerNoiseTime = 0.0f;
    overallBlurRadius = 1.0f;
    
    state = idle;
}

void ofApp::setupWarpers() {
    setupWarper(innerWarper, innerWidth, innerHeight);
    if (!DEV_MODE) loadWarp("innerWarper.xml", innerWarper);
    innerWarper.hide();
    innerWarper.disableKeyboardShortcuts();
    
    setupWarper(outerWarper, width, height);
    if (!DEV_MODE) loadWarp("outerWarper.xml", outerWarper);
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
    
    gui.add(scale.set("inner scale", 1.0, 0.0, 3.0));
    gui.add(innerBlurAmount.set("inner blur", 1.0, 0.0, 1.0));
    gui.add(innerNoiseAmount.set("inner noise", 0.0, 0.0, 1.0));
    gui.add(innerRadialNoiseAmount.set("inner radial noise", 0.0, 0.0, 1.0));
    
    gui.add(outerBlurAmount.set("outer blur", 1.0, 0.0, 1.0));
    gui.add(outerNoiseAmount.set("outer noise", 0.0, 0.0, 1.0));
    gui.add(outerRadialNoiseAmount.set("outer radial noise", 0.0, 0.0, 1.0));
    
    // gui.add(overallBlurAmount.set("overall blur", 1.0, 0.0, 1.0));
    
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
    
    boundarySettings.setName("boundary settings");
    boundarySettings.add(leftBoundsScale.set("left bound", -1.0, -2.0, -0.5));
    boundarySettings.add(rightBoundsScale.set("right bound", 1.0, 0.5, 2.0));
    boundarySettings.add(backBoundsScale.set("back bound", 0.0, -1.0, 0.5));
    boundarySettings.add(frontBoundsScale.set("front bound", 1.0, 0.5, 2.0));
    gui.add(boundarySettings);
    
    leftBoundsScale.addListener(this, &ofApp::setLeftBoundsScale);
    rightBoundsScale.addListener(this, &ofApp::setRightBoundsScale);
    backBoundsScale.addListener(this, &ofApp::setBackBoundsScale);
    frontBoundsScale.addListener(this, &ofApp::setFrontBoundsScale);
    
    emanationSettings.setName("emanation settings");
    emanationSettings.add(emanations.minSize.set("attractor min size", 5.0, 0.0, 10.0));
    emanationSettings.add(emanations.maxSize.set("attractor max size", 10.0, 5.0, 20.0));
    emanationSettings.add(emanations.minVelocity.set("attractor min vel", 1.0, 0.0, 10.0));
    emanationSettings.add(emanations.maxVelocity.set("attractor max vel", 10.0, 5.0, 20.0));
    emanationSettings.add(emanations.connectionRadius.set("attractor connection radius", 50, 0, 200.0));
    emanationSettings.add(emanations.attractorY.set("attractor y", 0, -height / 2.0, height / 2.0));
    gui.add(emanationSettings);
    
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
    updateState();
    memoryPlane.update();
    
	starField.setWarp(innerWarper, outerWarper);
    starField.update();
    
    ofPoint* points = innerWarper.getTargetPoints();
    
    ofPoint centroid(0, 0);
    for (int i = 0; i < 4; i++) {
        centroid += points[i];
    }
    centroid /= 4.0;
    centroid.x -= width / 2.0;
    centroid.y -= height / 2.0;
    
    emanations.setCenter(centroid);
    emanations.update();
    
    updateInnerFBO();
    updateOuterFBO();
    updateOverallFBO();
    
    innerNoiseTime += innerNoiseAmount * 0.06;
    outerNoiseTime += outerNoiseAmount * 0.06;
    overallBlurRadius = ofMap(ofNoise(ofGetElapsedTimef()) * 3.1f, 0.0f, 1.0f, 0.85f, 1.2f);
    
    ofxOscMessage msg;
    msg.setAddress("/fps");
    msg.addFloatArg(ofGetFrameRate());
    oscSender.sendMessage(msg);
    
}

//--------------------------------------------------------------
void ofApp::updateState() {
    switch(state) {
        case idle:
            emanations.isAttracting = false;
            emanations.isConverging = false;
            break;
        case firefly:
            emanations.isAttracting = true;
            emanations.isConverging = false;
            break;
        case converge:
            emanations.isConverging = true;
            // code block
            break;
        case memory:
            emanations.isAttracting = false;
            emanations.isConverging = false;

            break;
    }
}

void ofApp::drawFps() {
    std::stringstream strm;
    strm << std::setprecision(3) << "fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
}

void ofApp::draw() {
    ofBackground(backgroundColor);
    
    ofEnableAlphaBlending();
    
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
    
    ofPushMatrix();
    ofMultMatrix(outerMatrix);
    ofSetColor(ofColor::white);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fboOverallBlur.draw(0, 0);
    ofDisableBlendMode();
    ofPopMatrix();
    
    ofDisableAlphaBlending();
    
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
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(innerWidth / 2.0, innerHeight / 2.0, 0);
    ofScale(scale);
    memoryPlane.draw();
    ofPopMatrix();
    fboInnerWindow.end();
    
    // noise fbo
    fboInnerNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    innerNoise.begin();
    innerNoise.setUniform1f("u_distortion", innerNoiseAmount);
    innerNoise.setUniform1f("u_time", innerNoiseTime);
    innerNoise.setUniform2f("u_resolution", innerWidth, innerHeight);
    innerRadialNoise.begin();
    innerRadialNoise.setUniform1f("u_distortion", innerRadialNoiseAmount);
    innerRadialNoise.setUniform1f("u_time", innerNoiseTime);
    innerRadialNoise.setUniform2f("u_resolution", innerWidth, innerHeight);
    fboInnerWindow.draw(0, 0);
    innerRadialNoise.end();
    innerNoise.end();
    fboInnerNoise.end();
    
    // blur fbo
    fboInnerBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    innerBlur.begin();
    innerBlur.setUniform1f("u_blurMix", innerBlurAmount);
    fboInnerNoise.draw(0, 0);
    innerBlur.end();
	memoryPlane.draw();
    fboInnerBlur.end();
}

void ofApp::updateOuterFBO() {
    // outer fbo
    fboOuterWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    starField.draw();
    ofPopMatrix();
    fboOuterWindow.end();
    
    // noise fbo
    fboOuterNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    outerNoise.begin();
    outerNoise.setUniform1f("u_distortion", outerNoiseAmount);
    outerNoise.setUniform1f("u_time", outerNoiseTime);
    outerNoise.setUniform2f("u_resolution", width, height);
    outerRadialNoise.begin();
    outerRadialNoise.setUniform1f("u_distortion", outerRadialNoiseAmount);
    outerRadialNoise.setUniform1f("u_time", outerNoiseTime);
    outerRadialNoise.setUniform2f("u_resolution", width, height);
    fboOuterWindow.draw(0, 0);
    outerRadialNoise.end();
    outerNoise.end();
    fboOuterNoise.end();
    
    // blur fbo
    fboOuterBlur.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255);
    outerBlur.begin();
    outerBlur.setUniform1f("u_blurMix", outerBlurAmount);
    fboOuterNoise.draw(0, 0);
    outerBlur.end();
	starField.draw();

    fboOuterBlur.end();
}

void ofApp::updateOverallFBO() {
    /* // overall fbo
    fboOverallWindow.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255, 128);
    ofPushMatrix();
    emanations.draw();
    ofPopMatrix();
    fboOverallWindow.end();
    
    // noise fbo
    fboOverallNoise.begin();
    ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255, 128);
    overallNoise.begin();
    overallNoise.setUniform1f("u_distortion", outerNoiseAmount);
    overallNoise.setUniform1f("u_time", outerNoiseTime);
    overallNoise.setUniform2f("u_resolution", width, height);
    overallRadialNoise.begin();
    overallRadialNoise.setUniform1f("u_distortion", outerRadialNoiseAmount);
    overallRadialNoise.setUniform1f("u_time", outerNoiseTime);
    overallRadialNoise.setUniform2f("u_resolution", width, height);
    fboOverallWindow.draw(0, 0);
    overallRadialNoise.end();
    overallNoise.end();
    fboOverallNoise.end(); */
    
    // blur fbo
    //fboOverallBlur.begin();
    /*ofClear(0.0f, 0.0f, 0.0f, 0.0f);
    ofSetColor(255, 255, 255, 128);
    overallBlur.begin();
    overallBlur.setUniform1f("u_blurMix", overallBlurAmount);
    overallBlur.setUniform1f("u_blurRadiusScalar", overallBlurRadius);
    fboOverallNoise.draw(0, 0);
    overallBlur.end();*/
	//emanations.draw();
    //fboOverallBlur.end();
}

void ofApp::setLeftBoundsScale(float &leftBoundsScale) {
    emanations.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
    starField.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
}

void ofApp::setRightBoundsScale(float &rightBoundsScale) {
    emanations.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
    starField.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
}

void ofApp::setBackBoundsScale(float &backBoundsScale) {
    emanations.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
    starField.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
}

void ofApp::setFrontBoundsScale(float &frontBoundsScale) {
    emanations.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
    starField.setSkew(leftBoundsScale, rightBoundsScale, backBoundsScale, frontBoundsScale);
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

void ofApp::updateOsc() {
    while(oscReceiver.hasWaitingMessages()) {
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);
        
        if (m.getAddress() == "/memory") {
            int index = m.getArgAsInt(0);
            float radius = m.getArgAsFloat(1);
            float theta = m.getArgAsFloat(2);
            float arcDistance = m.getArgAsFloat(3);
            float thickness = m.getArgAsFloat(4);
            float minFollow = m.getArgAsFloat(5);
            float maxFollow = m.getArgAsFloat(6);
            float noiseSpeed = m.getArgAsFloat(7);
            float octaveMultiplier = m.getArgAsFloat(8);
            
            memoryPlane.setMemory(index, radius, theta, arcDistance, thickness, minFollow, maxFollow, noiseSpeed, octaveMultiplier);
        }
        
        if (m.getAddress() == "/state") {
            string stateString = m.getArgAsString(0);
            
            if (stateString == "idle") {
                state = idle;
            } else if (stateString == "firefly") {
                state = firefly;
            } else if (stateString == "converge") {
                state = converge;
            } else if (stateString == "memory") {
                state = memory;
            }
        }
        
        if (m.getAddress() == "/presence") {
            int index = m.getArgAsInt(0);
            float x = m.getArgAsFloat(1);
            float y = m.getArgAsFloat(2);
            float width = m.getArgAsFloat(3);
            starField.setPresence(index, x, y, width);
            emanations.setEmanation(index, x, y, width);
        }
        
        if (m.getAddress() == "/numberParticles") {
            starField.numberParticles = m.getArgAsInt(0);
        }
        
        if (m.getAddress() == "/timeScalar") {
            starField.timeScalar = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/influenceRadius") {
            starField.influenceRadius = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/gravityMultiplier") {
            starField.gravityMultiplier = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/minSize") {
            starField.minSize = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/maxSize") {
            starField.maxSize = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/connectionRadius") {
            starField.connectionRadius = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/gravityTheta") {
            starField.gravityTheta = m.getArgAsFloat(0);
        }
        
        if (m.getAddress() == "/flip") {
            int index = m.getArgAsInt(0);
            float theta = m.getArgAsFloat(1);
            memoryPlane.flip(index, theta);
        }
        
        if (m.getAddress() == "/innerRadialNoise") {
            innerNoiseAmount.set(m.getArgAsFloat(0));
            innerRadialNoiseAmount.set(m.getArgAsFloat(1));
        }
    }
}

void ofApp::onMouseDragged(ofMouseEventArgs& mouseArgs) {
    starField.setWarp(innerWarper, outerWarper);

}

void ofApp::onMouseReleased(ofMouseEventArgs& mouseArgs) {
    starField.setWarp(innerWarper, outerWarper);
}
