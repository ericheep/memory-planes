//
//  FluidSystem2D.cpp
//  fluidSimulation
//

#include "FluidSystem2D.hpp"

FluidSystem2D::FluidSystem2D() {
    influenceRadius = 1.0;

    kernels.calculate2DVolumesFromRadius(influenceRadius);
    kernels.calculate3DVolumesFromRadius(influenceRadius);
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            cellOffsets.push_back(ofVec2f(i, j));
        }
    }
    
    predictionFactor = 1.0f / 60.0f;
    pressureMultiplier = 10.0;
    nearPressureMultiplier = 10.0;
    targetDensity = 1.0;
    viscosityStrength = 0.5;
    collisionDamping = 0.5;
    connectionRadius = 50;
    deltaTime = 1.0f / 60.0f;
    
    //meters per second
    gravityConstant = 9.8;
    gravityMultiplier = 1.0;
    gravityForce = ofVec2f(0.0, -1.0);
}

void FluidSystem2D::updateFluidSystem() {
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            ofVec2f externalForce = calculateExternalForce(i);
            particles[i].velocity += externalForce;
            particles[i].predictedPosition = particles[i].position + particles[i].velocity * predictionFactor;
        }
    });
    
    updateSpatialLookup(influenceRadius, influenceStartIndices, influenceSpatialLookup);

    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            particles[i].indicesWithinInfluenceRadius = foreachPointWithinRadius(i, influenceRadius, influenceStartIndices, influenceSpatialLookup);
            
            pair<float, float> densities = calculateDensity(i, influenceRadius);
            particles[i].density = densities.first;
            particles[i].nearDensity = densities.second;
        }
    });
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            ofVec2f pressureForce = calculatePressureForce(i, influenceRadius);
            ofVec2f pressureAcceleration = pressureForce / particles[i].density;
            particles[i].velocity += pressureAcceleration * deltaTime;
        }
    });
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            ofVec2f viscosityForce = calculateViscosityForce(i, influenceRadius);
            particles[i].velocity += viscosityForce * deltaTime;
        }
    });
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            particles[i].position += particles[i].velocity * deltaTime;
            resolveCollisions(i);
        }
    });
}

ofVec2f FluidSystem2D::calculateInteractiveForce(int particleIndex) {
    ofVec2f particlePosition = particles[particleIndex].position;
    ofVec2f particleVelocity = particles[particleIndex].velocity;
    
    ofVec2f interactiveForce= ofVec2f::zero();
    
    presenceActive = true;
    if (presenceActive) {
        //interactiveForce = pushParticlesAwayFromLine(presenceBoundary, particlePosition, presenceVelocity);
        
    }
    
    return interactiveForce;
}

ofVec2f FluidSystem2D::pullParticlesToPoint(ofVec2f pointA, ofVec2f pointB) {
    ofVec2f interactiveForce = ofVec2f::zero();
    float force = 10;
    
    float inputRadius = 30;
    float squareDistance = pointA.squareDistance(pointB);
    
    if (squareDistance < inputRadius * inputRadius) {
        float distance = sqrt(squareDistance);
        ofVec2f direction = (pointA - pointB) / distance;
        float scalarProximity = distance / inputRadius;
        
        interactiveForce =  direction * force * scalarProximity;
    }
    return interactiveForce;
}

ofVec2f FluidSystem2D::pushParticlesAwayFromPoint(ofVec2f pointA, ofVec2f pointB, ofVec2f velocity) {
    ofVec2f interactiveForce = ofVec2f::zero();
    float force = 10;
    
    float inputRadius = 30;
    float squareDistance = pointA.squareDistance(pointB);
    
    if (squareDistance < inputRadius * inputRadius) {
        float distance = sqrt(squareDistance);
        ofVec2f direction = (pointB - pointA) / distance;
        float scalarProximity = 1.0 - distance / inputRadius;
        
        interactiveForce =  direction * force * scalarProximity * scalarProximity;
    }
    return interactiveForce;
}

ofVec2f FluidSystem2D::pushParticlesAwayFromLine(ofPolyline boundary, ofVec2f pointA, ofVec2f velocity) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    ofVec2f boundaryPoint = boundary.getClosestPoint(ofVec3f(pointA.x, pointA.y));
    float distance = pointA.distance(boundaryPoint);
    
    /*if (distance < presenceWidth) {
     ofVec2f direction = (pointA - boundaryPoint) / distance;
     float scalarProximity = 1.0 - (distance / presenceWidth);
     
     interactiveForce =  direction * 20 * scalarProximity * scalarProximity;
     }*/
    
    return interactiveForce;
}

ofVec2f FluidSystem2D::calculateExternalForce(int particleIndex) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    interactiveForce = calculateInteractiveForce(particleIndex);
    
    return interactiveForce + gravityForce * gravityConstant * gravityMultiplier * deltaTime;
}

pair<float, float> FluidSystem2D::calculateDensity(int particleIndex, float radius) {
    vector<int> indicesWithinRadius = particles[particleIndex].indicesWithinInfluenceRadius;
    ofVec2f particlePosition = particles[particleIndex].predictedPosition;
    
    float density = 0.0f;
    float nearDensity = 0.0f;
    
    for (int i = 0; i < indicesWithinRadius.size(); ++i) {
        int neighborParticleIndex = indicesWithinRadius[i];
        
        float distance = particlePosition.distance(particles[neighborParticleIndex].predictedPosition);

        density += kernels.densityKernel(distance, radius);
        nearDensity += kernels.nearDensityKernel(distance, radius);
        // cout << distance << " " << radius << " " << density << endl;
    }

    return pair<float, float> (density, nearDensity);
}

ofVec2f FluidSystem2D::calculatePressureForce(int particleIndex, float radius) {
    vector<int> indicesWithinRadius = particles[particleIndex].indicesWithinInfluenceRadius;
    ofVec2f particlePosition = particles[particleIndex].predictedPosition;
    float density = particles[particleIndex].density;
    float nearDensity = particles[particleIndex].nearDensity;
    float pressure = calculatePressureFromDensity(density);
    float nearPressure = calculateNearPressureFromDensity(nearDensity);
    
    ofVec2f pressureForce = ofVec2f::zero();
    
    for (int i = 0; i < indicesWithinRadius.size(); ++i) {
        int neighborParticleIndex = indicesWithinRadius[i];
        if (particleIndex == neighborParticleIndex) continue;
        
        ofVec2f neighborPosition = particles[neighborParticleIndex].predictedPosition;
        float distance = particlePosition.distance(neighborPosition);
        ofVec2f direction = (neighborPosition - particlePosition) / distance;
        
        if (distance < 1e-5) continue;
        
        float slope = kernels.densityDerivative(distance, radius);
        float nearSlope = kernels.nearDensityDerivative(distance, radius);
        
        float neighborDensity = particles[neighborParticleIndex].density;
        float neighborNearDensity = particles[neighborParticleIndex].nearDensity;
        float neighborPressure = calculatePressureFromDensity(neighborDensity);
        float neighborNearPressure = calculateNearPressureFromDensity(neighborNearDensity);
        
        float sharedPressure = (pressure + neighborPressure) * 0.5;
        float sharedNearPressure = (nearPressure + neighborNearPressure) * 0.5;
        
        float epsilon = 1e-5;
        pressureForce += sharedPressure * direction * slope / std::max(density, epsilon);
        pressureForce += sharedNearPressure * direction * nearSlope / std::max(nearDensity, epsilon);
    }
    
    return pressureForce;
}

ofVec2f FluidSystem2D::calculateViscosityForce(int particleIndex, float radius) {
    vector<int> indicesWithinRadius = particles[particleIndex].indicesWithinInfluenceRadius;
    ofVec2f particlePosition = particles[particleIndex].predictedPosition;
    ofVec2f viscosityForce = ofVec2f::zero();
    
    for (int i = 0; i < indicesWithinRadius.size(); ++i) {
        int neighborParticleIndex = indicesWithinRadius[i];
        if (particleIndex == neighborParticleIndex) continue;
        
        float distance = particlePosition.distance(particles[neighborParticleIndex].predictedPosition);
        float influence = kernels.viscosityKernel(distance, radius);
        viscosityForce += (particles[neighborParticleIndex].velocity - particles[particleIndex].velocity) * influence;
    }
    
    return viscosityForce * viscosityStrength;
}

float FluidSystem2D::calculatePressureFromDensity(float density) {
    float densityError = density - targetDensity;
    return densityError * pressureMultiplier;
}

float FluidSystem2D::calculateNearPressureFromDensity(float nearDensity) {
    return nearDensity * nearPressureMultiplier;
}

glm::vec2 FluidSystem2D::getClosestPointOnLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p) {
    glm::vec2 ab = b - a;
    float abSquared = glm::dot(ab, ab);
    
    if (abSquared == 0.0f) {
        return a;
    }
    
    float t = glm::dot(p - a, ab) / abSquared;
    t = glm::clamp(t, 0.0f, 1.0f);
    
    return a + t * ab;
}

void FluidSystem2D::resolveCollisions(int particleIndex) {
    particles[particleIndex].position;
    
    if (particles[particleIndex].position.x < xBounds.x) {
        particles[particleIndex].velocity.x *= -1.0 * collisionDamping;
        particles[particleIndex].position.x = xBounds.x;
    }
    
    if (particles[particleIndex].position.x > xBounds.y) {
        particles[particleIndex].velocity.x *= -1.0 * collisionDamping;
        particles[particleIndex].position.x = xBounds.y;
    }
    
    if (particles[particleIndex].position.y < yBounds.x) {
        particles[particleIndex].velocity.y *= -1.0 * collisionDamping;
        particles[particleIndex].position.y = yBounds.x;
    }
    
    if (particles[particleIndex].position.y > yBounds.y) {
        particles[particleIndex].velocity.y *= -1.0 * collisionDamping;
        particles[particleIndex].position.y = yBounds.y;
    }
    
    if (innerPolyline.inside(particles[particleIndex].position)) {
        ofPoint closestPoint = innerPolyline.getClosestPoint(particles[particleIndex].position);
        
        glm::vec2 position;
        position.x = closestPoint.x;
        position.y = closestPoint.y;
        
        float minDistance = std::numeric_limits<float>::max();
        int segmentIndex = 0;
        
        auto & vertices = innerPolyline.getVertices();
        
        for (int i = 0; i < vertices.size(); ++i) {
            glm::vec2 a = vertices[i];
            glm::vec2 b = vertices[(i + 1) % 4];
            glm::vec2 closest = getClosestPointOnLine(a, b, position);
            
            float distance = glm::distance(position, closest);
            if (distance < minDistance) {
                minDistance = distance;
                segmentIndex = i;
            }
        }
        
        if (segmentIndex == 0 || segmentIndex == 2) {
            particles[particleIndex].velocity.y *= -1.0 * collisionDamping;
            particles[particleIndex].position.y = closestPoint.y;
        }
        
        if (segmentIndex == 1 || segmentIndex == 3) {
            particles[particleIndex].velocity.x *= -1.0 * collisionDamping;
            particles[particleIndex].position.x = closestPoint.x;
        }
    }
}

void FluidSystem2D::resetGrid(float scale) {
    int rows = ceil(pow(particles.size(), 0.5));
    int cols = ceil(pow(particles.size(), 0.5));
    
    float width = boundsSize.x;
    float height = boundsSize.y;
    
    float xOffset = width / 2.0 - width / 2.0 * scale;
    float yOffset = height / 2.0 - height / 2.0 * scale;
    
    for (int i = 0; i < rows; i++) {
        float xSpace = width * scale / float(rows + 1);
        float x = xSpace * (i + 1) + xOffset;
        
        for (int j = 0; j < cols; j++) {
            int particleIndex = i * cols + j;
            if (particleIndex >= particles.size()) return;
            
            float ySpace = height * scale / float(cols + 1);
            float y = ySpace * (j + 1) + yOffset;
            
            float jitterX = xSpace * ofRandom(-0.1, 0.1);
            float jitterY = ySpace * ofRandom(-0.1, 0.1);
            
            particles[particleIndex].position = ofVec2f(x + jitterX, y + jitterY);
            particles[particleIndex].velocity = getRandom2DDirection();
        }
    }
}

void FluidSystem2D::setInfluenceRadius(float _influenceRadius) {
    kernels.calculate3DVolumesFromRadius(_influenceRadius);
    influenceRadius = _influenceRadius;
}

void FluidSystem2D::setGravityTheta(float _gravityTheta) {
    float x = cos(_gravityTheta);
    float y = sin(_gravityTheta);
    
    gravityForce = ofVec2f(x, y);
}

void FluidSystem2D::setGravityRotation(ofVec2f _gravityRotation) {
    gravityForce = _gravityRotation * gravityMultiplier;
}

void FluidSystem2D::setGravityMultiplier(float _gravityMultiplier) {
    gravityMultiplier = _gravityMultiplier;
}

void FluidSystem2D::setDeltaTime(float _deltaTime) {
    deltaTime = _deltaTime;
}

void FluidSystem2D::setCollisionDamping(float _collisionDamping) {
    collisionDamping = _collisionDamping;
}

void FluidSystem2D::setTargetDensity(float _targetDensity) {
    targetDensity = _targetDensity;
}

void FluidSystem2D::setPressureMultiplier(float _pressureMultiplier) {
    pressureMultiplier = _pressureMultiplier;
}

void FluidSystem2D::setViscosityStrength(float _viscosityStrength) {
    viscosityStrength = _viscosityStrength;
}

void FluidSystem2D::setNearPressureMultiplier(float _nearPressureMultiplier) {
    nearPressureMultiplier = _nearPressureMultiplier;
}

void FluidSystem2D::setPresence(int index, float _x, float _width) {
    float x1 = _x;
    float x2 = _x;
    float y1 = 0;
    float y2 = height;
    
    presenceBoundaries[index].clear();
    presenceBoundaries[index].addVertex(ofVec3f(x1, y1));
    presenceBoundaries[index].addVertex(ofVec3f(x2, y2));
    presenceBoundaries[index].close();
    
    presenceWidths[index] = _width;
}
