//
//  AttractionSystem.cpp
//  memory-planes-ofx
//

#include "AttractionSystem.hpp"

AttractionSystem::AttractionSystem() {
    for (int i = 0; i < 2; i++) {
        Attractor attractor = Attractor();
        attractors.push_back(attractor);
    }
}

void AttractionSystem::updateAttractionSystem() {
    for (int i = 0; i < 2; i++) {
        attractors[i].setPosition(ofVec2f((-0.25 + 0.5 * i) * width, 0));
        attractors[i].update();
        if (attractors[i].canAddParticle) {
            // add particle
            attractors[i].canAddParticle = false;
            
            float theta = ofRandom(0, TWO_PI);
            
            float radius = ofRandom(25, 50);
            float x = cos(theta) * radius + attractors[i].position.x;
            float y = sin(theta) * radius + attractors[i].position.y;
                        
            addParticle(ofVec3f(x, y, 0));
            resetDistanceLookups();
        }
    }
    
    float maxDistance = 20;
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
            
            for (int j = 0; j < attractors.size(); j++) {
                ofVec2f force = attractors[i].position - particles[i].position;
                float magnitudeSquared = force.lengthSquared();
                magnitudeSquared = ofClamp(magnitudeSquared, 0.1, 25);
                float G = 200;
                float strength = magnitudeSquared / G;
                force = force.normalize() * strength;
                particles[i].acceleration += force;
            }
            
           // float distance = particles[i].position.distance(attractor.position);
            //float damping = ofMap(distance, 0, maxDistance, 0.95, 0.999, true);
                       
            particles[i].velocity += particles[i].acceleration;
            // particles[i].velocity *= damping;
            particles[i].position += particles[i].velocity;
            particles[i].acceleration = ofVec3f::zero();
        }
    });
}

void AttractionSystem::setAttractorPosition(int _index, float _x, float _y) {
    attractors[_index].setPosition(ofVec3f(_x, _y, 0));
}

void AttractionSystem::drawAttractors() {
    for (int i = 0; i < 2; i++) {
        ofDrawCircle(attractors[i].position.x, attractors[i].position.y, 15);
    }
}
