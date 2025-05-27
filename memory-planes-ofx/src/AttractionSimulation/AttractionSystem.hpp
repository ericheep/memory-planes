//
//  AttractionSystem.hpp
//  memory-planes-ofx
//

#ifndef AttractionSystem_hpp
#define AttractionSystem_hpp

#include <stdio.h>
#include "ParticleSystem.hpp"
#include "Attractor.hpp"

class AttractionSystem : public ParticleSystem {
public:
    AttractionSystem();
    
    void drawAttractors();
    void updateAttractionSystem();
    void setAttractorPosition(int index, float x, float y);
    
    vector<Attractor> attractors;
};

#endif /* AttractionSystem_hpp */
