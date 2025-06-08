//
//  FluidSystem2D.hpp
//  fluidSimulation
//

#ifndef FluidSystem2D_hpp
#define FluidSystem2D_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleSystem.hpp"
#include "Kernels.hpp"
#include "Presence.hpp"
#include "tbb/parallel_for.h"
#include "tbb/parallel_sort.h"

class FluidSystem2D : public ParticleSystem {
public:
    FluidSystem2D();
    
    void updatePresences(vector<Presence> &presences);
    void updateFluidSystem();

    void resolveCollisions(int particleIndex);
    ofVec2f pushParticlesAwayFromPoint(ofVec2f pointA, ofVec2f pointB, ofVec2f velocity);
    ofVec2f pullParticlesToPoint(ofVec2f pointA, ofVec2f pointB);
    ofVec2f pushParticlesAwayFromPresence(Presence presence, ofVec2f pointA);
    
    // math
    float calculatePressureFromDensity(float density);
    float calculateNearPressureFromDensity(float nearDensity);
    float calculateSharedPressure(float densityA, float densityB);
    float calculateSharedNearPressure(float nearDensityA, float nearDensityB);
    
    pair<float, float> calculateDensity(int particleIndex, float radius);
    pair<float, float> convertDensityToPressure(float density, float nearDensity);
    ofVec2f calculateViscosityForce(int particleIndex, float radius);
    ofVec2f calculatePressureForce(int particleIndex, float radius);
    ofVec2f calculateExternalForce(int particleIndex);
    ofVec2f calculateInteractiveForce(int particleIndex);

    glm::vec2 getClosestPointOnLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p);
    
    // reset functions
    void resetGrid(float scale);
    
    // setters
    void setDeltaTime(float deltaTime);
    void setInfluenceRadius(float influenceRadius);
    void setGravityMultiplier(float gravityMultiplier);
    void setGravityRotation(ofVec2f gravityRotation);
    void setTargetDensity(float targetDensity);
    void setPressureMultiplier(float pressureMultiplier);
    void setNearPressureMultiplier(float nearPressureMultiplier);
    void setViscosityStrength(float viscosityStrength);
    void setCollisionDamping(float collisionDamping);
    void setGravityTheta(float gravityTheta);
    ofVec2f getClosestPointOnLineSegment(const ofVec2f& A, const ofVec2f& B, const ofVec2f& P);
    
    Kernels kernels;
    float influenceRadius, gravityConstant, deltaTime, collisionDamping, predictionFactor;
    float targetDensity, nearPressureMultiplier, pressureMultiplier, gravityMultiplier, timeScalar, viscosityStrength;
    int boundaryState;
    
    vector<Presence> presences;
private:
    vector<ofVec2f> cellOffsets;
};

#endif /* FluidSystem2D_hpp */
