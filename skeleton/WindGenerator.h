#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
	Vector3 velWind;
public:	
	WindGenerator(const Vector3 vel={0,0,0}, const float k1=0, const float k2=0);
	void updateForce(Particle* p, double duration)override;
	void setWindVelocity(Vector3 vel) { velWind = vel; };
};