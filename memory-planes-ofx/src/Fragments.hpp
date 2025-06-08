//
//  Fragments.hpp
//  memory-planes-ofx
//

#ifndef Fragments_hpp
#define Fragments_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Fragment.hpp"

class Fragments {
public:
    Fragments();
    void update();
    void draw();
    void addFragment(float radius, float theta, float arcDistance, float instability);
    void setLastFrameTime(float lastFrameTime);
    
    vector <Fragment> fragments;
};

#endif /* Fragments_hpp */


