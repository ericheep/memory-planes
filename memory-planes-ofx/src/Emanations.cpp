//
//  Emanations.cpp
//  memory-planes-ofx
//

#include "Emanations.hpp"

Emanations::Emanations() {
    position = ofVec3f(0, 0, 0);
}

Emanations::Emanations(float _width, float _height) {
    width = _width;
    height = _height;
    depth = width;
    rectangleWidth = 20;
    glow = Glow(width, height);
}

void Emanations::update() {
    glow.setPosition(position);
    glow.update();
}

void Emanations::setEmanation(int index, float x, float y, float blobWidth) {
    position.x = x * width;
    position.y = 0;
    position.z = y * depth;
    
    // Create a quad as two triangles
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    float hw = rectangleWidth / 2.0;
    float hh = 10;
    
    ofVec3f v0 = position + ofVec3f(-hw, -hh, 0);
    ofVec3f v1 = position + ofVec3f( hw, -hh, 0);
    ofVec3f v2 = position + ofVec3f( hw,  hh, 0);
    ofVec3f v3 = position + ofVec3f(-hw,  hh, 0);
    
    mesh.addVertex(v0);
    mesh.addVertex(v1);
    mesh.addVertex(v2);
    mesh.addVertex(v3);
    
    mesh.addIndex(0);
    mesh.addIndex(1);
    mesh.addIndex(2);

    mesh.addIndex(0);
    mesh.addIndex(2);
    mesh.addIndex(3);
}

void Emanations::draw() {
    ofPushMatrix();
    ofTranslate(width / 2.0, height / 2.0);
    mesh.draw();
    glow.draw();
    ofPopMatrix();
}

