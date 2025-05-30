//
//  AttractionSystem.cpp
//  memory-planes-ofx
//

#include "AttractionSystem.hpp"

AttractionSystem::AttractionSystem() {
    
}

AttractionSystem::AttractionSystem(int _width, int _height) {
    width = _width;
    height = _height;
    numberParticlesPerAttractor = 10;
    particleDestroyTimer = 0.0;
    particleDestroyTime = 1.0;
    
    centerAttractor = Attractor(0);
    centerAttractor.position = (ofVec3f(0.0, 0.0, 0.0));
    isConverging = false;
    isAttracting = true;
}

void AttractionSystem::updateAttractors() {
    for (int i = 0; i < attractors.size(); i++) {
        attractors[i].setLastFrameTime(lastFrameTime);
        attractors[i].update();
        
        if (attractors[i].canAddParticle) {
            attractors[i].canAddParticle = false;
            
            float theta = ofRandom(PI * 1.25, PI * 1.75);
            
            float radius = ofRandom(250, 500);
            float x = cos(theta) * radius + attractors[i].position.x;
            float y = sin(theta) * radius + attractors[i].position.y;
            
            addParticle(ofVec3f(x, y, z));
            resetDistanceLookups();
        }
    }
    
    if (particles.size() > attractors.size() * numberParticlesPerAttractor ) {
        particleDestroyTimer += lastFrameTime;
        if (particleDestroyTimer > particleDestroyTime) {
            particleDestroyTimer = 0;
            particleDestroyTime = ofRandom(0.02, 0.05);
            
            int randomStart = ofRandom(0, particles.size());
            for (int i  = 0; i < particles.size(); i++) {
                int index = (i + randomStart) % particles.size();
                if (particles[index].isAlive) {
                    particles[index].isDying = true;
                    break;
                }
            }
        }
    }
}

void AttractionSystem::updateAttractionSystem() {
    lastFrameTime = ofGetLastFrameTime();
    
    updateAttractors();

    setLastFrameTime(lastFrameTime);
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            if (isConverging) {
                attractTowardCenter(i);
            } else {
                attractTowardAttractors(i);
            }
        }
    });
    
    attractors.erase(std::remove_if(attractors.begin(), attractors.end(), [](Attractor& attractor) {
        return !attractor.isAlive;
    }), attractors.end());
    
    removeDeadParticles();
}

void AttractionSystem::attractTowardAttractors(int index) {
    particles[index].acceleration = ofVec3f::zero();
    
    if (isAttracting) {
        for (int j = 0; j < attractors.size(); j++) {
            ofVec2f force = attractors[j].position - particles[index].position;
            
            float magnitudeSquared = force.lengthSquared();
            magnitudeSquared = ofClamp(magnitudeSquared, 0.1, 100);
            float G = 500;
            float strength = magnitudeSquared / G;
            force = force.normalize() * strength;
            particles[index].acceleration += force;
        }
    }
    
    particles[index].velocity += particles[index].acceleration;
    
    if (particles[index].isDying) {
        particles[index].velocity *= 0.975;
    } else {
        particles[index].velocity *= 0.99;
        
        if (particles[index].velocity.length() < 3.5) {
            particles[index].velocity.normalize();
            particles[index].velocity *= 3.5;
            
            if (ofRandom(0.0, 1.0) < 0.02) {
                particles[index].velocity * 1.1;
            }
        } else {
            particles[index].velocity.limit(8.0);
        }
    }
    
    particles[index].position += particles[index].velocity;
}

void AttractionSystem::attractTowardCenter(int index) {
    particles[index].acceleration = ofVec3f::zero();
    
    if (isAttracting) {
        ofVec2f force = centerAttractor.position - particles[index].position;
        
        float magnitudeSquared = force.lengthSquared();
        magnitudeSquared = ofClamp(magnitudeSquared, 0.1, 100);
        float G = 1000;
        float strength = magnitudeSquared / G;
        force = force.normalize() * strength;
        particles[index].acceleration += force;
        
        particles[index].velocity += particles[index].acceleration;
    }
    
    particles[index].velocity *= 0.99;
    particles[index].velocity.limit(8.0);
    
    particles[index].position += particles[index].velocity;
}

int AttractionSystem::getAttractorVectorIndex(int index) {
    for (int i = 0; i < attractors.size(); i++) {
        if (attractors[i].index == index) return i;
    }
    
    return -1;
}

void AttractionSystem::addAttractor(int index, ofVec2f position) {
    Attractor attractor = Attractor(index);
    attractor.setPosition(position);
    attractor.numberParticles = numberParticlesPerAttractor;
    
    attractors.push_back(attractor);
}

void AttractionSystem::setAttractorPosition(int _index, ofVec2f position) {
    if (_index >= attractors.size()) return;
    attractors[_index].setPosition(position);
}

void AttractionSystem::drawAttractors() {
    ofSetCircleResolution(23);
    ofSetColor(ofColor::red);
    for (int i = 0; i < attractors.size(); i++) {
        ofDrawCircle(attractors[i].position.x, attractors[i].position.y, 13);
    }
}
