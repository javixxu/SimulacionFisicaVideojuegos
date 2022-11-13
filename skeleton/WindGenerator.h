#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
protected:
	Vector3 velWind;
public:	
	WindGenerator(const Vector3 vel={0,0,0}, const float k1=0, const float k2=0);
	virtual void updateForce(Particle* p, double duration);
	void setWindVelocity(Vector3 vel) { velWind = vel; };
};