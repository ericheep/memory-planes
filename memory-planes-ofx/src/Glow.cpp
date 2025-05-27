//
//  Glow.cpp
//  memory-planes-ofx
//

#include "Glow.hpp"

Glow::Glow() {
}

Glow::Glow(float _width, float _height) {
    width = _width;
    height = _height;
    
    attractionSystem.setWidth(width);
    attractionSystem.setHeight(height);
    
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
    attractionSystem.resetRandom();
    attractionSystem.setMode(0);
}

void Glow::draw() {
    attractionSystem.draw();
    attractionSystem.drawAttractors();
}

void Glow::update() {
    attractionSystem.updateAttractionSystem();
    attractionSystem.updateParticleSystem();
}

void Glow::setPosition(ofVec2f _position) {
    position = _position;
    attractionSystem.setAttractorPosition(0, position.x, position.y);
    attractionSystem.setAttractorPosition(1, position.x + 0.25, position.y + 0.25);
    
    ofVec2f xBounds = ofVec2f(-width, width);
    ofVec2f yBounds = ofVec2f(-height, height);
    attractionSystem.setBounds(xBounds, yBounds);
}
