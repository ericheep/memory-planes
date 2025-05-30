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
#include "tbb/parallel_sort.h"

class ParticleSystem {
public:
    ParticleSystem();
    
    vector<Particle> particles;

    enum drawModes { CIRCLES, RECTANGLES, VECTORS, LINES, POINTS } drawMode;
    int circleResolution, rectangleResolution, shapeResolution, drawModeInt;
    
    ofMesh mesh;
    ofMesh connectionMesh;
    
    vector<ofVec3f> meshVertices;
    vector<ofIndexType> meshIndices;
    
    vector <ofColor> colors;
    ofMesh shapeMesh;
    
    float centerX, centerY;
    ofVec2f gravityForce;
    ofVec2f center;
    ofVec2f xBounds, yBounds;
    ofVec3f boundsSize;
    ofVec3f bounds;
    float connectionRadius;
    float lastFrameTime;
    
    ofPolyline innerPolyline;
    
    vector<pair<int, unsigned int>> influenceSpatialLookup;
    vector<int> influenceStartIndices;
    
    vector<pair<int, unsigned int>> connectionsSpatialLookup;
    vector<int> connectionsStartIndices;
    
    void setBoundsSize(ofVec3f bounds);
    void setBounds(ofVec2f xBounds, ofVec2f yBounds);

    // aesthetics
    void setMinVelocity(float minVelocity);
    void setMaxVelocity(float maxVelocity);
    void setMinSize(float minSize);
    void setMaxSize(float maxSize);
    void setVelocityCurve(float velocityCurve);
    void setLineThickness(float lineThickness);
    void setVelocityHue(float hue);
    void setHotColor(ofColor hotColor);
    void setCoolColor(ofColor coolColor);
    void setZ(float z);
    void setLastFrameTime(float lastFrameTime);
    
    void setNumberParticles(int number);
    void setCenter(float centerX, float centerY);
    void setMode(int drawMode);
    void setWidth(int width);
    void setHeight(int height);
    void setConnectionRadius(float connectionRadius);
    void setInnerBoundarySpace(ofPolyline polyline);
    void resetDistanceLookups();
    
    // spatial lookup functions
    unsigned int hashCell(int cellX, int cellY);
    unsigned int getKeyFromHash(unsigned int hash, int size);
    pair<int, int> positionToCellCoordinate(ofVec2f position, float radius);
    void updateSpatialLookup(float radius, vector<int> &startIndices, vector<pair<int, unsigned int>> &spatialLookup);
    vector<int> foreachPointWithinRadius(int particleIndex, float radius, vector<int> &startIndices, vector<pair<int, unsigned int>> &spatialLookup);

    // creation functions
    void addParticle();
    void addParticle(ofVec3f position);
    void removeRandomParticle();
    void removeDeadParticles();
    ofVec2f getRandom2DDirection();
    
    void updateParticleSystem();
    void draw();
    
    void updateMesh(int particleIndex);
    void updateConnectionMesh(int particleIndex);
    void updateTriangle(int particleIndex);
    void updateLine(int particleIndex);
    void updatePoint(int particleIndex);
    void initializeTrianglesMesh(int numParticles, int shapeResolution);
    void initializeQuadsMesh(int numParticles);
    void initializeLinesMesh(int numParticles);
    void initializePointsMesh(int numParticles);
    void resetRandom();
    
    void saveSvg();
    
    int width, height;
    float minVelocity, maxVelocity, minSize, maxSize, velocityCurve, lineThickness;
    float z;
    ofColor coolColor, hotColor;
    
    vector<ofVec2f> cellOffsets;
private:
};

#endif /* ParticleSystem_hpp */
