//
//  Emanations.cpp
//  memory-planes-ofx
//

#include "Emanations.hpp"

Emanations::Emanations() {
}

Emanations::Emanations(float _width, float _height) {
    width = _width;
    height = _height;
    depth = width;
    rectangleWidth = 20;

    position = ofVec3f(0, 0, 0);
    
    attractionSystem = AttractionSystem(width, height);
    
    ofVec2f xBounds = ofVec2f(-width / 2.0, width / 2.0);
    ofVec2f yBounds = ofVec2f(-height / 2.0, height / 2.0);
    attractionSystem.setBounds(xBounds * 0.25, yBounds * 0.25);
    
    minSize = 5.0;
    maxSize = 10.0;
    minVelocity = 1.0;
    maxVelocity = 10.0;
    velocityCurve = 1.0;
    
    attractionSystem.setNumberParticles(0);
    attractionSystem.setConnectionRadius(50);
    attractionSystem.setHotColor(ofColor::white);
    attractionSystem.setCoolColor(ofColor::white);
    attractionSystem.setVelocityCurve(velocityCurve);
    attractionSystem.setMinSize(minSize);
    attractionSystem.setMaxSize(maxSize);
    attractionSystem.setMinVelocity(minVelocity);
    attractionSystem.setMaxVelocity(maxVelocity);
    attractionSystem.setMode(0);
    
    isConverging = false;
    isAttracting = true;
}

void Emanations::setCenter(ofVec2f center) {
    attractionSystem.centerAttractor.position = center;
}

void Emanations::update() {
    attractionSystem.updateAttractionSystem();
    attractionSystem.updateParticleSystem();
    attractionSystem.isConverging = isConverging;
    attractionSystem.isAttracting = isAttracting;
    
    for (auto &glow : glows) {
        glow.draw();
    }
}

void Emanations::setAttractorY(float &attractorY) {
    for (auto &attractors : attractionSystem.attractors) {
        attractors.position.y = attractorY;
    }
}

void Emanations::setSkew(float _leftBoundsScale, float _rightBoundsScale, float _backBoundsScale, float _frontBoundsScale) {
    leftBoundsScale = _leftBoundsScale;
    rightBoundsScale = _rightBoundsScale;
    backBoundsScale = _backBoundsScale;
    frontBoundsScale = _frontBoundsScale;
}

void Emanations::setEmanation(int index, float x, float y, float blobWidth) {
    y = ofMap(y, 0.0, 1.0, backBoundsScale, frontBoundsScale);
    
    float xScalar = cos(atan2(y, x));
    xScalar = ofMap(xScalar, -1.0, 1.0, leftBoundsScale, rightBoundsScale);
    
    ofVec2f position = ofVec3f(xScalar * width, attractorY);
    
    int vectorIndex = attractionSystem.getAttractorVectorIndex(index);
    if (vectorIndex >= 0) {
        attractionSystem.setAttractorPosition(vectorIndex, position);
    } else {
        attractionSystem.addAttractor(index, position);
    }
}

void Emanations::draw() {
    ofPushMatrix();
    ofTranslate(width / 2.0, height / 2.0);
    
    // attractionSystem.drawAttractors();
    attractionSystem.draw();
    
    for (auto &glow : glows) {
        glow.draw();
    }

    ofPopMatrix();
}
