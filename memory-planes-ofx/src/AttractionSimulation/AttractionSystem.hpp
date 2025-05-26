//
//  AttractionSystem.hpp
//  memory-planes-ofx
//

#ifndef AttractionSystem_hpp
#define AttractionSystem_hpp

#include <stdio.h>
#include "ParticleSystem.hpp"

class AttractionSystem : public ParticleSystem {
public:
    AttractionSystem();
    
    void update();
    void draw();
};

#endif /* AttractionSystem_hpp */
