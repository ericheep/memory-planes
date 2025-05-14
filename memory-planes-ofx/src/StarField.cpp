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
    
    // simulation settings
    particleSystem.setCenter(width * 0.5, height * 0.5);
    particleSystem.setCollisionDamping(0.05);
    particleSystem.setViscosityStrength(0.25);
    particleSystem.setMode(0);
    particleSystem.setNumberParticles(numberParticles);
    particleSystem.setHotColor(ofColor::white);
    particleSystem.setCoolColor(ofColor::red);
    particleSystem.resetCircle(1.0);
}

void StarField::update() {
    particleSystem.update();
}

void StarField::draw() {
    particleSystem.draw();
    innerBounds.draw();
}

void StarField::setInnerSize(float _innerWidth, float _innerHeight) {
    innerWidth = _innerWidth;
    innerHeight = _innerHeight;
}

void StarField::setPresence(float _x, float _width) {
    particleSystem.setPresence(_x, _width);
}

void StarField::setSize(float _width, float _height) {
    width = _width;
    height = _height;
    particleSystem.setWidth(width);
    particleSystem.setHeight(height);
    particleSystem.setBoundsSize(ofVec3f(width, height, 0));
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
    particleSystem.setInnerBoundarySpace(innerBounds);
}

void StarField::setDrawMode(int & drawMode) {
    // 0 = circles
    // 1 = rectangles
    // 2 = vectors
    // 3 = lines
    // 4 = points
    
    particleSystem.setMode(drawMode);
}

void StarField::setNumberParticles(int & numberParticles) {
    particleSystem.setNumberParticles(numberParticles);
    particleSystem.setVelocityCurve(velocityCurve);
    particleSystem.setMinSize(minSize);
    particleSystem.setMaxSize(maxSize);
    particleSystem.setMinVelocity(minVelocity);
    particleSystem.setMaxVelocity(maxVelocity);
}

void StarField::setTimeScalar(float & timeScalar) {
    particleSystem.setDeltaTime(1.0 / 60.0 / timeScalar);
}

void StarField::setInfluenceRadius(float & influenceRadius) {
    particleSystem.setRadius(influenceRadius);
}

void StarField::setGravityMultiplier(float & gravityMultiplier) {
    particleSystem.setGravityMultiplier(gravityMultiplier);
}

void StarField::setTargetDensity(float & targetDensity) {
    particleSystem.setTargetDensity(targetDensity);
}

void StarField::setPressureMultiplier(float & pressureMultiplier) {
    particleSystem.setPressureMultiplier(pressureMultiplier);
}

void StarField::setNearPressureMultiplier(float & nearPressureMultiplier) {
    particleSystem.setNearPressureMultiplier(nearPressureMultiplier);
}

void StarField::setVelocityCurve(float & velocityCurve) {
    particleSystem.setVelocityCurve(velocityCurve);
}

void StarField::setMinVelocity(float & minVelocity) {
    particleSystem.setMinVelocity(minVelocity);
}

void StarField::setMaxVelocity(float & maxVelocity) {
    particleSystem.setMaxVelocity(maxVelocity);
}

void StarField::setMinSize(float & minSize) {
    particleSystem.setMinSize(minSize);
}

void StarField::setMaxSize(float & maxSize) {
    particleSystem.setMaxSize(maxSize);
}

void StarField::setConnectionRadius(float & connectionRadius) {
    particleSystem.setConnectionRadius(connectionRadius);
}
