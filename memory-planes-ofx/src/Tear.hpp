//
//  Tear.hpp
//  memoryPlanes
//

#ifndef Tear_hpp
#define Tear_hpp

#include "Anchor.hpp"
#include <stdio.h>
#include <ofMain.h>

class Tear {
public:
    Tear();
    Tear(int numAnchors, int width, int height);
    
    void setRadius(float radius);
    void setTheta(float theta);
    void setArcDistance(float arcDistance);
    void setThickness(float thickness);
    void setNoiseTime(float noiseTime);
    void setFill(bool fillState);
    void setVisibility(float visibility);
    void setOctaveMultiplier(float octaveMultiplier);
    void setFollow(float minFollow, float maxFollow);
    
    void flip();
    void interpolatePoints();
    
    void update();
    void draw();
    void drawShape(ofPolyline polygon);
    
private:
    ofVec2f initialPoint, endPoint;
    ofVec2f initialHeading, endHeading;
    
    ofPolyline tearPolyline;
    
    int numAnchors, width, height;
    float radius, theta, arcDistance, velocity, thickness, totalTime;
    float time, distance, noiseTime, visibility, octaveMultiplier, scaledRadius;
    bool isFilled, isFlipped;

    ofColor primaryColor;
    vector <Anchor> anchors;
    vector <ofVec2f> currentPoints, targetPoints;
    vector <float> targetValues;
};

#endif /* Tear_hpp */
