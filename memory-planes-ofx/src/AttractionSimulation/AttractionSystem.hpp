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
    AttractionSystem(int width, int height);
    
    void addAttractor(int index, ofVec2f position);
    int getAttractorVectorIndex(int index);
    
    void drawAttractors();
    void updateAttractors();
    void updateAttractionSystem();
    void setAttractorPosition(int index, ofVec2f position);
    void attractTowardAttractors(int index);
    void attractTowardCenter(int index);
    
    float width, height;
    int numberParticlesPerAttractor;
    float particleDestroyTimer, particleDestroyTime;
    bool isConverging, isAttracting;
    ofMesh mesh;
    vector<Attractor> attractors;
    Attractor centerAttractor;
};

#endif /* AttractionSystem_hpp */
