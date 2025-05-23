//
//  ParticleSystem.cpp
//  fluidSimulation
//

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem() {
    radius = 1.0;
    predictionFactor = 1.0f / 120.0f;
    pressureMultiplier = 1.0;
    nearPressureMultiplier = 1.0;
    targetDensity = 1.0;
    viscosityStrength = 0.5;
    collisionDamping = 0.95;
    connectionRadius = 50;
    
    //meters per second
    gravityConstant = 9.8;
    gravityMultiplier = 1.0;
    gravityForce = ofVec2f(0.0, -1.0);

    pauseActive = false;
    mouseForce = 1.0;
    circleBoundaryRadius = 455;
    
    rectangleResolution = 4;
    circleResolution = 22;
    
    systemWidth = ofGetWidth();
    systemHeight = ofGetHeight();
}

void ParticleSystem::setWidth(int _systemWidth) {
    systemWidth = _systemWidth;
}

void ParticleSystem::setHeight(int _systemHeight) {
    systemHeight = _systemHeight;
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
    
    presenceBoundary.draw();
}

// create particles
void ParticleSystem::addParticle() {
    ofVec2f position;
    
    if (circleBoundaryActive) {
        ofVec2f center = ofVec2f(systemWidth / 2.0, systemHeight / 2.0);
        
        float theta = ofRandom(0, TWO_PI);
        float magnitude = ofRandom(0, circleBoundaryRadius);
        
        float x = cos(theta) * magnitude;
        float y = sin(theta) * magnitude;
        
        position = ofVec2f(x, y) + center;
    } else {
        float x = ofRandom(bounds.x, bounds.x + boundsSize.x);
        float y = ofRandom(bounds.y, bounds.y + boundsSize.y);
        
        position = ofVec2f(x, y);
    }
    
    int index = particles.size();
    particles.push_back(Particle(position, 1.0, index));
}

ofVec2f ParticleSystem::getRandom2DDirection() {
    ofVec2f randomDirection = ofVec2f(1.0, 0.0).rotateRad(ofRandom(0, TWO_PI));
    return randomDirection;
}

ofVec3f ParticleSystem::getRandom3DDirection() {
    ofVec3f unitVector = ofVec3f(1.0, 0.0, 0.0);
    
    float rX = ofRandom(0, TWO_PI);
    float rY = ofRandom(0, TWO_PI);
    float rZ = ofRandom(0, TWO_PI);
    
    ofVec3f randomDirection = unitVector.rotateRad(rX, rY, rZ);
    
    return randomDirection;
}

void ParticleSystem::pause(Boolean _pauseActive) {
    pauseActive = _pauseActive;
}

void ParticleSystem::nextFrame() {
    nextFrameActive = true;
}

void ParticleSystem::saveSvg() {
    exportFrameActive = true;
}

// mouse input
void ParticleSystem::mouseInput(int x, int y, int button, Boolean active) {
    mouseInputActive = active;
    mouseButton = button;
    mouseInput(x, y);
}

void ParticleSystem::mouseInput(int x, int y) {
    mousePosition = ofVec2f(x, y);
}

void ParticleSystem::setPresence(float _x, float _width) {
    float x1 = _x;
    float x2 = _x;
    float y1 = 0;
    float y2 = systemHeight;

    presenceBoundary.clear();
    presenceBoundary.addVertex(ofVec3f(x1, y1));
    presenceBoundary.addVertex(ofVec3f(x2, y2));
    presenceBoundary.close();
        
    presenceWidth = _width;
}

// setters
void ParticleSystem::setNumberParticles(int number) {
    if (number > particles.size()) {
        while (particles.size() < number) {
            addParticle();
        }
        spatialLookup.resize(particles.size());
        startIndices.resize(particles.size());
        connectionsSpatialLookup.resize(particles.size());
        connectionsStartIndices.resize(particles.size());
        setMode(drawModeInt);
    }
    if (number < particles.size()) {
        while (particles.size() > number) {
            particles.pop_back();
        }
        spatialLookup.resize(particles.size());
        startIndices.resize(particles.size());
        connectionsSpatialLookup.resize(particles.size());
        connectionsStartIndices.resize(particles.size());
        setMode(drawModeInt);
    }
}

void ParticleSystem::setBoundsSize(ofVec3f _boundsSize) {
    center.x = systemWidth / 2.0;
    center.y = systemHeight / 2.0;
    boundsSize = _boundsSize;
    bounds.x = center.x - boundsSize.x / 2.0;
    bounds.y = center.y  - boundsSize.y / 2.0;
    bounds.z = center.x - boundsSize.z / 2.0;
    
    xBounds = ofVec2f(bounds.x, bounds.x + boundsSize.x);
    xBounds = ofVec2f(bounds.x, bounds.x + boundsSize.x);
    yBounds = ofVec2f(bounds.y, bounds.y + boundsSize.y);
    zBounds = ofVec2f(bounds.z, bounds.z + boundsSize.z);
}

void ParticleSystem::setRadius(float _radius) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].setRadius(_radius);
    }
    kernels.calculate3DVolumesFromRadius(_radius);
    radius = _radius;
}

void ParticleSystem::setGravityRotation(ofVec2f _gravityRotation) {
    gravityForce = _gravityRotation * gravityMultiplier;
}

void ParticleSystem::setGravityMultiplier(float _gravityMultiplier) {
    gravityMultiplier = _gravityMultiplier;
}

void ParticleSystem::setDeltaTime(float _deltaTime) {
    deltaTime = _deltaTime;
}

void ParticleSystem::setCollisionDamping(float _collisionDamping) {
    collisionDamping = _collisionDamping;
}

void ParticleSystem::setTargetDensity(float _targetDensity) {
    targetDensity = _targetDensity;
}

void ParticleSystem::setPressureMultiplier(float _pressureMultiplier) {
    pressureMultiplier = _pressureMultiplier;
}

void ParticleSystem::setViscosityStrength(float _viscosityStrength) {
    viscosityStrength = _viscosityStrength;
}

void ParticleSystem::setNearPressureMultiplier(float _nearPressureMultiplier) {
    nearPressureMultiplier = _nearPressureMultiplier;
}

void ParticleSystem::setMouseRadius(int _mouseRadius) {
    mouseRadius = _mouseRadius;
}

void ParticleSystem::setGravityTheta(float _gravityTheta) {
    float x = cos(_gravityTheta);
    float y = sin(_gravityTheta);
    
    gravityForce = ofVec2f(x, y);
}

void ParticleSystem::setCoolColor(ofColor coolColor) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].coolColor = coolColor;
    }
}

void ParticleSystem::setHotColor(ofColor hotColor) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].hotColor = hotColor;
    }
}

void ParticleSystem::setMinVelocity(float minVelocity) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].minVelocity = minVelocity;
    }
}

void ParticleSystem::setMaxVelocity(float maxVelocity) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].maxVelocity = maxVelocity;
    }
}

void ParticleSystem::setLineThickness(float lineThickness) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].lineThickness = lineThickness;
    }
}

void ParticleSystem::setVelocityCurve(float velocityCurve) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].velocityCurve = velocityCurve;
    }
}

void ParticleSystem::setMinSize(float minSize) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].minSize = minSize;
    }
}

void ParticleSystem::setMaxSize(float maxSize) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].maxSize = maxSize;
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
    }
    else if (_drawModeInt == 4) {
        
        drawMode = POINTS;
        initializePointsMesh(particles.size());
    }
    else if (_drawModeInt == 5) {
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

void ParticleSystem::setCircleBoundary(Boolean _circleBoundaryActive) {
    circleBoundaryActive = _circleBoundaryActive;
}

void ParticleSystem::setMouseForce(float _mouseForce) {
    mouseForce = _mouseForce;
}

void ParticleSystem::setConnectionRadius(float _connectionRadius) {
    connectionRadius = _connectionRadius;
}

void ParticleSystem::setInnerBoundarySpace(ofPolyline _innerPolyline) {
    innerPolyline = _innerPolyline;
}
