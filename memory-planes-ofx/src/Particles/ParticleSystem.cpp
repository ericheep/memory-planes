//
//  ParticleSystem.cpp
//  fluidSimulation
//

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem() {
    rectangleResolution = 4;
    circleResolution = 22;
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            cellOffsets.push_back(ofVec2f(i, j));
        }
    }
}

void ParticleSystem::updateParticleSystem() {
    updateSpatialLookup(connectionRadius, connectionsStartIndices, connectionsSpatialLookup);
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            particles[i].indicesWithinConnectionRadius = foreachPointWithinRadius(i, connectionRadius, connectionsStartIndices, connectionsSpatialLookup);
            
            for (int j = 0; j < particles[i].indicesWithinConnectionRadius.size(); j++) {
                int otherParticleIndex = particles[i].indicesWithinConnectionRadius[j];
                particles[i].updateNeighbor(otherParticleIndex, particles[otherParticleIndex].position);
            }
            
            particles[i].update();
            updateMesh(i);
        }
    });
}

void ParticleSystem::setWidth(int _width) {
    width = _width;
}

void ParticleSystem::setHeight(int _height) {
    height = _height;
}

void ParticleSystem::initializeTrianglesMesh(int numParticles, int shapeResolution) {
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    for (int i = 0; i < numParticles; i++) {
        mesh.addVertex(ofVec3f(0, 0, 0));
        mesh.addColor(ofColor::black);
        for (int j = 0; j < shapeResolution; j++) {
            mesh.addVertex(ofVec3f(0, 0, 0));
            mesh.addColor(ofColor::black);
        }
    }
    
    for (int i = 0; i < numParticles; i++) {
        int particleIndex = (shapeResolution + 1) * i;
        
        for (int j = 0; j < shapeResolution; j++) {
            mesh.addIndex(particleIndex);
            mesh.addIndex(particleIndex + j + 1);
            
            if (j < shapeResolution - 1) {
                mesh.addIndex(particleIndex + j + 2);
            } else {
                mesh.addIndex(particleIndex + 1);
            }
        }
    }
}

void ParticleSystem::initializeLinesMesh(int numParticles) {
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    for (int i = 0; i < numParticles * 2; i++) {
        mesh.addVertex(ofVec3f(0, 0, 0));
        mesh.addColor(ofColor::black);
        mesh.addIndex(i);
    }
}

void ParticleSystem::initializePointsMesh(int numParticles) {
    mesh.clear();
    glPointSize(3);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    for (int i = 0; i < numParticles; i++) {
        mesh.addVertex(ofVec3f(0, 0, 0));
        mesh.addColor(ofColor::black);
        mesh.addIndex(i);
    }
}

void ParticleSystem::updateMesh(int particleIndex) {
    switch(drawMode) {
        case CIRCLES:
            updateTriangle(particleIndex);
            break;
        case RECTANGLES:
            updateTriangle(particleIndex);
            break;
        case VECTORS:
            updateTriangle(particleIndex);
            break;
        case LINES:
            updateLine(particleIndex);
            break;
        case POINTS:
            updatePoint(particleIndex);
            break;
    }
}

void ParticleSystem::updateTriangle(int particleIndex) {
    ofMesh shapeMesh = particles[particleIndex].getShapeMesh();
    int meshIndex = (shapeResolution + 1) * particleIndex;
    
    mesh.setVertex(meshIndex, particles[particleIndex].position);
    mesh.setColor(meshIndex, particles[particleIndex].particleColor);
    
    for (int j = 0; j < shapeResolution; j++) {
        mesh.setVertex(meshIndex + j + 1, shapeMesh.getVertex(j) + particles[particleIndex].position);
        mesh.setColor(meshIndex + j + 1, particles[particleIndex].particleColor);
    }
}

void ParticleSystem::updateLine(int particleIndex) {
    ofMesh shapeMesh = particles[particleIndex].getShapeMesh();
    
    int indexA = particleIndex * 2;
    int indexB = particleIndex * 2 + 1;
    
    mesh.setVertex(indexA, shapeMesh.getVertex(0) + particles[particleIndex].position);
    mesh.setColor(indexA, particles[particleIndex].particleColor);
    
    mesh.setVertex(indexB, shapeMesh.getVertex(1) + particles[particleIndex].position);
    mesh.setColor(indexB, particles[particleIndex].particleColor);
}

void ParticleSystem::updatePoint(int particleIndex) {
    mesh.setVertex(particleIndex, particles[particleIndex].position);
    mesh.setColor(particleIndex, particles[particleIndex].particleColor);
}

void ParticleSystem::draw() {
    mesh.draw();
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i].drawConnections();
    }
}

// create particles
void ParticleSystem::addParticle() {
    float x = ofRandom(bounds.x, bounds.x + boundsSize.x);
    float y = ofRandom(bounds.y, bounds.y + boundsSize.y);
    
    addParticle(ofVec2f(x, y));
}

void ParticleSystem::setZ(float _z) {
    z = _z;
    for (auto &particle : particles) {
        particle.position.z = z;
    }
}

// create particles
void ParticleSystem::addParticle(ofVec3f position) {
    int index = particles.size();
    
    Particle particle = Particle(position, index);
    particle.coolColor = coolColor;
    particle.hotColor = hotColor;
    particle.lineThickness = lineThickness;
    particle.velocityCurve = velocityCurve;
    particle.targetMinSize = minSize;
    particle.targetMaxSize = maxSize;
    particle.minVelocity = minVelocity;
    particle.maxVelocity = maxVelocity;
    
    particles.push_back(particle);
}

void ParticleSystem::resetDistanceLookups() {
    int hashTableSize = particles.size() * 2;
    influenceSpatialLookup.resize(particles.size());
    influenceStartIndices.resize(hashTableSize, INT_MAX);
    
    connectionsSpatialLookup.resize(particles.size());
    connectionsStartIndices.resize(hashTableSize, INT_MAX);

    setMode(drawModeInt);
}

ofVec2f ParticleSystem::getRandom2DDirection() {
    return ofVec2f(1.0, 0.0).rotateRad(ofRandom(0, TWO_PI));
}

// setters
void ParticleSystem::setNumberParticles(int number) {
    if (number > particles.size()) {
        while (particles.size() < number) {
            addParticle();
        }
    }
    
    if (number < particles.size()) {
        while (particles.size() > number) {
            particles.pop_back();
        }
    }
    
    resetDistanceLookups();
}

void ParticleSystem::removeRandomParticle() {
    int randomIndex = ofRandom(0, particles.size() - 1);
    particles.erase(particles.begin() + randomIndex);
    
    resetDistanceLookups();
}

void ParticleSystem::removeDeadParticles() {
    int prevParticlesSize = particles.size();
    
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle& particle) {
        return !particle.isAlive;
    }), particles.end());
    
    if (prevParticlesSize != particles.size()) {
        resetDistanceLookups();
    }
}

void ParticleSystem::setLastFrameTime(float lastFrameTime) {
    for (auto &particle : particles) {
        particle.setLastFrameTime(lastFrameTime);
    }
}

void ParticleSystem::setBoundsSize(ofVec3f _boundsSize) {
    center.x = width / 2.0;
    center.y = height / 2.0;
    
    boundsSize = _boundsSize;
    bounds.x = center.x - boundsSize.x / 2.0;
    bounds.y = center.y - boundsSize.y / 2.0;
    
    xBounds = ofVec2f(bounds.x, bounds.x + boundsSize.x);
    yBounds = ofVec2f(bounds.y, bounds.y + boundsSize.y);
}

void ParticleSystem::setBounds(ofVec2f _xBounds, ofVec2f _yBounds) {
    boundsSize = ofVec3f(_xBounds.y - _xBounds.x, _yBounds.y - _yBounds.x, 0);
    
    bounds.x = _xBounds.x;
    bounds.y = _yBounds.x;
    
    xBounds = _xBounds;
    yBounds = _yBounds;
}

void ParticleSystem::setCoolColor(ofColor _coolColor) {
    coolColor = _coolColor;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].coolColor = coolColor;
    }
}

void ParticleSystem::setHotColor(ofColor _hotColor) {
    hotColor = _hotColor;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].hotColor = hotColor;
    }
}

void ParticleSystem::setMinVelocity(float _minVelocity) {
    minVelocity = _minVelocity;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].minVelocity = minVelocity;
    }
}

void ParticleSystem::setMaxVelocity(float _maxVelocity) {
    maxVelocity = _maxVelocity;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].maxVelocity = maxVelocity;
    }
}

void ParticleSystem::setLineThickness(float _lineThickness) {
    lineThickness = _lineThickness;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].lineThickness = lineThickness;
    }
}

void ParticleSystem::setVelocityCurve(float _velocityCurve) {
    velocityCurve = _velocityCurve;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].velocityCurve = velocityCurve;
    }
}

void ParticleSystem::setMinSize(float _minSize) {
    minSize = _minSize;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].targetMinSize = minSize;
    }
}

void ParticleSystem::setMaxSize(float _maxSize) {
    maxSize = _maxSize;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].targetMaxSize = maxSize;
    }
}

void ParticleSystem::setMode(int _drawModeInt) {
    if (_drawModeInt == 0) {
        drawMode = CIRCLES;
        shapeResolution = circleResolution;
        initializeTrianglesMesh(particles.size(), circleResolution);
    } else if (_drawModeInt == 1) {
        drawMode = RECTANGLES;
        shapeResolution = rectangleResolution;
        initializeTrianglesMesh(particles.size(), rectangleResolution);
    } else if (_drawModeInt == 2) {
        drawMode = VECTORS;
        shapeResolution = rectangleResolution;
        initializeTrianglesMesh(particles.size(), rectangleResolution);
    } else if (_drawModeInt == 3) {
        drawMode = LINES;
        initializeLinesMesh(particles.size());
    } else if (_drawModeInt == 4) {
        drawMode = POINTS;
        initializePointsMesh(particles.size());
    } else if (_drawModeInt == 5) {
        // drawMode = SVG;
        // initializePointsMesh(particles.size());
    }
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i].setMode(_drawModeInt);
    }
    
    drawModeInt = _drawModeInt;
}

void ParticleSystem::setCenter(float _centerX, float _centerY) {
    centerX = _centerX;
    centerY = _centerY;
}

void ParticleSystem::setConnectionRadius(float _connectionRadius) {
    connectionRadius = _connectionRadius;
}

void ParticleSystem::setInnerBoundarySpace(ofPolyline _innerPolyline) {
    innerPolyline = _innerPolyline;
}

unsigned int ParticleSystem::hashCell(int cellX, int cellY) {
    unsigned int a = u_int(cellX * 15823);
    unsigned int b = u_int(cellY * 9737333);
    return a + b;
}

unsigned int ParticleSystem::getKeyFromHash(unsigned int hash, int size) {
    return hash % u_int(size);
}

pair<int, int> ParticleSystem::positionToCellCoordinate(ofVec2f position, float radius) {
    return pair<int, int> (int(position.x / radius), int(position.y / radius));
}

vector<int> ParticleSystem::foreachPointWithinRadius(int particleIndex, float radius, vector<int> &startIndices, vector<pair<int, unsigned int>> &spatialLookup) {
    ofVec2f position = particles[particleIndex].position;
    
    pair<int, int> center = positionToCellCoordinate(position, radius);
    int centerX = center.first;
    int centerY = center.second;
    float squareRadius = radius * radius;
    
    vector<int> indicesWithinConnectionRadius;
    
    for (auto offsetPair : cellOffsets) {
        int offsetX = offsetPair.x;
        int offsetY = offsetPair.y;
        
        unsigned int key = getKeyFromHash(hashCell(centerX + offsetX, centerY + offsetY), startIndices.size());
        int cellStartIndex = startIndices[key];
        if (cellStartIndex == INT_MAX) continue;

        for (int i = cellStartIndex; i < spatialLookup.size(); i++) {
            if (spatialLookup[i].second != key) break;
            
            int otherParticleIndex = spatialLookup[i].first;
            ofVec2f otherPosition = particles[otherParticleIndex].position;
                        
            if (otherPosition.squareDistance(position) <= squareRadius) {
                indicesWithinConnectionRadius.push_back(otherParticleIndex);
            }
        }
    }
    
    return indicesWithinConnectionRadius;
}

void ParticleSystem::updateSpatialLookup(float radius, vector<int> &startIndices, vector<pair<int, unsigned int>> &spatialLookup) {
    std::fill(startIndices.begin(), startIndices.end(), INT_MAX);
    
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            pair<int, int> cell = positionToCellCoordinate(particles[i].position, radius);
            unsigned int cellKey = getKeyFromHash(hashCell(cell.first, cell.second), startIndices.size());
            spatialLookup[i] = pair<int, unsigned int> (i, cellKey);
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

// reset particles
void ParticleSystem::resetRandom() {
    for (int i = 0; i < particles.size(); ++i) {
        float x = ofRandom(bounds.x, bounds.x + boundsSize.x);
        float y = ofRandom(bounds.y, bounds.y + boundsSize.y);
        particles[i].position = ofVec2f(x, y);
        particles[i].velocity = getRandom2DDirection();
    }
}
