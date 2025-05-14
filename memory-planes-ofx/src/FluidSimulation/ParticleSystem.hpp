//
//  ParticleSystem.hpp
//  fluidSimulation
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <stdio.h>
#include "Particle.hpp"
#include "Kernels.hpp"
#include "ofMain.h"
#include "tbb/parallel_for.h"

class ParticleSystem {
public:
    ParticleSystem();
    
    vector<Particle> particles;

    float radius, gravityConstant, deltaTime, collisionDamping, predictionFactor, interactiveGravity;
    float targetDensity, nearPressureMultiplier, pressureMultiplier, gravityMultiplier, timeScalar, viscosityStrength, connectionRadius;
    int mouseButton, mouseRadius;

    enum drawModes { CIRCLES, RECTANGLES, VECTORS, LINES, POINTS } drawMode;
    int circleResolution, rectangleResolution, shapeResolution, drawModeInt;;
    
    ofMesh mesh;
    vector<ofVec3f> meshVertices;
    vector<ofIndexType> meshIndices;
    vector<pair<ofVec2f, ofVec2f>> connections;
    vector<pair<ofColor, ofColor>> connectionColors;
    
    vector <ofColor> colors;
    ofMesh shapeMesh;
    
    float centerX, centerY;
    ofVec2f gravityForce;
    float mouseForce;
    ofVec2f center;
    ofVec2f xBounds, yBounds, zBounds, mousePosition;
    ofVec3f boundsSize;
    ofVec3f bounds;
    Kernels kernels;
    ofPolyline innerPolyline;
    Boolean mouseInputActive, pauseActive, nextFrameActive, exportFrameActive;
    
    float circleBoundaryRadius;
    Boolean circleBoundaryActive;

    vector<pair<int, unsigned int>> spatialLookup;
    vector<int> startIndices;
    
    vector<pair<int, unsigned int>> connectionsSpatialLookup;
    vector<int> connectionsStartIndices;
    
    // setters
    void setDeltaTime(float deltaTime);
    void setRadius(float radius);
    void setGravityMultiplier(float gravityMultiplier);
    void setGravityRotation(ofVec2f gravityRotation);
    void setTargetDensity(float targetDensity);
    void setPressureMultiplier(float pressureMultiplier);
    void setNearPressureMultiplier(float nearPressureMultiplier);
    void setViscosityStrength(float viscosityStrength);
    void setCollisionDamping(float collisionDamping);
    void setBoundsSize(ofVec3f bounds);
    void setMouseRadius(int mouseRadius);
    void setMouseForce(float mouseForce);
    void setMinVelocity(float minVelocity);
    void setMaxVelocity(float maxVelocity);
    void setMinSize(float minSize);
    void setMaxSize(float maxSize);
    void setVelocityCurve(float velocityCurve);
    void setLineThickness(float lineThickness);
    void setVelocityHue(float hue);
    void setHotColor(ofColor hotColor);
    void setCoolColor(ofColor coolColor);
    void setNumberParticles(int number);
    void setLineCurve(float lineCurve);
    void setCenter(float centerX, float centerY);
    void setMode(int drawMode);
    void setCircleBoundary(Boolean circleBoundaryActive);
    void setWidth(int systemWidth);
    void setHeight(int systemHeight);
    void setConnectionRadius(float connectionRadius);
    void setInnerBoundarySpace(ofPolyline polyline);
    
    // creation functions
    void addParticle();
    void addParticle(ofVec3f position);
    void addParticle(ofVec3f position, float radius);
    ofVec2f getRandom2DDirection();
    ofVec3f getRandom3DDirection();
    
    // interactions
    void mouseInput(int x, int y);
    void mouseInput(int x, int y, int button, Boolean active);
    
    void draw();
    void updateMesh(int particleIndex);
    void updateTriangle(int particleIndex);
    void updateLine(int particleIndex);
    void updatePoint(int particleIndex);
    void initializeTrianglesMesh(int numParticles, int shapeResolution);
    void initializeQuadsMesh(int numParticles);
    void initializeLinesMesh(int numParticles);
    void initializePointsMesh(int numParticles);

    void pause(Boolean pauseButton);
    void nextFrame();
    void saveSvg();
    
    int systemWidth, systemHeight;
private:
};

#endif /* ParticleSystem_hpp */
