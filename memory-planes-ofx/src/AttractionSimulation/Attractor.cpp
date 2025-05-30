//
//  Attractor.cpp
//  memory-planes-ofx
//

#include "Attractor.hpp"

Attractor::Attractor() {
    
}

Attractor::Attractor(int _index) {
    index = _index;
    maxNumberParticles = 20;
    spawnTime = 1.0;
    spawnTimer = 0.0;
    canAddParticle = false;
    isAlive = true;
    numberParticles = 0;
    lifetime = 0.0;
    totalLifetime = 2.0;
}

void Attractor::update() {
    spawnTimer += lastFrameTime;
    lifetime += lastFrameTime;
    
    if (lifetime > totalLifetime) {
        isAlive = false;
    }

    if (spawnTimer > spawnTime) {
        spawnTimer = 0;
        spawnTime = ofRandom(1.0, 2.0);
        
        if (numberParticles < maxNumberParticles) {
            numberParticles++;
            canAddParticle = true;
        }
    }
}

void Attractor::setLastFrameTime(float _lastFrameTime) {
    lastFrameTime = _lastFrameTime;
}

void Attractor::draw() {
    
}

void Attractor::setPosition(ofVec3f _position) {
    position = _position;
    lifetime = 0;
}
