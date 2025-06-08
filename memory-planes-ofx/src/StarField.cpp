//
//  StarField.cpp
//

#include "StarField.hpp"

StarField::StarField() {
    numberParticles.addListener(this, &StarField::setNumberParticles);
    influenceRadius.addListener(this, &StarField::setInfluenceRadius);
    timeScalar.addListener(this, &StarField::setTimeScalar);
    gravityMultiplier.addListener(this, &StarField::setGravityMultiplier);
    connectionRadius.addListener(this, &StarField::setConnectionRadius);
    drawMode.addListener(this, &StarField::setDrawMode);
    velocityCurve.addListener(this, &StarField::setVelocityCurve);
    minVelocity.addListener(this, &StarField::setMinVelocity);
    maxVelocity.addListener(this, &StarField::setMaxVelocity);
    minSize.addListener(this, &StarField::setMinSize);
    maxSize.addListener(this, &StarField::setMaxSize);
    targetDensity.addListener(this, &StarField::setTargetDensity);
    pressureMultiplier.addListener(this, &StarField::setPressureMultiplier);
    nearPressureMultiplier.addListener(this, &StarField::setNearPressureMultiplier);
    gravityTheta.addListener(this, &StarField::setGravityTheta);
    
    // simulation settings
    fluidSystem.setCenter(width * 0.5, height * 0.5);
    fluidSystem.setCollisionDamping(0.5);
    fluidSystem.setViscosityStrength(0.25);
    fluidSystem.setMode(0);
    fluidSystem.setNumberParticles(numberParticles);
    fluidSystem.setHotColor(ofColor::white);
    fluidSystem.setCoolColor(ofColor::white);
}

void StarField::update() {
    float lastFrameTime = ofGetLastFrameTime();
    
    for (auto &presence : presences) {
        presence.setLastFrameTime(lastFrameTime);
        presence.update();
    }

    presences.erase(std::remove_if(presences.begin(), presences.end(), [](Presence& presence) {
        return !presence.isAlive;
    }), presences.end());

    fluidSystem.updatePresences(presences);
    fluidSystem.updateFluidSystem();
    fluidSystem.updateParticleSystem();
}

void StarField::draw() {
    ofSetColor(ofColor::thistle);
    for (auto &presence : presences) {
        ofRectangle rect;
        
        rect.setFromCenter(presence.x, presence.y, 30, 30);
        ofDrawRectangle(rect);
    }
    
    fluidSystem.draw();
    innerBounds.draw();
}

void StarField::setSkew(float _leftBoundsScale, float _rightBoundsScale, float _backBoundsScale, float _frontBoundsScale) {
    leftBoundsScale = _leftBoundsScale;
    rightBoundsScale = _rightBoundsScale;
    backBoundsScale = _backBoundsScale;
    frontBoundsScale = _frontBoundsScale;
}

void StarField::setInnerSize(float _innerWidth, float _innerHeight) {
    innerWidth = _innerWidth;
    innerHeight = _innerHeight;
}

void StarField::setPresence(int index, float x, float y, float blobWidth) {
    y = ofMap(y, 0.0, 1.0, backBoundsScale, frontBoundsScale);
    
    float xScalar = cos(atan2(y, x));
    xScalar = ofMap(xScalar, -1.0, 1.0, leftBoundsScale, rightBoundsScale);

    x = xScalar * width + width / 2.0;
    
    int vectorIndex = getPresenceVectorIndex(index);

    if (vectorIndex >= 0) {
        presences[vectorIndex].setPresence(x, height * 0.75, blobWidth);
    } else {
        addPresence(index, x, height * 0.75, blobWidth);
    }
}

void StarField::addPresence(int index, float x, float y, float blobWidth) {
    Presence presence;
    presence.index = index;
    presence.x = x;
    presence.y = y;
    presence.blobWidth = blobWidth;

    presences.push_back(presence);
}

int StarField::getPresenceVectorIndex(int index) {
    for (int i = 0; i < presences.size(); i++) {
        if (presences[i].index == index) return i;
    }
    
    return -1;
}

void StarField::setSize(float _width, float _height) {
    width = _width;
    height = _height;
    fluidSystem.setWidth(width);
    fluidSystem.setHeight(height);
    fluidSystem.setBoundsSize(ofVec3f(width, height, 0));
}

void StarField::setWarp(ofxQuadWarp & innerWarper, ofxQuadWarp & outerWarper) {
    // transformation from inner space to outer space
    ofMatrix4x4 innerMatrix = innerWarper.getMatrix();
    ofMatrix4x4 outerMatrix = outerWarper.getMatrix();
    ofMatrix4x4 relativeMatrix = outerMatrix.getInverse() * innerMatrix;
    
    // define the corners of the inner FBO (before any warp)
    std::vector<ofVec3f> corners = {
        {0, 0},
        {innerWidth, 0},
        {innerWidth, innerHeight},
        {0, innerHeight},
    };
    
    // transform them into the outer FBO's warped space
    innerBounds.clear();
    for (auto& corner : corners) {
        ofVec3f warped = corner * relativeMatrix;
        innerBounds.addVertex(warped);
    }

    innerBounds.close();
    fluidSystem.setInnerBoundarySpace(innerBounds);
}

void StarField::setDrawMode(int & drawMode) {
    // 0 = circles
    // 1 = rectangles
    // 2 = vectors
    // 3 = lines
    // 4 = points
    
    fluidSystem.setMode(drawMode);
}

void StarField::setNumberParticles(int & numberParticles) {
    fluidSystem.setNumberParticles(numberParticles);
}

void StarField::setTimeScalar(float & timeScalar) {
    fluidSystem.setDeltaTime(1.0 / 60.0 / timeScalar);
}

void StarField::setInfluenceRadius(float & influenceRadius) {
    fluidSystem.setInfluenceRadius(influenceRadius);
}

void StarField::setGravityMultiplier(float & gravityMultiplier) {
    fluidSystem.setGravityMultiplier(gravityMultiplier);
}

void StarField::setTargetDensity(float & targetDensity) {
    fluidSystem.setTargetDensity(targetDensity);
}

void StarField::setPressureMultiplier(float & pressureMultiplier) {
    fluidSystem.setPressureMultiplier(pressureMultiplier);
}

void StarField::setNearPressureMultiplier(float & nearPressureMultiplier) {
    fluidSystem.setNearPressureMultiplier(nearPressureMultiplier);
}

void StarField::setVelocityCurve(float & velocityCurve) {
    fluidSystem.setVelocityCurve(velocityCurve);
}

void StarField::setMinVelocity(float & minVelocity) {
    fluidSystem.setMinVelocity(minVelocity);
}

void StarField::setMaxVelocity(float & maxVelocity) {
    fluidSystem.setMaxVelocity(maxVelocity);
}

void StarField::setMinSize(float& minSize) {
    fluidSystem.setMinSize(minSize);
}

void StarField::setMaxSize(float& maxSize) {
    fluidSystem.setMaxSize(maxSize);
}

void StarField::setConnectionRadius(float& connectionRadius) {
    fluidSystem.setConnectionRadius(connectionRadius);
}

void StarField::setGravityTheta(float& gravityTheta) {
    fluidSystem.setGravityTheta(gravityTheta);
}

void StarField::setBoundaryState(int _boundaryState) {
    fluidSystem.boundaryState = _boundaryState;
}
