//
//  Tear.cpp
//  memoryPlanes
//

#include "Tear.hpp"

Tear::Tear() {};

Tear::Tear(int _numAnchors) {
    primaryColor = ofColor::white;
    numAnchors = _numAnchors;
    velocity = 0.0;
    totalTime = 0.0;

    for (int i = 0; i < numAnchors; i++) {
        Anchor anchor = Anchor(numAnchors);
        anchors.push_back(anchor);
    }
}

void Tear::update() {
    float initialTheta = theta - arcDistance / 2.0;
    float splineDistance = arcDistance / anchors.size();
    totalTime += ofGetLastFrameTime() * noiseSpeed;
        
    for (int i = 0; i < anchors.size(); i++) {
        float anchorTheta = initialTheta + splineDistance * (i + 1);
        float anchorDeviation = sin(ofMap(i, -1, numAnchors - 1, -HALF_PI, TWO_PI - HALF_PI));
        anchorDeviation = ofMap(anchorDeviation, -1.0, 1.0, 0.0, 1.0);
        
        anchors[i].setTheta(anchorTheta);
        anchors[i].setDeviation(anchorDeviation);
        anchors[i].setThickness(thickness);
    }
    
    initialPoint.set(1.0, 0.0);
    initialPoint.rotateRad(initialTheta);
    initialPoint *= radius;
    
    initialHeading.set(1.0, 0.0);
    initialHeading.rotateRad(initialTheta + HALF_PI);
    initialHeading *= velocity;
    
    tearPolyline.clear();
    tearPolyline.addVertex(initialPoint.x, initialPoint.y);
    ofVec2f controlPoint1 = initialPoint + initialHeading;
            
    for (int i = 0; i < anchors.size(); i++) {
        anchors[i].setDirection(1);
        anchors[i].setNoiseTime(totalTime);
        anchors[i].setOctaveMultiplier(octaveMultiplier);
        anchors[i].update();
        
        ofVec2f nextPoint = anchors[i].getPoint();
        ofVec2f nextHeading = anchors[i].getHeading();
                        
        ofVec2f controlPoint2 = nextPoint - nextHeading;
        tearPolyline.bezierTo(controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y);
        
        controlPoint1 = nextPoint + nextHeading;
    }
    
    ofVec2f endingPoint = anchors[numAnchors - 1].getPoint();
    ofVec2f endingHeading = anchors[numAnchors - 1].getHeading();
    
    controlPoint1 = endingPoint - endingHeading;
    
    for (int i = anchors.size() - 1; i >= 0; i--) {
        anchors[i].setDirection(-1);
        anchors[i].update();

        ofVec2f nextPoint = anchors[i].getPoint();
        ofVec2f nextHeading = anchors[i].getHeading();
    
        ofVec2f controlPoint2 = nextPoint - nextHeading;
        
        tearPolyline.bezierTo(controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y);
    
        controlPoint1 = nextPoint + nextHeading;
    }
    
    ofVec2f controlPoint2 = initialPoint - initialHeading * 1.0;
    tearPolyline.bezierTo(controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, initialPoint.x, initialPoint.y);
    tearPolyline.close();
    
    tearPolyline.simplify();
}

void Tear::drawShape(ofPolyline polygon) {
    ofFill();
    ofSetColor(primaryColor);
    ofBeginShape();
    
    auto vertices = polygon.getVertices();
    for( int i = 0; i < vertices.size(); i++) {
        ofVertex(vertices[i].x, vertices[i].y);
    }
    ofEndShape();
}

void Tear::draw() {
    if (fillState) {
        drawShape(tearPolyline);
    } else {
        tearPolyline.draw();
    }
}

void Tear::setRadius(float _radius) {
    radius = _radius;
    
    for (int i = 0; i < anchors.size(); i++) {
        anchors[i].setRadius(radius);
    }
}

void Tear::setDistance(float _distance) {
    distance = _distance;
}

void Tear::setTheta(float _theta) {
    theta = _theta;
}

void Tear::setArcDistance(float _arcDistance) {
    arcDistance = _arcDistance;
    float initialTheta = theta - arcDistance / 2.0;
    float splineDistance = arcDistance / numAnchors;

    for (int i = 0; i < numAnchors; i++) {
        float anchorTheta = initialTheta + splineDistance * (i + 1);
        anchors[i].setTheta(anchorTheta);
    }
}

void Tear::setThickness(float _thickness) {
    thickness = _thickness;
}

void Tear::setNoiseTime(float _noiseTime) {
    noiseTime = _noiseTime;
}

void Tear::setFill(bool _fillState) {
    fillState = _fillState;
}

void Tear::setVisibility(float _visibility) {
    visibility = _visibility;
}

void Tear::setNoiseSpeed(float _noiseSpeed) {
    noiseSpeed = _noiseSpeed;
}

void Tear::setOctaveMultiplier(float _octaveMultiplier) {
    octaveMultiplier = _octaveMultiplier;
}
