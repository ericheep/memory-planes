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
#include "tbb/parallel_for.h"
#include "tbb/parallel_sort.h"

class FluidSystem2D : public ParticleSystem {
public:
    FluidSystem2D();
    
    void updateFluidSystem();

    void resolveCollisions(int particleIndex);
    ofVec2f pushParticlesAwayFromPoint(ofVec2f pointA, ofVec2f pointB, ofVec2f velocity);
    ofVec2f pullParticlesToPoint(ofVec2f pointA, ofVec2f pointB);
    ofVec2f pushParticlesAwayFromLine(ofPolyline line, ofVec2f pointA, ofVec2f velocity);
    
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
    void resetRandom();
    void resetGrid(float scale);
    void resetCircle(float scale);
    
    // setters
    void setPresence(int index, float x, float width);
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
    
    Kernels kernels;
    float influenceRadius, gravityConstant, deltaTime, collisionDamping, predictionFactor;
    float targetDensity, nearPressureMultiplier, pressureMultiplier, gravityMultiplier, timeScalar, viscosityStrength;
    
    bool presenceActive;
    vector<float> presenceWidths;
    vector<ofPolyline> presenceBoundaries;
    ofVec2f presenceVelocity;
private:
    vector<ofVec2f> cellOffsets;
};

#endif /* FluidSystem2D_hpp */
