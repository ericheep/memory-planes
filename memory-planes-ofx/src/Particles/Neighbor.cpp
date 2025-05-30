//
//  Neighbor.cpp
//  memory-planes-ofx
//

#include "Neighbor.hpp"

Neighbor::Neighbor() {
    progressIncrement = 0.1;
    progress = 0.0;
    isAlive = true;
    position = ofVec2f::zero();
}

void Neighbor::update() {
    if (isInRange) {
        progress += progressIncrement;
    } else {
        progress -= progressIncrement;
    }
    
    if (progress > 1.0) progress = 1.0;
    if (progress < 0.0) isAlive = false;
}
