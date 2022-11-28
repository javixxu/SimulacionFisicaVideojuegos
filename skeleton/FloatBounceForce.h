#pragma once
#include "ForceGenerator.h"
class FloatBounceForce :public ForceGenerator {
protected:
    float height;
    float volume;
    float liquid_density;
    float gravity = 9.8;

    Particle* lqparticle;
public:
    FloatBounceForce(float h, float v, float d);
    ~FloatBounceForce();
    virtual void updateForce(Particle* p, double t);
};

