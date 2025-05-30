//
//  Presence.cpp
//  memory-planes-ofx
//

#include "Presence.hpp"

Presence::Presence() {
    isAlive = true;
    totalLifetime = 1.0;
}

void Presence::setLastFrameTime(float _lastFrameTime) {
    lastFrameTime = _lastFrameTime;
}

void Presence::setPresence(float _x, float _y, float _blobWidth) {
    x = _x;
    y = _y;
    blobWidth = _blobWidth;
    
    lifetime = 0;
}

void Presence::update() {
    lifetime += lastFrameTime;
    
    if (lifetime > totalLifetime) {
        isAlive = false;
    }
}

