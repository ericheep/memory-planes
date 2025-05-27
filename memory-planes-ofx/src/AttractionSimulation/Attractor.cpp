//
//  Attractor.cpp
//  memory-planes-ofx
//

#include "Attractor.hpp"

Attractor::Attractor() {
    maxNumberParticles = 20;
    spawnTime = 1.0;
    canAddParticle = false;
}

void Attractor::update() {
    spawnTimer += ofGetLastFrameTime();
    if (spawnTimer > spawnTime) {
        spawnTimer = 0;
        spawnTime = ofRandom(1.0, 2.0);
        
        if (numberParticles < maxNumberParticles) {
            numberParticles++;
            canAddParticle = true;
        }
    }
}

void Attractor::draw() {
    
}

void Attractor::setPosition(ofVec2f _position) {
    position = _position;
}
