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
    virtual void updateForceRigid(PxRigidDynamic* solid, double duration);
    void setVolume(float x) { volume = x; if (volume < 0)volume = 0.0; cout << volume << "\n"; };
    float getVolume() { return volume; };
    void setHeight(float x) { height = x; if (height < 0)height = 0.0; cout << height << "\n"; };
    float getHeight() { return height; };
};

