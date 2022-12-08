#pragma once
#include "SpringForceGenerator.h"
class ElasticBand :public SpringForceGenerator {
public:
    ElasticBand(Particle* other, double k, double resting_lenght) :SpringForceGenerator(other,  k,  resting_lenght){
    }
	inline void updateForce(Particle* p, double t) override{ 
        Vector3 f = p->getPosition();
        f -= other->getPosition();

        float lenght = f.normalize();
        lenght = (lenght - resting_lenght);
        if (lenght <= 0.0f) return;

        f *= -(lenght * k);
        p->addForce(f);
	}
    inline void updateForceRigid(PxRigidDynamic* solid, double duration) override {
        Vector3 f = solid->getGlobalPose().p;
        f -= other->getPosition();

        float lenght = f.normalize();
        lenght = (lenght - resting_lenght);
        if (lenght <= 0.0f) return;

        f *= -(lenght * k);
        solid->addForce(f);
    }
};

