//
//  AttractionSystem.cpp
//  memory-planes-ofx
//

#include "AttractionSystem.hpp"

AttractionSystem::AttractionSystem() {
    
}

void AttractionSystem::update() {
    tbb::parallel_for( tbb::blocked_range<int>(0, particles.size()), [&](tbb::blocked_range<int> r) {
        for (int i = r.begin(); i < r.end(); ++i) {
        //particles[i].indicesWithinConnectionRadius = foreachPointWithinConnectionRadius(i);
        }
    });
}

void AttractionSystem::draw() {
    
}
