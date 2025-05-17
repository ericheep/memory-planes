//
//  FluidSystem2D.cpp
//  fluidSimulation
//

#include "FluidSystem2D.hpp"

FluidSystem2D::FluidSystem2D() {
    kernels.calculate3DVolumesFromRadius(radius);
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            cellOffsets.push_back(ofVec2f(i, j));
        }
    }
}

void FluidSystem2D::update() {
    if (!pauseActive || nextFrameActive) {
        tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                ofVec2f externalForce = calculateExternalForce(i);
                particles[i].velocity += externalForce;
                particles[i].predictedPosition = particles[i].position + particles[i].velocity * predictionFactor;
            }
        });
        
        updateInfluenceRadiusSpatialLookup();
        updateConnectionsSpatialLookup();
        
        tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                particles[i].indicesWithinInfluenceRadius = foreachPointWithinInfluenceRadius(i);
                particles[i].indicesWithinConnectionRadius = foreachPointWithinConnectionRadius(i);
                pair<float, float> densities = calculateDensity(i);
                particles[i].density = densities.first;
                particles[i].nearDensity = densities.second;
            }
        });
        
        tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                ofVec2f pressureForce = calculatePressureForce(i);
                ofVec2f pressureAcceleration = pressureForce / particles[i].density;
                particles[i].velocity += pressureAcceleration * deltaTime;
                
                ofVec2f viscosityForce = calculateViscosityForce(i);
                particles[i].velocity += viscosityForce * deltaTime;
            }
        });
        
        tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
            for (int i = r.begin(); i < r.end(); ++i) {
                particles[i].position += particles[i].velocity * deltaTime;
                resolveCollisions(i);
            }
        });
        
        nextFrameActive = false;
    }
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            particles[i].update();
            particles[i].updateNeighbors();
            
            updateMesh(i);
        }
    });
}

ofVec2f FluidSystem2D::calculateInteractiveForce(int particleIndex) {
    ofVec2f particlePosition = particles[particleIndex].position;
    ofVec2f particleVelocity = particles[particleIndex].velocity;
    
    ofVec2f interactiveForce= ofVec2f::zero();
    
    if (mouseButton == 0) {
        interactiveForce = pushParticlesAwayFromPoint(mousePosition, particlePosition, particleVelocity);
    }
    if (mouseButton == 2) {
        interactiveForce = pullParticlesToPoint(mousePosition, particlePosition);
    }

    presenceActive = true;
    if (presenceActive) {
        interactiveForce = pushParticlesAwayFromLine(presenceBoundary, particlePosition, presenceVelocity);

    }
    
    return interactiveForce;
}

ofVec2f FluidSystem2D::pullParticlesToPoint(ofVec2f pointA, ofVec2f pointB) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    float inputRadius = mouseRadius;
    float squareDistance = pointA.squareDistance(pointB);
    
    if (squareDistance < inputRadius * inputRadius) {
        float distance = sqrt(squareDistance);
        ofVec2f direction = (pointA - pointB) / distance;
        float scalarProximity = distance / inputRadius;
        
        interactiveForce =  direction * mouseForce * scalarProximity;
    }
    return interactiveForce;
}

ofVec2f FluidSystem2D::pushParticlesAwayFromPoint(ofVec2f pointA, ofVec2f pointB, ofVec2f velocity) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    float inputRadius = mouseRadius;
    float squareDistance = pointA.squareDistance(pointB);
    
    if (squareDistance < inputRadius * inputRadius) {
        float distance = sqrt(squareDistance);
        ofVec2f direction = (pointB - pointA) / distance;
        float scalarProximity = 1.0 - distance / inputRadius;
        
        interactiveForce =  direction * mouseForce * scalarProximity * scalarProximity;
    }
    return interactiveForce;
}

ofVec2f FluidSystem2D::pushParticlesAwayFromLine(ofPolyline boundary, ofVec2f pointA, ofVec2f velocity) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    ofVec2f boundaryPoint = boundary.getClosestPoint(ofVec3f(pointA.x, pointA.y));
    float distance = pointA.distance(boundaryPoint);
    
    if (distance < presenceWidth) {
        ofVec2f direction = (pointA - boundaryPoint) / distance;
        float scalarProximity = 1.0 - (distance / presenceWidth);
        
        interactiveForce =  direction * 20 * scalarProximity * scalarProximity;
    }

    return interactiveForce;
}

ofVec2f FluidSystem2D::calculateExternalForce(int particleIndex) {
    ofVec2f interactiveForce = ofVec2f::zero();
    
    interactiveForce = calculateInteractiveForce(particleIndex);
    
    
    return interactiveForce + gravityForce * gravityConstant * gravityMultiplier * deltaTime;
}

pair<float, float> FluidSystem2D::calculateDensity(int particleIndex) {
    vector<int> indicesWithinRadius = particles[particleIndex].indicesWithinInfluenceRadius;
    ofVec2f particlePosition = particles[particleIndex].predictedPosition;
    
    float density = 0.0f;
    float nearDensity = 0.0f;
    
    for (int i = 0; i < indicesWithinRadius.size(); ++i) {
        int neighborParticleIndex = indicesWithinRadius[i];
        
        float distance = particlePosition.distance(particles[neighborParticleIndex].predictedPosition);
        density += kernels.densityKernel(distance, radius);
        nearDensity += kernels.nearDensityKernel(distance, radius);
    }
    
    return pair<float, float> (density, nearDensity);
}

ofVec2f FluidSystem2D::calculatePressureForce(int particleIndex) {
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
        direction = distance == 0.0 ? getRandom2DDirection() : direction;
        
        float slope = kernels.densityDerivative(distance, radius);
        float nearSlope = kernels.nearDensityDerivative(distance, radius);
        
        float neighborDensity = particles[neighborParticleIndex].density;
        float neighborNearDensity = particles[neighborParticleIndex].nearDensity;
        float neighborPressure = calculatePressureFromDensity(neighborDensity);
        float neighborNearPressure = calculateNearPressureFromDensity(neighborNearDensity);
        
        float sharedPressure = (pressure + neighborPressure) * 0.5;
        float sharedNearPressure = (nearPressure = neighborNearPressure) * 0.5;
        
        pressureForce += sharedPressure * direction * slope / density;
        pressureForce += sharedNearPressure * direction * nearSlope / nearDensity;
    }
    
    return pressureForce;
}

ofVec2f FluidSystem2D::calculateViscosityForce(int particleIndex) {
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

// spatial lookup

vector<int> FluidSystem2D::foreachPointWithinInfluenceRadius(int particleIndex) {
    ofVec2f position = particles[particleIndex].position;
    
    pair<int, int> center = positionToCellCoordinate(position, radius);
    int centerX = center.first;
    int centerY = center.second;
    float squareRadius = radius * radius;
    
    vector<int> indicesWithinRadius;
    
    for (auto offsetPair : cellOffsets) {
        int offsetX = offsetPair.x;
        int offsetY = offsetPair.y;
        
        unsigned int key = getKeyFromHash(hashCell(centerX + offsetX, centerY + offsetY));
        int cellStartIndex = startIndices[key];
        
        for (int i = cellStartIndex; i < spatialLookup.size(); i++) {
            if (spatialLookup[i].second != key) break;
            
            int otherParticleIndex = spatialLookup[i].first;
            float squareDistance = particles[otherParticleIndex].position.squareDistance(position);
            
            if (squareDistance <= squareRadius) {
                indicesWithinRadius.push_back(otherParticleIndex);
            }
        }
    }
    
    return indicesWithinRadius;
}

void FluidSystem2D::updateInfluenceRadiusSpatialLookup() {
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            pair<int, int> cell = positionToCellCoordinate(particles[i].position, radius);
            unsigned int cellKey = getKeyFromHash(hashCell(cell.first, cell.second));
            spatialLookup[i] = pair<int, unsigned int> (i, cellKey);
            startIndices[i] = INT_MAX;
        }
    });
    
    tbb::parallel_sort(spatialLookup.begin(), spatialLookup.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            unsigned int key = spatialLookup[i].second;
            unsigned int keyPrev = i == 0 ? UINT_MAX : spatialLookup[i - 1].second;
            if (key != keyPrev) {
                startIndices[key] = i;
            }
        }
    });
}

vector<int> FluidSystem2D::foreachPointWithinConnectionRadius(int particleIndex) {
    ofVec2f position = particles[particleIndex].position;
    
    pair<int, int> center = positionToCellCoordinate(position, connectionRadius);
    int centerX = center.first;
    int centerY = center.second;
    float squareRadius = connectionRadius * connectionRadius;
    
    vector<int> indicesWithinConnectionRadius;
    
    for (auto offsetPair : cellOffsets) {
        int offsetX = offsetPair.x;
        int offsetY = offsetPair.y;
        
        unsigned int key = getKeyFromHash(hashCell(centerX + offsetX, centerY + offsetY));
        int cellStartIndex = connectionsStartIndices[key];
        
        for (int i = cellStartIndex; i < connectionsSpatialLookup.size(); i++) {
            if (connectionsSpatialLookup[i].second != key) break;
            
            int otherParticleIndex = connectionsSpatialLookup[i].first;
            ofVec2f otherPosition = particles[otherParticleIndex].position;
            
            float squareDistance = otherPosition.squareDistance(position);
            
            if (squareDistance <= squareRadius) {
                indicesWithinConnectionRadius.push_back(otherParticleIndex);
                
                if (otherParticleIndex != particleIndex) {
                    particles[particleIndex].updateNeighbor(otherParticleIndex, otherPosition);
                }
            }
        }
    }
    
    return indicesWithinConnectionRadius;
}

void FluidSystem2D::updateConnectionsSpatialLookup() {
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            pair<int, int> cell = positionToCellCoordinate(particles[i].position, connectionRadius);
            unsigned int cellKey = getKeyFromHash(hashCell(cell.first, cell.second));
            connectionsSpatialLookup[i] = pair<int, unsigned int> (i, cellKey);
            connectionsStartIndices[i] = INT_MAX;
        }
    });
    
    tbb::parallel_sort(connectionsSpatialLookup.begin(), connectionsSpatialLookup.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            unsigned int key = connectionsSpatialLookup[i].second;
            unsigned int keyPrev = i == 0 ? UINT_MAX : connectionsSpatialLookup[i - 1].second;
            if (key != keyPrev) {
                connectionsStartIndices[key] = i;
            }
        }
    });
}

unsigned int FluidSystem2D::hashCell(int cellX, int cellY) {
    unsigned int a = u_int(cellX * 15823);
    unsigned int b = u_int(cellY * 9737333);
    return a + b;
}

unsigned int FluidSystem2D::getKeyFromHash(unsigned int hash) {
    return hash % u_int(spatialLookup.size());
}

pair<int, int> FluidSystem2D::positionToCellCoordinate(ofVec2f position, float radius) {
    return pair<int, int> (int(position.x / radius), int(position.y / radius));
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

// reset particles
void FluidSystem2D::resetRandom() {
    if (circleBoundaryActive) {
        resetCircle(1.0);
    } else {
        for (int i = 0; i < particles.size(); ++i) {
            float x = ofRandom(bounds.x, bounds.x + boundsSize.x);
            float y = ofRandom(bounds.y, bounds.y + boundsSize.y);
            particles[i].position = ofVec2f(x, y);
            particles[i].velocity = getRandom2DDirection();
        }
    }
}

void FluidSystem2D::resetGrid(float scale) {
    int rows = ceil(pow(particles.size(), 0.5));
    int cols = ceil(pow(particles.size(), 0.5));
    
    float width = boundsSize.x;
    float height = boundsSize.y;
    
    float xOffset = systemWidth / 2.0 - width / 2.0 * scale;
    float yOffset = systemHeight / 2.0 - height / 2.0 * scale;
    
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

void FluidSystem2D::resetCircle(float scale) {
    ofVec2f center = ofVec2f(systemWidth / 2.0, systemHeight / 2.0);
    
    float diameter = boundsSize.x;
    if (boundsSize.y < boundsSize.x) {
        diameter = boundsSize.y;
    }
    
    float radius = diameter / 2.0 * scale;
    
    for (int i = 0; i < particles.size(); i++) {
        float theta = ofRandom(0, TWO_PI);
        float magnitude = ofRandom(0, radius);
        
        float x = cos(theta) * magnitude;
        float y = sin(theta) * magnitude;
        
        particles[i].position = ofVec2f(x, y) + center;
        particles[i].velocity = getRandom2DDirection();
    }
}
