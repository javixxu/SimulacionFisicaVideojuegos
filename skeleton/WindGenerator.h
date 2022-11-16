#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
protected:
	Vector3 velWind;
	Vector3 out;
	float range;
public:	
	WindGenerator(const Vector3 vel = { 0,0,0 }, const float k1 = 0, const float k2 = 0, Vector3 out= {0,0,0},float range=-1);
	virtual void updateForce(Particle* p, double duration);
	void setWindVelocity(Vector3 vel) { velWind = vel; };
	bool aplicateWindForce(Vector3 pos);
};