//
//  Neighbor.hpp
//

#ifndef Neighbor_hpp
#define Neighbor_hpp

#include <stdio.h>
#include "ofMain.h"

class Neighbor {
public:
    Neighbor();
    
    void update();
  
    bool isAlive, isInRange;
    float progress, progressIncrement;
    int index;
    ofVec2f position;
};

#endif /* Neighbor_hpp */
