//
//  Fragments.cpp
//  memory-planes-ofx
//

#include "Fragments.hpp"

Fragments::Fragments() {
    
}

void Fragments::setLastFrameTime(float lastFrameTime) {
    for (auto& fragment : fragments) {
        fragment.setLastFrameTime(lastFrameTime);
    }
}

void Fragments::update() {
    for (auto& fragment : fragments) {
        fragment.update();
    }
    
    fragments.erase(std::remove_if(fragments.begin(), fragments.end(), [](Fragment& fragment) {
        return !fragment.isAlive;
    }), fragments.end());
}

void Fragments::addFragment(float radius, float theta, float arcDistance, float instability) {
    Fragment fragment(radius, theta, arcDistance, instability);
    fragments.push_back(fragment);
}

void Fragments::draw() {
     ofSetCircleResolution(13);
     for (auto fragment : fragments) {
         fragment.draw();
     }
}
