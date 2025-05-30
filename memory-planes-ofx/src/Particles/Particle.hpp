//
//  Particle.hpp
//  fluidSimulation
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Neighbor.hpp"

class Particle {
public:
    Particle(ofVec3f position, int index);

    // member variables for fluid calculations
    float density, nearDensity;
    
    // gui parameters
    float lineThickness, lineLength;
    float minVelocity, maxVelocity, velocityCurve;
    float minSize, maxSize, size;
    float targetMinSize, targetMaxSize;

    // drawing variables
    float magnitude, lerpedMagnitude;
    float theta, lerpedTheta;
    float xOffset, yOffset, zOffset;
    int circleResolution, rectangleResolution;
    float connectionThickness;
    int index;
    
    ofVec3f position, velocity, predictedPosition;
    ofVec3f acceleration;
    ofColor particleColor, coolColor, hotColor;
    
    ofMesh circleMesh, rectangleMesh, vectorMesh, lineMesh;
    ofMesh normalizedCircleMesh, normalizedRectangleMesh;
    ofMesh normalizedLineMesh, normalizedVectorMesh;
    
    // drawn mesh
    ofMesh mesh;
    ofMesh connectionMesh;
    ofMesh flushMesh;
    
    enum shapeModes { CIRCLE, RECTANGLE, VECTOR, LINE } shapeMode;

    vector <int> indicesWithinInfluenceRadius;
    vector <int> indicesWithinConnectionRadius;
    vector <Neighbor> neighbors;
    
    ofMesh getShapeMesh();
    
    void setMode(int mode);
    void setRadius(float radius);
    void setVertices();
    void setSizes();
    void setSmoothedVelocity();
    void setRectangleOffsets();
    void setVectorOffsets();
    void setLineOffsets();
    void setLastFrameTime(float lastFrameTime);

    void initializeCircleMeshes();
    void initializeRectangleMeshes();
    void initializeVectorMeshes();
    void initializeLineMeshes();

    void updateCircleMesh();
    void updateRectangleMesh();
    void updateVectorMesh();
    void updateLineMesh();
    
    void update();
    void updateNeighbor(int index, ofVec2f position);
    void updateNeighbors();
    
    void draw();
    void drawConnections();
    
    bool isAlive, isDying;
    float lifetime, totalLifetime;
    float lastFrameTime;
private:
};

#endif /* Particle_hpp */
