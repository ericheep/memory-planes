//
//  Tear.cpp
//  memoryPlanes
//

#include "Tear.hpp"

Tear::Tear() {};

Tear::Tear(int _numAnchors, int _width, int _height) {
    primaryColor = ofColor::white;
    numAnchors = _numAnchors;
    width = _width;
    height = _height;
    velocity = 0.0;
    totalTime = 0.0;
    scaledRadius = 0.0;

    for (int i = 0; i < numAnchors; i++) {
        Anchor anchor = Anchor(numAnchors);
        anchors.push_back(anchor);
    }
    
    for (int i = 0; i < numAnchors * 3 * 2 + 4; i++) {
        targetValues.push_back(ofRandom(0.03, 0.15));
        targetPoints.push_back(ofVec2f::zero());
        currentPoints.push_back(ofVec2f::zero());
    }
	    
    isFilled = true;
    isFlipped = false;
}

void Tear::update() {
    float initialTheta = theta - arcDistance / 2.0;
    float splineDistance = arcDistance / anchors.size();
    float scaledThickness = thickness * width / 5.0;
    int pointIndex = 0;
    
    for (int i = 0; i < anchors.size(); i++) {
        float anchorTheta = initialTheta + splineDistance * (i + 1);
        float anchorDeviation = sin(ofMap(i, -1, numAnchors - 1, -HALF_PI, TWO_PI - HALF_PI));
        anchorDeviation = ofMap(anchorDeviation, -1.0, 1.0, 0.0, 1.0);
        
        anchors[i].setTheta(anchorTheta);
        anchors[i].setDeviation(anchorDeviation);
        anchors[i].setThickness(scaledThickness);
    }
    
    initialPoint.set(1.0, 0.0);
    initialPoint.rotateRad(initialTheta);
    initialPoint *= scaledRadius;
    
    initialHeading.set(1.0, 0.0);
    initialHeading.rotateRad(initialTheta + HALF_PI);
    initialHeading *= velocity;
    
    targetPoints[pointIndex].set(initialPoint.x, initialPoint.y);
    pointIndex++;
    
    ofVec2f controlPoint1 = initialPoint + initialHeading;
    
    for (int i = 0; i < anchors.size(); i++) {
        anchors[i].setDirection(1);
        anchors[i].setNoiseTime(noiseTime);
        anchors[i].setOctaveMultiplier(octaveMultiplier);
        anchors[i].update();
        
        ofVec2f nextPoint = anchors[i].getPoint();
        ofVec2f nextHeading = anchors[i].getHeading();
        
        ofVec2f controlPoint2 = nextPoint - nextHeading;
        
        targetPoints[pointIndex].set(controlPoint1.x, controlPoint1.y);
        targetPoints[pointIndex + 1].set(controlPoint2.x, controlPoint2.y);
        targetPoints[pointIndex + 2].set(nextPoint.x, nextPoint.y);
        pointIndex += 3;
        
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
        
        targetPoints[pointIndex].set(controlPoint1.x, controlPoint1.y);
        targetPoints[pointIndex + 1].set(controlPoint2.x, controlPoint2.y);
        targetPoints[pointIndex + 2].set(nextPoint.x, nextPoint.y);
        pointIndex += 3;
        
        controlPoint1 = nextPoint + nextHeading;
    }
    
    ofVec2f controlPoint2 = initialPoint - initialHeading * 1.0;
    
    targetPoints[pointIndex].set(controlPoint1.x, controlPoint1.y);
    targetPoints[pointIndex + 1].set(controlPoint2.x, controlPoint2.y);
    targetPoints[pointIndex + 2].set(initialPoint.x, initialPoint.y);
    
    interpolatePoints();
}

void Tear::interpolatePoints() {
    int pointIndex = 0;
    
    for (int i = 0; i < currentPoints.size(); i++) {
        int index = i;
        if (isFlipped) index = (targetPoints.size() / 2 + i) % targetPoints.size();
        
        if (targetValues[i] < 1.0) {
            currentPoints[i].interpolate(targetPoints[index], targetValues[i]);
        } else {
            currentPoints[i] = targetPoints[index];
        }
    }
    
    tearPolyline.clear();
    tearPolyline.addVertex(currentPoints[pointIndex].x, currentPoints[pointIndex].y);
    pointIndex++;
    
    for (int i = 0; i < anchors.size(); i++) {
        tearPolyline.bezierTo(                              currentPoints[pointIndex].x, currentPoints[pointIndex].y, currentPoints[pointIndex + 1].x, currentPoints[pointIndex + 1].y, currentPoints[pointIndex + 2].x, currentPoints[pointIndex + 2].y
        );
        
        pointIndex += 3;
    }
    
    for (int i = anchors.size() - 1; i >= 0; i--) {
        tearPolyline.bezierTo(                              currentPoints[pointIndex].x, currentPoints[pointIndex].y, currentPoints[pointIndex + 1].x, currentPoints[pointIndex + 1].y, currentPoints[pointIndex + 2].x, currentPoints[pointIndex + 2].y
        );
        
        pointIndex += 3;
    }
    
    tearPolyline.addVertex(currentPoints[pointIndex].x, currentPoints[pointIndex].y);
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
    if (isFilled) {
        drawShape(tearPolyline);
    } else {
        tearPolyline.draw();
    }
}

void Tear::setRadius(float _radius) {
    radius = _radius;
    scaledRadius = radius * height / 3.0;
    
    for (int i = 0; i < anchors.size(); i++) {
        anchors[i].setRadius(scaledRadius);
    }
}

void Tear::flip() {
    isFlipped = !isFlipped;
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

void Tear::setFill(bool _isFilled) {
    isFilled = _isFilled;
}

void Tear::setVisibility(float _visibility) {
    visibility = _visibility;
}

void Tear::setOctaveMultiplier(float _octaveMultiplier) {
    octaveMultiplier = _octaveMultiplier;
}

void Tear::setFollow(float _minFollow, float _maxFollow) {
    for (int i = 0; i < numAnchors * 3 * 2 + 2; i++) {
        targetValues[i] = ofRandom(_minFollow, _maxFollow);
    }
}
